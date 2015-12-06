char html_buff[2048];

const char index_html[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
<head><title>ESP8266 Root Page</title></head>
<body>ESP8266 power!</body>
</html>
)=====";

const char setGauge_html[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
 <head><title>ESP8266 Set Gauges</title></head>
 <body>ESP8266 power!
 </body>
</html>
)=====";

const char getGauge_html[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
 <head><title>ESP8266 Get Gauges</title></head>
 <body>ESP8266 power!
 </body>
</html>
)=====";

const char getTime_html[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
 <head><title>ESP8266 Time</title></head>
 <body><h1>ESP8266</h1>
 <p>%s
 </body>
</html>
)=====";

