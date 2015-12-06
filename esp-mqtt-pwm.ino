#include <TimeLib.h>
#include <Time.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
//#include <string.h>
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

void setup() {

  Wire.begin(PIN_I2C_SDA,PIN_I2C_SCL);

  Serial.begin(9600);
  Serial.println();
  Serial.println("MQTT / PWM test");
  Serial.println("===============");
//  Serial.print("sizeof(C)=");Serial.println(sizeof(C));

  delay(10);
  
  pwm.begin();
  pwm.setPWMFreq(1600);  

//xxvoid setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
//  setDS3231time(0,57,23,5,3,12,15);
  Serial.print("DS3231 time: ");
  displayTime();

  time_t t = now();
  Serial.print("ESP time: ");Serial.print(hour(t));Serial.print(":");Serial.print(minute(t));Serial.print(":");Serial.print(second(t));Serial.println(" ");
  
  tick.attach(0.1,tock);

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

}
