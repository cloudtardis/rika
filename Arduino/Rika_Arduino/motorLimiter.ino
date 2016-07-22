void limiter(byte k){//馬達加減速計算(微分),計算要走角度
  static float T = motorfre/1000.0;
  int positDir = isPlusMinus(positLeft[k]);
  int vDir = isPlusMinus(Vi[k]);
  float Acc = limAcc[k];
  float A = Acc*vDir;
  float motorS = 0;
  
//int B = 0;
  
  if(Vi[k] == 0){
    if(positLeft[k] != 0){
      if(abs(positLeft[k]) < Acc*T*T){
//B=1;
        motorS = positLeft[k]/2;
        Vi[k] = positLeft[k]*2/T;
      }
      else{
//B=2;
        Vi[k] = positDir*Acc*T;
        motorS = Vi[k]*T/2;  //約等於0
      }
    }
  }
  else{
    float i = 0.0;
    if(positDir == vDir){i = abs(positLeft[k]);}
    if(Vi[k]*Vi[k]/Acc < i * 2){
//B=3;
      if((Vi[k]+T*A)*vDir > limVel[k]){
        float Vmax = limVel[k]*vDir;
        motorS = (Vmax*Vmax- Vi[k]*Vi[k])/(A*2)+Vmax*(T-(Vmax-Vi[k])/A);
        Vi[k] = Vmax;
      }
      else{
//B=4;
        motorS = Vi[k]*T + A*T*T/2;
        Vi[k] += A*T;
      }
    }
    else{
      if(Vi[k]/A <= T){
//B=5;
        motorS = Vi[k]*T/2;
        Vi[k] = 0;
      }
      else{
//B=6;
        A *= -1;
        motorS = Vi[k]*T + A*T*T/2;
        Vi[k] += A*T;
      }
    }
  }
  
/*
if(k == 1){
Serial.println(B);
Serial.println(positLeft[k]);
Serial.println(Vi[k]);
Serial.println(motorAngle[k]);
Serial.println(motorS);
}
*/
  if(motorType[k] == 0){
    motorS = positLeft[k];
    servo(k,motorS);
  }
  else if(motorType[k] == 2){
    int i = 0;
    if(Vi[k] > 0){i = 1;}
    digitalWrite(pinDir[k],i);
//    Serial.println(motorS);
//    Serial.println(abs(motorS/stepAngle[k]));
    stepper(k,abs(motorS/stepAngle[k]));
  }
}


int isPlusMinus(float data){//return數值正負
  if(data > 0){return  1;}
  else if(data < 0){return -1;}
  else{return 0;}
}
