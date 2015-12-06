void callback(char* topic, byte* payload, unsigned int length) {
  char message_buff[length+1]; //+1 for terminating null
  char* tmp;
  char* gcmd;
  char* topicname = strdup(topic); //local copy since strtok is going to modify
  if (topicname == 0 ) { return; }

  snprintf(message_buff,length+1,"%s",payload); //make payload a null term char array

//  Serial.print("Message arrived: topic: ");
//  Serial.print(topicname);
//  Serial.print(" message: ");
//  Serial.println(message_buff);
  
  tmp = strtok(topicname,"/");
  if (strcmp(tmp, MQTTClientName) == 0) {
      tmp = strtok(NULL,"/");
      if (strcmp(tmp, "gauge")  == 0) {
        tmp = strtok(NULL,"/");
        if (strcmp(gcmd=strtok(NULL,"/"),"last") == 0) {
//          Serial.print("  Message in a _last_ gauge topic - gauge# ");
//          Serial.print(tmp);
//          Serial.print(" set to: ");
//          Serial.println(message_buff);
//          pwm.setPWM(atoi(tmp), 0, atoi(message_buff));
          pwm.setPWM((uint8_t)atoi(tmp), 0, (uint16_t)atoi(message_buff));
//          pwm.setPWM(14,0,2048);
        } else if (strcmp(gcmd, "max") == 0) {
//          Serial.print("  Message in a _max_ gauge topic - gauge# ");
//          Serial.print(atoi(tmp));
//          Serial.print(" set to: ");
//          Serial.println(atoi(message_buff));
        }
      }
  }
  free(topicname);
}

