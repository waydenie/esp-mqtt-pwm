
#include <TimeLib.h>
#include <Time.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
#include <FS.h>
#include "EspMqttPwm.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(PWM_I2C_ADDRESS);

ESP8266WebServer HTTPserver(80);
WiFiClient wifiClient;
PubSubClient client(C.mqtt_server, 1883, callback, wifiClient);
Ticker tick;

void readConfig(Config *C) {

    File f=SPIFFS.open("/config.txt.wgn","r");
    if (!f) {
      Serial.println("Failed to open stored config.");
    } else {
      char line[256],key[256],val[256]; 
      while (f.available()) {

        f.readStringUntil('\n').toCharArray(line,sizeof(line));
        int linelen=strlen(line);
        int i=0,j=0;
        while(isspace(line[i]) && i<linelen) i++;
        if (i==linelen || line[i]=='#') continue; //skip comments and blank lines
        while(!(isspace(line[i]) || line[i]=='=') && i<linelen) {
          key[j] = tolower(line[i]);
          i++; j++;
        }
        key[j]='\0';
        j=0;
        while((isspace(line[i]) or line[i]=='=') && i<linelen) i++;
        if (i==linelen) continue;
        while(!isspace(line[i]) && i<linelen) {
          val[j] = line[i];
          i++; j++;
        }
        j = j>CONFIG_LEN ? CONFIG_LEN : j;
        val[j]='\0';

//        Serial.print(strlen(line)); Serial.println(line);
//        Serial.print("key:["); Serial.print(key); Serial.print("] val:["); Serial.print(val); Serial.print("]"); Serial.print(" j:"); Serial.println(j); 

        if (strcmp(key,"wifi_ssid") == 0) {
          memcpy(C->wifi_ssid,val,j);
        } else if (strcmp(key,"wifi_pwd") == 0) {
          memcpy(C->wifi_pwd,val,j);
        } else if (strcmp(key,"admin_pwd") == 0) {
          memcpy(C->admin_pwd,val,j);
        } else if (strcmp(key,"mqtt_server") == 0) {
          memcpy(C->mqtt_server,val,j);
        } else if (strcmp(key,"cntdwn_clock1") == 0) {
          C->cntdwn_clock1=(time_t)atoi(val);
        } else if (strcmp(key,"cntdwn_clock2") == 0) {
          C->cntdwn_clock2=(time_t)atoi(val);
        }
      }
      f.close();
    }
}

void setup() {

  Serial.begin(9600);
  Serial.println();
  Serial.println("MQTT / PWM test");
  Serial.println("===============");
//  Serial.print("sizeof(C)=");Serial.println(sizeof(C));

  if (!SPIFFS.begin()) {
    Serial.println("Mount of SPIFFS FS failed!");  
  } else {
    Serial.println("SPIFFS FS mounted.");
    readConfig(&C);
  }

  Wire.begin(PIN_I2C_SDA,PIN_I2C_SCL);
  delay(10);
  
  pwm.begin();
  pwm.setPWMFreq(1600);  

  setSyncProvider(getClockTime);
  while(timeStatus()== timeNotSet) {   
     delay(0); // wait until the time is set by the sync provider
  }
  
//xxvoid setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
//  setDS3231time(0,57,23,5,3,12,15);
  Serial.print("DS3231 time: ");
  displayTime();

  Serial.print("getClockTime: "); Serial.println(getClockTime());

  time_t t = now();
  Serial.print("ESP time: ");Serial.print(hour(t));Serial.print(":");Serial.print(minute(t));Serial.print(":");Serial.print(second(t));Serial.println(" ");
  
  tick.attach_ms(50,tock);

  Serial.print("ssid:");Serial.println(C.wifi_ssid);
  WiFi.begin(C.wifi_ssid, C.wifi_pwd);

  Serial.print("ESP-01 MAC: ");
  uint8_t mac[6];
  WiFi.macAddress(mac);
  for (int i=0; i<sizeof(mac); ++i) {
    sprintf(MAC_char,"%s%02x:",MAC_char,mac[i]);
  }
  MAC_char[strlen(MAC_char)-1]='\0';
  Serial.println(MAC_char);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  HTTPserver.on("/", handle_root);
  HTTPserver.on("/setgauge", handle_setgauge);
  HTTPserver.on("/getgauge", handle_getgauge);
  HTTPserver.on("/gettime", handle_gettime);
  HTTPserver.begin();
  Serial.println("HTTP HTTPserver started");
  
  // Generate client name based on MAC address and last 8 bits of microsecond counter
//  sprintf(MQTTClientName,"%-17s-%02x", MAC_char, micros() & 0xff,16);
  sprintf(MQTTClientName,"%-17s-%s", MAC_char, "xx"); //For debugging set -xx so that I'm not chasing the clientname

  Serial.print("Connecting to ");
  Serial.print(C.mqtt_server);
  Serial.print(" as ");
  Serial.println(MQTTClientName);

  if (client.connect(MQTTClientName)) {
    Serial.println("Connected to MQTT broker");

    MQTTTopicName = (char*)malloc(sizeof(char) * 100);
    snprintf(MQTTTopicName,100,"%s/gauge/#",MQTTClientName);
    if (client.subscribe(MQTTTopicName,0)) {
      Serial.print("Subscribe to topics ok: ");
      Serial.println(MQTTTopicName);
    }
    else {
      Serial.print("Subscribe to topics failed: ");
      Serial.println(MQTTTopicName);
    }
  }
  else {
    Serial.println("MQTT connect failed");
    Serial.println("Will reset and try again...");
    abort();
  }
}

void loop() {

  client.loop();
  HTTPserver.handleClient();

  if (process_clocktick) {
    switch(current_mode) {
      case MODE_CLOCK :
//        Serial.print("MODE_CLOCK time_t: ");Serial.println(lastTime);
//        Serial.print("ESP Clock: ");Serial.print(hour(lastTime));Serial.print(":");Serial.print(minute(lastTime)); Serial.print(":"); Serial.print(second(lastTime)); Serial.print(" ");
//        Serial.print(weekday(lastTime)); Serial.print(" "); Serial.print(day(lastTime)); Serial.print("/"); Serial.print(month(lastTime)); Serial.print("/"); Serial.println(year(lastTime));
        setTimeGauges(lastTime);
        break;
      
      case MODE_COUNTDOWN :
        Serial.print("MODE_CLOUNTDOWN: ");Serial.println("");
        setGauges((C.cntdwn_clock1 - lastTime), (C.cntdwn_clock2 - lastTime));
        break;
      
      case MODE_GAUGES :
        Serial.print("MODE_GAUGES: ");Serial.println("");
        break;
        
    }
    process_clocktick=false;
  }
}
