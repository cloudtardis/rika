#include <Wire.h>
#include <EEPROM.h>
//#include <avr/pgmspace.h>
#include <I2Cdev.h>
#include <HMC5883L.h>
#include <MPU6050_6Axis_MotionApps20.h>

//pan(az) 0     tilt(alt) 1
byte pinSpeaker = 6;
byte pinBattery = 14;  //A0
byte pinMotor[2] = {7,10};                    //馬達訊號pin腳位
byte pinDir[2] = {8,11};                      //步進方向pin腳位
byte pinSleep[2] = {9,12};                    //步進開關pin腳位

byte state = 0;                               //馬達啟用開關
int motorfre = 20;                            //馬達刷新周期(ms)

//const String ParaName[] PROGMEM = {"accel", "speed", "range","rev","pwmrange","center","motortype","stepangle","TRK_tilttrim","compass","gps","LVW","batsw"};
String ParaName[13]= {"accel", "speed", "range","rev","pwmrange","center","motortype","stepangle","TRK_tilttrim","compass","gps","LVW","batsw"};

//需寫入EER記憶體位置
int limAcc[2] = {180,180};                    //limiter acceleration(degree/s^2)
int limVel[2] = {360,360};                    //limiter velocity(degree/s)
int maxRange[2] = {360,360};                  //馬達最大角度
int reverse[2] = {0,0};                       //正反轉對調
int pwmRange[2] = {1000,1000};                //servo pwm範圍(pwm max - pwm min)
int pwmCenter[2] = {1500,1500};               //servo 90度位置pwm值
volatile byte motorType[2] = {0,0};           //馬達種類 (0,1,2)(servo,none,stepper)
volatile float stepAngle[2] = {0.45,0.45};    //步進角度(1步)                          //需除以1000
int tilttrim = 0;                             //alt校正角度
byte compass = 0;                             //compass開關
byte gps = 0;                                 //gps開關
float battery = 11.1;                         //電池低電壓警告啟動電壓                  //需除以100
byte batsw = 0;                               //蜂鳴器開關
int c_xoffset = 0;                            //compass x軸校正值
int c_yoffset = 0;                            //compass y軸校正值
int c_zoffset = 0;                            //compass z軸校正值
float CompassGain = 1.0;                      //compass Y軸增益(為了與X軸輸出一樣範圍)  //需除以10000
float MagneticDeclination = 3.5;              //compass磁偏角                           //需除以100


volatile float Vi[2] = {0,0};                 //當前速度
volatile float positLeft[2] = {0,0};          //馬達還需要轉的角度
volatile float motorAngle[2] = {0,0};         //馬達目前位置
volatile byte flagTimer[2] = {0,0};           //timer(PWM)旗標
volatile unsigned long ocr1a[2] = {375,5000}; //計數器1中斷所需目標值(0,1)(高電位值,低電位值)
volatile unsigned long ocr2a[2] = {375,5000}; //計數器2中斷所需目標值(0,1)(高電位值,低電位值)
float rxAngle[2] = {0,0};
float compassAngle = 0;
unsigned long time;                           //運行時間

MPU6050 mpu;
#define OUTPUT_READABLE_YAWPITCHROLL
HMC5883L mag;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize = 42;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
//VectorInt16 aa;         // [x, y, z]            accel sensor measurements
//VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
//VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
//float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

bool blinkState = false;

void setup(){
  pinMode(4, INPUT);
  pinMode(pinSpeaker,OUTPUT);
  pinMode(pinDir[0], OUTPUT);
  pinMode(pinDir[1], OUTPUT);
  pinMode(pinMotor[0], OUTPUT);
  pinMode(pinMotor[1], OUTPUT);
  pinMode(pinSleep[0], OUTPUT);
  pinMode(pinSleep[1], OUTPUT);
  digitalWrite(pinSleep[0],1);
  digitalWrite(pinSleep[1],1);
  
  Serial.begin(115200);
  Wire.begin();
  mag.initialize();

while(1){
  mpu.initialize();
  devStatus = mpu.dmpInitialize();
  if (devStatus == 0) {
    mpu.setDMPEnabled(true);
    dmpReady = true;
    break;
  } 
  else {
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    Serial.print(F("DMP Initialization failed (code "));
    Serial.println(devStatus);
    delay(100);
  }
}

  timerInit();
  if(readEER(52) == 0){savePara();saveEER(52,1);}//第一次使用寫入EER
  readPara();//載入系統參數
  delay(3);
  initMotor(0);
  initMotor(1);
  delay(5);
  pinMode(13, OUTPUT);
}

unsigned long time1 = 0;
unsigned long timeTX = 0;
void loop(){
  time = millis();
  if(time1 != time){
    time1 = time;
    if(time % motorfre == 0){
      compute(rxAngle[0],rxAngle[1]);
    }
    if(time % 15 == 5){
      MPU6050();
    }
    if(time % 20 == 2){limiter(0);}
    else if(time % 20 == 3){limiter(1); }
    if(time % 100 == 15){  //設定周期
      sensor();
      
      compassAngle = getCompassDegree();
      blinkState = !blinkState;
      digitalWrite(13, blinkState);
    }
  }
  comportRx();
  send();
//  Serial.println("123");
}

