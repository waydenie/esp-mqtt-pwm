char html_buff[HTML_BUFF_LEN];
char html_sub_buff[HTML_BUFF_LEN];

const char index_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta content="text/html; charset=utf-8" http-equiv="content-type">
    <title>ESPWebConf</title>
    <link rel="stylesheet" type="text/css" href="stylemain.css">
    <link rel="stylesheet" type="text/css" href="style.css">
    <link rel="stylesheet" type="text/css" href="tswnavbar.css">
  </head>
  <body>
    <div class="yui-t1" id="doc3">
      <div id="hd">
        <h1>PAGE HEADER</h1>
      </div>
      <div id="bd">
        <div id="yui-main">
          <div class="yui-b">
            <div style="height: 400px;" class="yui-g"> <iframe name="content" src="status.html"
                frameborder="0" height="100%" width="100%"></iframe></div>
          </div>
        </div>
        <div class="yui-b">
          <div id="tswcssbuttons">
            <ul>
              <li><a href="status.html" target="content">Status</a></li>
              <li><a href="meters.html" target="content">Meters</a></li>
              <li><a href="network.html" target="content">Network</a></li>
              <li><a href="time.html" target="content">Time</a></li>
              <li><a href="mqtt.html" target="content">MQTT</a></li>
              <li><a href="about.html" target="content">About</a></li>
            </ul>
          </div>
        </div>
      </div>
      <div id="ft">
        <hr>ESPWebConf</div>
    </div>
  </body>
</html>
)=====";

const char status_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta content="text/html; charset=utf-8" http-equiv="content-type">
    <title>Status</title>
    <link rel="stylesheet" type="text/css" href="stylemain.css">
    <link rel="stylesheet" type="text/css" href="style.css">
  </head>
  <body>
    <div class="yui-t1" id="doc3">
      <h1>Status</h1>
      <table style="width: 80%" border="1">
        <thead>
          <tr>
            <th style="text-align: right;" width="20%"><br>
            </th>
            <th width="40%">running<br>
            </th>
            <th width="40%">saved<br>
            </th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td style="text-align: right;">WiFi SSID:</td>
            <td>%s</td>
            <td>%s<br>
            </td>
          </tr>
          <tr>
            <td style="text-align: right;">IP:</td>
            <td>%s</td>
            <td>%s<br>
            </td>
          </tr>
          <tr>
            <td style="text-align: right;">Netmask:</td>
            <td>%s</td>
            <td>%s<br>
            </td>
          </tr>
          <tr>
            <td style="text-align: right;">Gateway:</td>
            <td>%s</td>
            <td>%s<br>
            </td>
          </tr>
          <tr>
            <td style="text-align: right;">ESP Time:</td>
            <td>%s</td>
            <td>%s<br>
            </td>
          </tr>
          <tr>
            <td style="text-align: right;">RTC Time:</td>
            <td>%s</td>
            <td>%s<br>
            </td>
          </tr>
          <tr>
            <td style="text-align: right;">NTP Time:</td>
            <td>%s</td>
            <td>%s<br>
            </td>
          </tr>
          <tr>
            <td style="text-align: right;">Top Countdown Time:</td>
            <td>%lu</td>
            <td>%lu</td>
          </tr>
          <tr>
            <td style="text-align: right;">Bottom Countdown Time:</td>
            <td>%lu</td>
            <td>%lu</td>
          </tr>
        </tbody>
      </table>
    </div>
  </body>
</html>
)=====";

const char meters_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta content="text/html; charset=utf-8" http-equiv="content-type">
    <title>Meters</title>
    <link rel="stylesheet" type="text/css" href="stylemain.css">
    <link rel="stylesheet" type="text/css" href="style.css">
  </head>
  <body>
    <div class="yui-t1" id="doc3">
      <h1>Meters</h1>
      <fieldset><ledgend>Clock Mode</ledgend>
        <form>
          <input name="CLKMODE" value="TIMECLK" %s type="radio">Time Clock<br>
          <input name="CLKMODE" value="CNTDWN" %s type="radio">Countdown Clock<br>
          <input name="CLKMODE" value="GAUGE" %s type="radio">Guages<br>
          <br><input type="submit">
        </form>
      </fieldset>
      %s
    </div>
  </body>
</html>
)=====";

const char meters_timeclock_html[] PROGMEM = R"=====(
<fieldset><ledgend>Time Date Clock</ledgend>
  %s
<fieldset>
)=====";
const char meters_countdown_html[] PROGMEM = R"=====(
<fieldset><ledgend>Countdown Clock</ledgend>
  <form>
    <p>Top Count Down Date:
    <input name="CNTDWN1" value="%lu" type="text"> %s
    <p>Bottom Count Down Date:
    <input name="CNTDWN2" value="%lu" type="text"> %s
    <br><input type="submit">
  </form>
<fieldset>
)=====";
const char meters_gauges_html[] PROGMEM = R"=====(
<fieldset><ledgend>MQTT Gauges</ledgend>
  %s
<fieldset>
)=====";

const char style_css[] PROGMEM = R"=====(
/*
 * Copyright (c) 2008, Yahoo! Inc. All rights reserved.
 * Code licensed under the BSD License:
 * http://developer.yahoo.net/yui/license.txt
 * version: 2.6.0
 * */
h1{font-size:138.5%;}h2{font-size:123.1%;}h3{font-size:108%;}h1,h2,h3{margin:1em 0;}h1,h2,h3,h4,h5,h6,strong{font-weight:bold;}abbr,acronym{border-bottom:1px dotted #000;cursor:help;} em{font-style:italic;}blockquote,ul,ol,dl{margin:1em;}ol,ul,dl{margin-left:2em;}ol li{list-style:decimal outside;}ul li{list-style:disc outside;}dl dd{margin-left:1em;}th,td{border:1px solid #000;padding:.5em;}th{font-weight:bold;text-align:center;}caption{margin-bottom:.5em;text-align:center;}p,fieldset,table,pre{margin-bottom:1em;}input[type=text],input[type=password],textarea{width:12.25em;*width:11.9em;}
)=====";

const char stylemain_css[] PROGMEM = R"=====(
/*
 * Copyright (c) 2008, Yahoo! Inc. All rights reserved.
 * Code licensed under the BSD License:
 * http://developer.yahoo.net/yui/license.txt
 * version: 2.6.0
 * */
html{color:#000;background:#FFF;}body,div,dl,dt,dd,ul,ol,li,h1,h2,h3,h4,h5,h6,pre,code,form,fieldset,legend,input,textarea,p,blockquote,th,td{margin:0;padding:0;}table{border-collapse:collapse;border-spacing:0;}fieldset,img{border:0;}address,caption,cite,code,dfn,em,strong,th,var{font-style:normal;font-weight:normal;}li{list-style:none;}caption,th{text-align:left;}h1,h2,h3,h4,h5,h6{font-size:100%;font-weight:normal;}q:before,q:after{content:'';}abbr,acronym{border:0;font-variant:normal;}sup{vertical-align:text-top;}sub{vertical-align:text-bottom;}input,textarea,select{font-family:inherit;font-size:inherit;font-weight:inherit;}input,textarea,select{*font-size:100%;}legend{color:#000;}del,ins{text-decoration:none;}body{font:13px/1.231 arial,helvetica,clean,sans-serif;*font-size:small;*font:x-small;}select,input,button,textarea{font:99% arial,helvetica,clean,sans-serif;}table{font-size:inherit;font:100%;}pre,code,kbd,samp,tt{font-family:monospace;*font-size:108%;line-height:100%;}body{text-align:center;}#ft{clear:both;}#doc,#doc2,#doc3,#doc4,.yui-t1,.yui-t2,.yui-t3,.yui-t4,.yui-t5,.yui-t6,.yui-t7{margin:auto;text-align:left;width:57.69em;*width:56.25em;min-width:750px;}#doc2{width:73.076em;*width:71.25em;}#doc3{margin:auto 10px;width:auto;}#doc4{width:74.923em;*width:73.05em;}.yui-b{position:relative;}.yui-b{_position:static;}#yui-main .yui-b{position:static;}#yui-main,.yui-g .yui-u .yui-g{width:100%;}{width:100%;}.yui-t1 #yui-main,.yui-t2 #yui-main,.yui-t3 #yui-main{float:right;margin-left:-25em;}.yui-t4 #yui-main,.yui-t5 #yui-main,.yui-t6 #yui-main{float:left;margin-right:-25em;}.yui-t1 .yui-b{float:left;width:12.30769em;*width:12.00em;}.yui-t1 #yui-main .yui-b{margin-left:13.30769em;*margin-left:13.05em;}.yui-t2 .yui-b{float:left;width:13.8461em;*width:13.50em;}.yui-t2 #yui-main .yui-b{margin-left:14.8461em;*margin-left:14.55em;}.yui-t3 .yui-b{float:left;width:23.0769em;*width:22.50em;}.yui-t3 #yui-main .yui-b{margin-left:24.0769em;*margin-left:23.62em;}.yui-t4 .yui-b{float:right;width:13.8456em;*width:13.50em;}.yui-t4 #yui-main .yui-b{margin-right:14.8456em;*margin-right:14.55em;}.yui-t5 .yui-b{float:right;width:18.4615em;*width:18.00em;}.yui-t5 #yui-main .yui-b{margin-right:19.4615em;*margin-right:19.125em;}.yui-t6 .yui-b{float:right;width:23.0769em;*width:22.50em;}.yui-t6 #yui-main .yui-b{margin-right:24.0769em;*margin-right:23.62em;}.yui-t7 #yui-main .yui-b{display:block;margin:0 0 1em 0;}#yui-main .yui-b{float:none;width:auto;}.yui-gb .yui-u,.yui-g .yui-gb .yui-u,.yui-gb .yui-g,.yui-gb .yui-gb,.yui-gb .yui-gc,.yui-gb .yui-gd,.yui-gb .yui-ge,.yui-gb .yui-gf,.yui-gc .yui-u,.yui-gc .yui-g,.yui-gd .yui-u{float:left;}.yui-g .yui-u,.yui-g .yui-g,.yui-g .yui-gb,.yui-g .yui-gc,.yui-g .yui-gd,.yui-g .yui-ge,.yui-g .yui-gf,.yui-gc .yui-u,.yui-gd .yui-g,.yui-g .yui-gc .yui-u,.yui-ge .yui-u,.yui-ge .yui-g,.yui-gf .yui-g,.yui-gf .yui-u{float:right;}.yui-g div.first,.yui-gb div.first,.yui-gc div.first,.yui-gd div.first,.yui-ge div.first,.yui-gf div.first,.yui-g .yui-gc div.first,.yui-g .yui-ge div.first,.yui-gc div.first div.first{float:left;}.yui-g .yui-u,.yui-g .yui-g,.yui-g .yui-gb,.yui-g .yui-gc,.yui-g .yui-gd,.yui-g .yui-ge,.yui-g .yui-gf{width:49.1%;}.yui-gb .yui-u,.yui-g .yui-gb .yui-u,.yui-gb .yui-g,.yui-gb .yui-gb,.yui-gb .yui-gc,.yui-gb .yui-gd,.yui-gb .yui-ge,.yui-gb .yui-gf,.yui-gc .yui-u,.yui-gc .yui-g,.yui-gd .yui-u{width:32%;margin-left:1.99%;}.yui-gb .yui-u{*margin-left:1.9%;*width:31.9%;}.yui-gc div.first,.yui-gd .yui-u{width:66%;}.yui-gd div.first{width:32%;}.yui-ge div.first,.yui-gf .yui-u{width:74.2%;}.yui-ge .yui-u,.yui-gf div.first{width:24%;}.yui-g .yui-gb div.first,.yui-gb div.first,.yui-gc div.first,.yui-gd div.first{margin-left:0;}.yui-g .yui-g .yui-u,.yui-gb .yui-g .yui-u,.yui-gc .yui-g .yui-u,.yui-gd .yui-g .yui-u,.yui-ge .yui-g .yui-u,.yui-gf .yui-g .yui-u{width:49%;*width:48.1%;*margin-left:0;}.yui-g .yui-g .yui-u{width:48.1%;}.yui-g .yui-gb div.first,.yui-gb .yui-gb div.first{*margin-right:0;*width:32%;_width:31.7%;}.yui-g .yui-gc div.first,.yui-gd .yui-g{width:66%;}.yui-gb .yui-g div.first{*margin-right:4%;_margin-right:1.3%;}.yui-gb .yui-gc div.first,.yui-gb .yui-gd div.first{*margin-right:0;}.yui-gb .yui-gb .yui-u,.yui-gb .yui-gc .yui-u{*margin-left:1.8%;_margin-left:4%;}.yui-g .yui-gb .yui-u{_margin-left:1.0%;}.yui-gb .yui-gd .yui-u{*width:66%;_width:61.2%;}.yui-gb .yui-gd div.first{*width:31%;_width:29.5%;}.yui-g .yui-gc .yui-u,.yui-gb .yui-gc .yui-u{width:32%;_float:right;margin-right:0;_margin-left:0;}.yui-gb .yui-gc div.first{width:66%;*float:left;*margin-left:0;}.yui-gb .yui-ge .yui-u,.yui-gb .yui-gf .yui-u{margin:0;}.yui-gb .yui-gb .yui-u{_margin-left:.7%;}.yui-gb .yui-g div.first,.yui-gb .yui-gb div.first{*margin-left:0;}.yui-gc .yui-g .yui-u,.yui-gd .yui-g .yui-u{*width:48.1%;*margin-left:0;} .yui-gb .yui-gd div.first{width:32%;}.yui-g .yui-gd div.first{_width:29.9%;}.yui-ge .yui-g{width:24%;}.yui-gf .yui-g{width:74.2%;}.yui-gb .yui-ge div.yui-u,.yui-gb .yui-gf div.yui-u{float:right;}.yui-gb .yui-ge div.first,.yui-gb .yui-gf div.first{float:left;}.yui-gb .yui-ge .yui-u,.yui-gb .yui-gf div.first{*width:24%;_width:20%;}.yui-gb .yui-ge div.first,.yui-gb .yui-gf .yui-u{*width:73.5%;_width:65.5%;}.yui-ge div.first .yui-gd .yui-u{width:65%;}.yui-ge div.first .yui-gd div.first{width:32%;}#bd:after,.yui-g:after,.yui-gb:after,.yui-gc:after,.yui-gd:after,.yui-ge:after,.yui-gf:after{content:".";display:block;height:0;clear:both;visibility:hidden;}#bd,.yui-g,.yui-gb,.yui-gc,.yui-gd,.yui-ge,.yui-gf{zoom:1;}
)=====";

const char tswnavbar_css[] PROGMEM = R"=====(
/*
	tswnavbar.css	1.0.6
	Please use the CSS Menu Button Wizard at
		http://www.thesitewizard.com/wizards/css-menu-buttons.shtml
	to generate your own customized menu buttons.
*/
#tswcssbuttons {
	width: 140px ;
	text-align: center ;
}
#tswcssbuttons ul {
	margin: 0 ;
	padding: 0 ;
	list-style: none ;
}
#tswcssbuttons li {
	margin: 0 ;
	padding: 0 ;
	display: block ;
	list-style: none ;
	font-family: Arial, Helvetica, sans-serif ;
}
#tswcssbuttons li a {
	color: #ff0 ;
	background-color: #12127d ;
	border: 1px outset #9191ff ;
	width: 140px ;
	text-decoration: none ;
	display: block ;
}
#tswcssbuttons li a:hover {
	color: #000 ;
	background-color: #c8c8ff ;
}
)=====";

