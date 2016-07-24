static byte standby = 0;
static void (*f[5])();
void sendtoComputer(void (*b)()){
  if(standby < 4){
    f[standby] = b;
    standby++;
  }
}
void sendtoComputer(){
  if(standby != 0){
    Serial.print("$$$");
    for(int i = 0 ;i < standby;i++){
      (*f[i])();
    }
    Serial.print(",\n");
    standby = 0;
  }
}

void send(){
  if(digitalRead(4) == 0){
    timeTX = time + 40;
  }
  if(timeTX <= time){
    sendtoComputer();
    timeTX += 220;
  }
}

void batimfo(){
  Serial.print(",");
  Serial.print(2);
  Serial.print(":");
  Serial.print("batimfo");
  Serial.print(":");
  Serial.print(analogRead(pinBattery)*15/1023);
}

static byte l = 0;
void loadPara(){ //0:pan  1:tilt  2:other
  for(int k = 0;k < 2;k++){
    l = 0;
    senddata(k,limAcc[k]);
    senddata(k,limVel[k]);
    senddata(k,maxRange[k]);
    senddata(k,reverse[k]);
    senddata(k,pwmRange[k]);
    senddata(k,pwmCenter[k]);
    
    if(motorType[k] == 0){senddata(k,"Servo");}
    else{senddata(k,"Stepper");}
    
    senddata(k,stepAngle[k],0);
  }
  senddata(2,tilttrim);
  senddata(2,compass);
  senddata(2,gps);
  senddata(2,battery,1);
  senddata(2,batsw);
}

void senddata(byte k,int m){
  //String ParaName[13]= {"accel", "speed", "range","rev","pwmrange","center","motortype","stepangle","TRK_tilttrim","compass","gps","LVW","batsw"};
  Serial.print(",");
  Serial.print(k);
  Serial.print(":");
  Serial.print(ParaName[l]);
  Serial.print(":");
  Serial.print(m);
  l += 1;
}

void senddata(byte k,float m,byte n){//n test
//  String ParaName[13]= {"accel", "speed", "range","rev","pwmrange","center","motortype","stepangle","TRK_tilttrim","compass","gps","LVW","batsw"};
  Serial.print(",");
  Serial.print(k);
  Serial.print(":");
  Serial.print(ParaName[l]);
  Serial.print(":");
  Serial.print(m);
  l += 1;
}

void senddata(byte k,String m){
 // String ParaName[13]= {"accel", "speed", "range","rev","pwmrange","center","motortype","stepangle","TRK_tilttrim","compass","gps","LVW","batsw"};
  Serial.print(",");
  Serial.print(k);
  Serial.print(":");
  Serial.print(ParaName[l]);
  Serial.print(":");
  Serial.print(m);
  l += 1;
}
