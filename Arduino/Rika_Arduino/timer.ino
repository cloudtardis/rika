volatile static unsigned long timerpara[2] = {0,0};

ISR(TIMER1_COMPA_vect)//timer1中段時程式碼
{
//  TCNT1 = 0;
  if(motorType[0] == 2){
    sei();
  }
  if(timerpara[0] == 0){
    digitalWrite(pinMotor[0],flagTimer[0]);
    flagTimer[0] = 1 - flagTimer[0];  //flag切換
    if(ocr1a[flagTimer[0]]>65000){  //是否超過計數器暫存器
      OCR1A = 65000;
      timerpara[0] = ocr1a[flagTimer[0]] - 65000;
    }
    else{
      OCR1A = ocr1a[flagTimer[0]];
      timerpara[0] = 0;
    }
    if(motorType[0] == 2 && flagTimer[0] == 0){
      stepTimer(0);
    }
  }
  else if(timerpara[0]>65000){
    OCR1A = 65000;
    timerpara[0] -= 65000;
  }
  else{
    OCR1A = timerpara[0];
    timerpara[0] = 0;
  }
}

ISR(TIMER2_COMPA_vect){ //timer2中段時程式碼
  //Serial.println(TCNT2);
  //TCNT2 = 0;
  if(motorType[1] == 2){
    sei();
  }
  if(timerpara[1] == 0){
    digitalWrite(pinMotor[1],flagTimer[1]);
    //Serial.println(flagTimer[1]);
    flagTimer[1] = 1 - flagTimer[1];
    if(ocr2a[flagTimer[1]]>250){
      OCR2A = 250;
      timerpara[1] = ocr2a[flagTimer[1]] - 250;
    }
    else{
      OCR2A = ocr2a[flagTimer[1]];
      timerpara[1] = 0;
    }
    if(motorType[1] == 2 && flagTimer[1] == 0){
      stepTimer(1);
    }
  }
  else if(timerpara[1]>250){
    OCR2A = 250;
    timerpara[1] -= 250;
  }
  else
  {
    OCR2A = timerpara[1];
    timerpara[1] = 0;
  }
}

void timerInit(){//初始化timer
  cli();
  TCCR1A = 0x00;
  TCCR1B |= _BV(WGM12);
  TCCR1B |= _BV(CS11);  // Timer1 1/64
  TCCR1B |= _BV(CS10);
  /*
  TCCR1B &= ~_BV(CS12);
  TCCR1B &= ~_BV(CS11); //Timer1關閉
  TCCR1B &= ~_BV(CS10);
  */
//  delayMicroseconds(16);
  TCCR2A |= _BV(WGM21);
  TCCR2B |= _BV(CS22);  // Timer2 1/64
  /*
  TCCR2B &= ~_BV(CS22);
  TCCR2B &= ~_BV(CS21);//Timer2關閉
  TCCR2B &= ~_BV(CS20);
  */
  OCR1A=300;
  OCR2A=250;
  //TCNT1=0;
  //TCNT2=0;
  //TIMSK1 |= _BV(OCIE1A);
  //TIMSK2 |= _BV(OCIE2A); 
  TIMSK1 &= ~_BV(OCIE1A);
  TIMSK2 &= ~_BV(OCIE2A);
  sei();
}

void timerStop(byte k){//關閉計數器
  if(k == 0 && TIMSK1 == 1){
    byte a = (TIMSK1 >> OCIE1A)&1;
    if(a == 1){
//      TCCR1B &= ~_BV(CS12);
//      TCCR1B &= ~_BV(CS11); //Timer1關閉
//      TCCR1B &= ~_BV(CS10);
      
      TIMSK1 &= ~_BV(OCIE1A);
//      TCNT1 = 0;//(counter stop)
      timerpara[0] = 0;
    }
  }
  else if(k == 1){
    byte a = (TIMSK2 >> OCIE2A)&1;
    if(a == 1){
      TIMSK2 &= ~_BV(OCIE2A);
//      TCNT2 = 0;//(counter stop)
      timerpara[1] = 0;
    }
  }
  digitalWrite(pinMotor[k],0);
  flagTimer[k] = 0;
}

void timerStart(byte k,unsigned long OCIE0,unsigned long OCIE1){//timer起動與設定timer中斷時間
  if(k == 0){
    ocr1a[0] = OCIE0;
    ocr1a[1] = OCIE1;
    if(TIMSK1 == 0){
      TCNT1 = 0;
      TIMSK1 |= _BV(OCIE1A);
      
//      TCCR1B |= _BV(WGM12);
//      TCCR1B |= _BV(CS11);  // Timer1 1/64
//      TCCR1B |= _BV(CS10);
    }
  }
  else if(k == 1){
    ocr2a[0] = OCIE0;
    ocr2a[1] = OCIE1;
    if(TIMSK2 == 0){
      TCNT2 = 0;
      TIMSK2 |= _BV(OCIE2A);
    }
  }
}

/*
void timerStart(byte k,unsigned long OCIE0){//寫給Servo用(但多餘的)目前直接寫在motor/servo裡面
  if(k == 0){
    ocr1a[0] = OCIE0;
    if(TIMSK1 == 0){
      TCNT1 = 0;
      TIMSK1 |= _BV(OCIE1A);
    }
  }
  else if(k == 1){
    ocr2a[0] = OCIE0;
    if(TIMSK2 == 0){
      TCNT2 = 0;
      TIMSK2 |= _BV(OCIE2A);
    }
  }
}
*/

