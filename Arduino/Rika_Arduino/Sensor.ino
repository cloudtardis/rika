void sensor(){//偵測sensor數據
  if(batsw == 1 && analogRead(pinBattery)*15 < battery*1023){digitalWrite(pinSpeaker,HIGH);}//是否啟動蜂鳴器
  else{digitalWrite(pinSpeaker,LOW);}
}
