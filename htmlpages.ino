#include "htmlpages.h"

void handle_index() {
  HTTPserver.send_P(200, "text/html", index_html);
  Serial.println("HTTPServer request: /");
}
void handle_status() {
  char esp_time[25],rtc_time[25];
  
  strDateTime(esp_time, now());
  strDateTime(rtc_time, getClockTime());
  
  snprintf_P(html_buff,HTML_BUFF_LEN, status_html, 
     C_run.wifi_ssid, C_save.wifi_ssid,
     C_run.wifi_ssid, C_save.wifi_ssid,
     C_run.wifi_ssid, C_save.wifi_ssid,
     C_run.wifi_ssid, C_save.wifi_ssid,
     esp_time, "NA",
     rtc_time, "NA",
     "NA", "NA",
     C_run.cntdwn_clock1, C_save.cntdwn_clock1,
     C_run.cntdwn_clock2, C_save.cntdwn_clock2
  );
  html_buff[HTML_BUFF_LEN]='\0';

  HTTPserver.send(200, "text/html", html_buff);
  Serial.println("HTTPServer request: /status.html");
}
void handle_stylemain() {
  HTTPserver.send_P(200, "text/css", stylemain_css);
  Serial.println("HTTPServer request: /stylemain.css");
}
void handle_style() {
  HTTPserver.send_P(200, "text/css", style_css);
  Serial.println("HTTPServer request: /style.css");
}
void handle_tswnavbar() {
  HTTPserver.send_P(200, "text/css", tswnavbar_css);
  Serial.println("HTTPServer request: /tswnavbar.css");
}

void handle_meters() {
  
  if (HTTPserver.args() > 0 ) {
    for ( uint8_t i = 0; i < HTTPserver.args(); i++ ) {
      if (HTTPserver.argName(i) == "CLKMODE") {
        if (HTTPserver.arg(i) == "TIMECLK") {
          Serial.println("Set Clock Mode");
          current_mode=MODE_CLOCK;
        } else if (HTTPserver.arg(i) == "CNTDWN") {
          Serial.println("Set Countdown Clock Mode");
          current_mode=MODE_COUNTDOWN;
        } else if (HTTPserver.arg(i) == "GAUGE") {
          Serial.println("Set Gauges Mode");
          current_mode=MODE_GAUGES;
        } 
      } else if (HTTPserver.argName(i) == "CNTDWN1") {
        if (HTTPserver.arg(i)) {
          C_run.cntdwn_clock1=(time_t)atol(HTTPserver.arg(i).c_str());
        }
      } else if (HTTPserver.argName(i) == "CNTDWN2") {
        if (HTTPserver.arg(i)) {
          C_run.cntdwn_clock2=(time_t)atol(HTTPserver.arg(i).c_str());
        }
      }
    }
  }

  html_sub_buff[0]='\0'; //start with a null buffer
  switch(current_mode) {
    case MODE_CLOCK :
      snprintf_P(html_sub_buff,HTML_BUFF_LEN, meters_timeclock_html,
        "<h1>Time/Date Clock</h1>"
      );
      break;
      
    case MODE_COUNTDOWN :
      char cntdwn_time1[25],cntdwn_time2[25];
  
      strDateTime(cntdwn_time1, C_run.cntdwn_clock1);
      strDateTime(cntdwn_time2, C_run.cntdwn_clock2);
      snprintf_P(html_sub_buff,HTML_BUFF_LEN, meters_countdown_html,
        C_run.cntdwn_clock1, cntdwn_time1,
        C_run.cntdwn_clock2, cntdwn_time2
      );
      break;
      
    case MODE_GAUGES :
      snprintf_P(html_sub_buff,HTML_BUFF_LEN, meters_gauges_html,
        "<h1>MQTT Gauges</h1>"
      );
      break;
  }
  html_sub_buff[HTML_BUFF_LEN]='\0'; //make sure we have a null term string, even if buffer wasn't big enough

  snprintf_P(html_buff,HTML_BUFF_LEN, meters_html, 
    current_mode==MODE_CLOCK ? "checked" : "",
    current_mode==MODE_COUNTDOWN ? "checked" : "",
    current_mode==MODE_GAUGES ? "checked" : "",
    html_sub_buff
  );    
  html_buff[HTML_BUFF_LEN]='\0';
  
  HTTPserver.send(200, "text/html", html_buff);
  Serial.println("HTTPServer request: /meters.html");

}

