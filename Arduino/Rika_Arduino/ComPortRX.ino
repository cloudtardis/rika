void comportRx(void){
  static char buffer[100]; //The traditional SERIAL buffer.
  static int counter = 0; //Traditional counter

  while(Serial.available() > 0){ //Check and loop while any byte is in the Serial Buffer. 
    buffer[counter] = Serial.read();//Reading the byte
    if(buffer[counter] == 0x0A){//Looks for \n, if not jump and continue receveing. 
      if (strncmp(buffer,"!!!",3) == 0){ //Looks for the !!! header
        rxAngle[0] = Search_Int(buffer,"PAN:");
        rxAngle[1] = Search_Int(buffer,"TLT:");
        rxAngle[0] = (rxAngle[0] - pwmCenter[0]) * 9 / 25;//360,1000,1500
        rxAngle[1] = (rxAngle[1] - pwmCenter[1]) * 9 / 50;//180,1000,1500
        rxAngle[1] = 90 - rxAngle[1];
      }
      
      else if (strncmp(buffer,"###",3) == 0){ //角度header(自製版)
        rxAngle[0] = Search_Int(buffer,"PAN:")/10;
        rxAngle[1] = Search_Int(buffer,"TLT:")/10;
      }
      
      else if (strncmp(buffer,"$$$",3) == 0){ //系統設定header(自製版)
        
        int i = 0;
        char *data[20];
        char *buff = buffer;
        char *posColon = NULL;
        char *posComma = NULL;
        while((data[i] = strtok_r(buff,",",&posComma)) != NULL){
          buff = data[i];
          while((data[i] = strtok_r(buff,":",&posColon)) != NULL){
            i++;
            buff = NULL;
          }
          buff = NULL; 
        }
        for(int j = 1;j < i;j += 3){
          byte k = atoi(data[j]);
          
          decode(k,data[j + 1],data[j + 2]);
        }
      }
     
      for(int i = 0;i <= counter;i++){buffer[i] = 0;} //Clearing the buffer
      counter = 0;
    }
    else{counter++;} //Incrementing the counter
    
    if(counter >= 100){//restarting the buffer
      for(int i = 0;i <= counter;i++){buffer[i] = 0;} //Clearing the buffer
      counter = 0; //Restarting the counter. 
      break;
    }
  }
}


float Search_Float(char buffer[],char looking[]){//以後資料輸入可以改這個
  char *BufferPointer = buffer;
  char *SearchString;
  SearchString = strstr(BufferPointer,looking);
  return atof(SearchString + 4);
}

int Search_Int(char buffer[],char looking[]){
  char *BufferPointer = buffer;
  char *SearchString;
  SearchString = strstr(BufferPointer,looking);
  return atoi(SearchString + 4);
}

long Search_Long(char buffer[],char looking[]){//目前無使用
  char *BufferPointer = buffer;
  char *SearchString;
  SearchString = strstr(BufferPointer,looking);
  return atol(SearchString + 4);
}

void decode(byte k,char *name,char *dat){//系統設定資料配對與作動
  String A(name);
  float C = atof(dat);//只能到小數點後兩位
  if(k == 2){
    if(A =="batimfo"){
      sendtoComputer(&batimfo);
    }
    else if(A =="tilttrim"){
      tilttrim = C;
    }
    else if(A =="button"){
      state = 1 - state;
    }
    else if(A =="LVW"){
      battery = C;
    }
    else if(A =="batsw"){
      batsw = C;
    }
    else if(A =="save"){
      savePara();
    }
    else if(A =="load"){
      readPara();
      sendtoComputer(&loadPara);
    }
    else if(A =="compass"){
      compass = C;
    }
    else if(A == "gps"){
      gps = C;
    }
    else if(A =="compasscorrect"){
      correctCompass();
    }
    else if(A =="c_xoffset"){
      c_xoffset = C;
    }
    else if(A =="c_yoffset"){
      c_yoffset = C;
    }
  }
  else if(k == 0 || k == 1){
    if(A == "motortype"){
      motorType[k] = C;
      initMotor(k);
    }
    else if(A =="accel"){
      limAcc[k] = C;
    }
    else if(A =="speed"){
      limVel[k] = C;
    }
    else if(A =="range"){
      maxRange[k] = C;
    }
    else if(A =="rev"){
      reverse[k] = C;
    }
    else if(A =="stepangle"){
      stepAngle[k] = C;
    }
    else if(A =="pwmrange"){
      pwmRange[k] = C;
    }
    else if(A =="center"){
      pwmCenter[k] = C;
    }
  }
}
