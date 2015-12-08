#include "htmlpages.h"

void handle_conf() {
  
  if (HTTPserver.args() > 0 ) {
    for ( uint8_t i = 0; i < HTTPserver.args(); i++ ) {
      if (HTTPserver.argName(i) == "CLKMODE") {
         // do something here with value from server.arg(i);
         if (HTTPserver.arg(i) == "CLK") {
           Serial.println("Set Clock Mode");
           current_mode=MODE_CLOCK;
         } else if (HTTPserver.arg(i) == "CNTDWN") {
           Serial.println("Set Countdown Clock Mode");
           current_mode=MODE_COUNTDOWN;
         } else if (HTTPserver.arg(i) == "GAUGE") {
           Serial.println("Set Gauges Mode");
           current_mode=MODE_GAUGES;
         }
      }
   }



    delay(100);
  }
}

void handle_root() {
  HTTPserver.send(200, "text/html", index_html);
  Serial.println("HTTPServer request: /");
  delay(100);
}
void handle_setgauge() {
  HTTPserver.send(200, "text/html", setGauge_html);
  Serial.println("HTTPServer request: /setgauge");
  delay(100);
}
void handle_getgauge() {
  HTTPserver.send(200, "text/html", getGauge_html);
  Serial.println("HTTPServer request: /getgauge");
  delay(100);
}
void handle_gettime() {
  char curtime[100];
  char* htmlbuff;
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  snprintf(curtime,100,"%02d:%02d:%02d",hour,minute,second);
  Serial.print("curtime: "); Serial.println(curtime);
//  htmlbuff = (char*)malloc(strlen(curtime)+strlen(getTime_html));
//  
//  sprintf(htmlbuff,getTime_html,curtime);
//
//  HTTPserver.send(200, "text/html", htmlbuff);
//  Serial.println("HTTPServer request: /gettime");
//  free (htmlbuff);
  delay(100);
}

