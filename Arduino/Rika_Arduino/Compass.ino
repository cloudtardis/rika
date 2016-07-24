int c_axis[3] = {0,0,0};

void getCompassValues(){
  mag.getHeading(&c_axis[0], &c_axis[1], &c_axis[2]);
  
  c_axis[0] -= c_xoffset;
  c_axis[1] -= c_yoffset;
//  c_axis[0] = c_axis[0]*cos(ypr[])    //傾斜角度校正

//Serial.print("  ");
//Serial.print(c_axis[0]);
//Serial.print("  ");
//Serial.println(c_axis[1]);
//Serial.print("  ");
//Serial.println(c_axis[2]);
}

int getCompassDegree(){//取得compass角度
  if(mag.testConnection()){
    getCompassValues();
    float heading = atan2(c_axis[0], c_axis[1]);
    heading *= 57.3; //*180/PI
    heading -= MagneticDeclination;
    if(heading <= 0){heading += 360.0;}
    heading -= 180.0;
    Serial.println(heading);
    return heading;
//    if(reverse[0] == 0){return heading;}
//    else{return -1.0*heading;}
 }
  return motorAngle[0];
}

int correctCompass(){//自動校正(未完成) //調整轉速
  if(maxRange[0] < 300 && !mag.testConnection()){return 0;}
  int c_max[2] = {0,0};int c_min[2] = {0,0};
  getCompassDegree();
  int c_xy[2] = {c_axis[0],c_axis[1]};
  for(int i = 0; i < 4;i++){
    if(i == 0){positLeft[0] = 0  - motorAngle[0];}
    else if(i == 1){positLeft[0] = maxRange[0]/2  - motorAngle[0];}
    else if(i == 2){positLeft[0] = -1*maxRange[0]/2  - motorAngle[0];}
    else if(i == 3){positLeft[0] = 0  - motorAngle[0];}
    unsigned long time2 = millis();
    while(positLeft[0] != 0){
      time = millis();
      if(time % motorfre == 0 ){limiter(0);}
      if(time % 15 == 0 ){
        if(time2 - time > 30000){return 0;}
        getCompassDegree();
        if(c_max[0] < c_axis[0]){c_max[0] = c_axis[0];}
        else if(c_min[0] > c_axis[0]){c_min[0] = c_axis[0];}
        if(c_max[1] < c_axis[1]){c_max[1] = c_axis[1];}
        else if(c_min[1] > c_axis[1]){c_min[1] = c_axis[1];}
      }
    }
  }
  CompassGain = (c_max[0]-c_min[0])/(c_max[1]-c_min[1]);
  c_xoffset = (c_max[0]+c_min[0])/-2;
  c_yoffset = (c_max[1]+c_min[1])/-2;
  
  saveEER(36,c_xoffset);
  saveEER(38,c_yoffset);
  saveEER(42,CompassGain*10000);
}
