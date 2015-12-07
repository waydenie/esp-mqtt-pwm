time_t getClockTime() {
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;

  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  tmElements_t s = {(uint8_t)second,(uint8_t)minute,(uint8_t)hour,(uint8_t)dayOfWeek,(uint8_t)dayOfMonth,(uint8_t)month,(uint8_t)(year+30)};
  Serial.print("Clock updated: ");
  Serial.print(hour); Serial.print(":"); Serial.print(minute); Serial.print(":"); Serial.print(second); Serial.print(" ");
  Serial.print(dayOfWeek); Serial.print(" "); Serial.print(dayOfMonth); Serial.print("/");
  Serial.print(month); Serial.print("/"); Serial.println(year);
  return makeTime(s);
}

void tock() {
  time_t curTime = now();
  if (lastTime < curTime) {
    process_clocktick=true;
    lastTime = curTime;
  }
}

void setTimeGauges (time_t curTime) {
  pwm.setPWM(15, 0, (uint16_t)(((double)hour(curTime)/24)*4095));
  pwm.setPWM(14, 0, (uint16_t)(((double)minute(curTime)/60)*4095));
  pwm.setPWM(13, 0, (uint16_t)(((double)second(curTime)/60)*4095));
//  pwm.setPWM(12, 0, (uint16_t)(((second(curTime)%2)*3481)+205)); // 5%-95%FS
  pwm.setPWM(12, 0, (uint16_t)((second(curTime)%2)*4095)); // 0%-100%FS - cause clock to tick

//Serial.print("curTime weekday: "); Serial.println(weekday(curTime));    
  pwm.setPWM(11, 0, (uint16_t)((weekday(curTime)/(double)8)*4095));  // 1of8(Sun) through 7of8(Sat)
  pwm.setPWM(10, 0, (uint16_t)((((double)day(curTime)-1)/30)*4095)); // 0-30 => 1-31
  pwm.setPWM( 9, 0, (uint16_t)((((double)month(curTime)-1)/11)*4095));
  pwm.setPWM( 8, 0, (uint16_t)(((year(curTime)%10)/(double)10)*4095));
}

void setGauges (time_t cntdwn_clock1, time_t cntdwn_clock2) {
  
}

//void tock() {
//  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
//  // retrieve data from DS3231
//  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
//
//  pwm.setPWM(15, 0, (uint16_t)(((double)hour/24)*4095));
//  pwm.setPWM(14, 0, (uint16_t)(((double)minute/60)*4095));
//  pwm.setPWM(13, 0, (uint16_t)(((double)second/60)*4095));
//  pwm.setPWM(12, 0, (uint16_t)(((second%2)*3481)+205)); // 5%-95%FS
//
//  pwm.setPWM(11, 0, (uint16_t)(((double)dayOfWeek/8)*4095)); // 1of8(Sun) through 7of8(Sat)
//  pwm.setPWM(10, 0, (uint16_t)((((double)dayOfMonth-1)/30)*4095)); // 0-30 => 1-31
//  pwm.setPWM( 9, 0, (uint16_t)((((double)month-1)/11)*4095));
//  pwm.setPWM( 8, 0, (uint16_t)(((year%10)/(double)10)*4095));
//  
//}

//From http://tronixlabs.com/news/tutorial-using-ds1307-and-ds3231-realtime-clock-modules-with-arduino/

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(RTC_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}
void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(RTC_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(RTC_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  // send it to the serial monitor
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute<10)
  {
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second<10)
  {
    Serial.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" Day of week: ");
  switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    break;
  case 2:
    Serial.println("Monday");
    break;
  case 3:
    Serial.println("Tuesday");
    break;
  case 4:
    Serial.println("Wednesday");
    break;
  case 5:
    Serial.println("Thursday");
    break;
  case 6:
    Serial.println("Friday");
    break;
  case 7:
    Serial.println("Saturday");
    break;
  }
}
