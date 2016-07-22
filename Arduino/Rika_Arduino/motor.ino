void servo(byte k,float angle){//servo驅動
  positLeft[k] -= angle;
  motorAngle[k] += angle;
  int pwm = pwmCenter[k] + motorAngle[k]*2*pwmRange[k]/maxRange[k];
//  Serial.println(positLeft[k]);
  if(k == 0){ocr1a[0] = pwm/4;}
  else if(k == 1){ocr2a[0] = pwm/4;}
}

void stepper(byte k,int Num){//步進驅動
//  Serial.println(Num);
  if(Num == 0){//設定剩餘幾步以下忽略,設太大或太小都會在靜止時不穩
    timerStop(k);
    digitalWrite(pinSleep[k],1);//關閉步進
  }
  else{
    unsigned long i = motorfre*125/Num;
    digitalWrite(pinSleep[k],0);
    timerStart(k,i,i);
  }
}

void stepTimer(byte k){//步進目前座標寫入
  if(Vi[k] > 0){
    positLeft[k] -= stepAngle[k];
    motorAngle[k] += stepAngle[k];
  }
  else{
    positLeft[k] += stepAngle[k];
    motorAngle[k] -= stepAngle[k];
  }
}

void initMotor(byte k){
  timerStop(k);
  if(motorType[k] == 0){
    timerStart(k,375,5000);
  }
}
