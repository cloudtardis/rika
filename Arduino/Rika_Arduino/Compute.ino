void compute(float az,float alt){
//unsigned long aa = micros(); 

  if(state == 0){
    if(compass == 1){//如有啟動compass進行校正角度
        az = az - compassAngle + motorAngle[0];
//      float azleft = az - compassAngle;
//      az =  azleft + motorAngle[0];
    }
    else{
//      float azleft  = az - motorAngle[0];
//      float motorTarget = az;
    }
     
    int pos = isPlusMinus(az);
//    while(az > 360){
//      az -= 360;
//    }
//    while(az < -360){
//      az += 360;
//    }
    
    static byte Reversion = 0;
    if(maxRange[0] >= 200 ){  //az超過180度時alt進行翻轉
      if(pos*az < 160 && Reversion == 1){
        Reversion = 0;
      }
      else if(pos*az > 200 && Reversion == 0){
        Reversion = 1;
      }
    }
    if(Reversion == 1){
      az -= pos*180;
      alt = -(alt);
    }
    
//    Serial.println(ypr[1]* 180/M_PI);
    alt = alt + tilttrim -(ypr[1] * 57.296);//alt角度校正 -(ypr[1] * 180/M_PI)
    
    int maxRandeX = maxRange[0]/2;
    int maxRandeY = maxRange[1]/2;
    alt = constrain(alt,-maxRandeY,maxRandeY);//alt馬達最大角度保護

    positLeft[0] = az - motorAngle[0];
    positLeft[1] = alt - motorAngle[1];

//    if(a > maxRandeX){positLeft[0] = maxRandeX - motorAngle[0];}  //az限制最大角度(未完成)
//    else if(a < -maxRandeX){positLeft[0] = -maxRandeX - motorAngle[0];}

//Serial.print("az "); 
//Serial.println(az); 
//Serial.print("moto "); 
//Serial.println(motorAngle[1]);

//    limiter(0);
//    limiter(1); 
//Serial.println(micros()-aa); 
  }
  else if(state == 01){
    positLeft[0] = 0;positLeft[1] = 0;
  }
}

void readPara(){//讀取資料EER
  for(int k = 0; k < 2;k++){
    limAcc[k] = readEER(0+2*k);
    limVel[k] = readEER(4+2*k);
    maxRange[k] = readEER(8+2*k);
    reverse[k] = readEER(12+2*k);
    pwmRange[k] = readEER(16+2*k);
    pwmCenter[k] = readEER(20+2*k);
    motorType[k] = readEER(24 + 2*k);
    stepAngle[k] = readEER(28 + 2*k)/1000.0;
  }
  tilttrim = readEER(32);
  compass = readEER(34);
  gps = readEER(36);
  c_xoffset = readEER(38);
  c_yoffset = readEER(40);
  c_zoffset = readEER(42);
  CompassGain = readEER(44)/10000.0;
  MagneticDeclination = readEER(46)/100.0;
  battery = readEER(48)/100.0;
  batsw = readEER(50);
}

void savePara(){//寫入資料EER
  for(int k = 0; k < 2;k++){
    saveEER(0+2*k,limAcc[k]);
    saveEER(4+2*k,limVel[k]);
    saveEER(8+2*k,maxRange[k]);
    saveEER(12+2*k,reverse[k]);
    saveEER(16+2*k,pwmRange[k]);
    saveEER(20+2*k,pwmCenter[k]);
    saveEER(24 + 2*k,motorType[k]);
    saveEER(28 + 2*k,stepAngle[k]*1000);
  }
  saveEER(32,tilttrim);
  saveEER(34,compass);
  saveEER(36,gps);
  saveEER(38,c_xoffset);
  saveEER(40,c_yoffset);
  saveEER(42,c_zoffset);
  saveEER(44,CompassGain*10000);
  saveEER(46,MagneticDeclination*100);
  saveEER(48,battery*100);
  saveEER(50,batsw);
}

void saveEER(int address,int data){
  if(readEER(address) != data){
    EEPROM.write(address,lowByte(data));
    EEPROM.write(address + 1,highByte(data));
  }
}

int readEER(int address){
  int b = EEPROM.read(address + 1);
  int a = ((b << 8) | EEPROM.read(address));
  return  a;
}
