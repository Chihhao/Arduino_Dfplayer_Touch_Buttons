#include "CapacitiveSensor.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
//#define threshold 100  // 感測電容量的臨界值

#define MAX_VOL 25
#define SOFTTX A0
#define SOFTRX A1 

int dataPin = 2;   //Pin connected to DS of 74HC595
int latchPin = 3;  //Pin connected to ST_CP of 74HC595
int clockPin = 4;  //Pin connected to SH_CP of 74HC595
//int threshold[8] = {200, 50, 70, 200, 200, 200, 300, 300};
//int checktimes[8] = {100, 300, 300, 100, 100, 100, 100, 100};
int threshold[8] = {300, 300, 300, 300, 300, 300, 300, 300};
int checktimes[8] = {100, 100, 100, 100, 100, 100, 100, 100};

// 設定電容觸控的訊號輸入和輸出腳位
CapacitiveSensor touchButton[8] = {
  CapacitiveSensor(13,5),
  CapacitiveSensor(13,6),
  CapacitiveSensor(13,7),
  CapacitiveSensor(13,8),
  CapacitiveSensor(13,9),
  CapacitiveSensor(13,10),
  CapacitiveSensor(13,11),
  CapacitiveSensor(13,12)};

SoftwareSerial mySoftwareSerial(SOFTRX, SOFTTX);
DFRobotDFPlayerMini myDFPlayer;

int mp3_no_new = 0;
int mp3_no_old = 0;
int now_vol = 18;

int LEDSTATE = 0;


void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  Serial.begin(9600);
  mySoftwareSerial.begin(9600); 
  Serial.println();
  Serial.println(F("START"));   // 印出DFRobot DFPlayer Mini Demo字串到Serial通訊埠
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));    // 以下用法依此類通, 不再贅述喔

  if (!myDFPlayer.begin(mySoftwareSerial, true, false)){       
      Serial.println(F("Unable to begin:"));
      Serial.println(F("1.Please recheck the connection!"));
      Serial.println(F("2.Please insert the SD card!"));
      while (true);                                     
  }       
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.stop();
  myDFPlayer.setTimeOut(500);
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  myDFPlayer.enableDAC();

}

void loop() {
  // 熄滅大聲小聲指示燈
  LEDSTATE = LEDSTATE & B00111111;
  showLED(LEDSTATE);
  
  long touchValue = 0 ;

  // 檢查是否按下按鈕7(小聲)
  touchValue = touchButton[6].capacitiveSensor(checktimes[6]);
  if (touchValue >= threshold[6]){
    LEDSTATE = LEDSTATE | B01000000;
    showLED(LEDSTATE);
    if (now_vol > 1) now_vol -= 1;
    myDFPlayer.volume(now_vol);        
    Serial.print("Vol Down to "); Serial.println(now_vol);  
    delay(200);   
    return; 
  }
  
  // 檢查是否按下按鈕8(大聲)
  touchValue = touchButton[7].capacitiveSensor(checktimes[7]);
  if (touchValue >= threshold[7]){
    LEDSTATE = LEDSTATE | B10000000;
    showLED(LEDSTATE);
    if (now_vol < MAX_VOL) now_vol += 1;
    myDFPlayer.volume(now_vol);        
    Serial.print("Vol up to "); Serial.println(now_vol);  
    delay(200);
    return;
  }

  // 檢查是否按下按鈕1~6
  for(int i=0; i<6; i++){
    touchValue = touchButton[i].capacitiveSensor(checktimes[i]);  
    if (touchValue >= threshold[i]){
      int mask_led_on = B00000001;
      for(int j=0; j<i; j++){ mask_led_on <<= 1; } 
      int mask_led_off = mask_led_on ^ B11111111;

      /* //Debug Print
      Serial.print("=================="); Serial.println(); 
      Serial.print("i                ="); Serial.print(i); Serial.println(); 
      Serial.print("mask_led_on      ="); Serial.print(mask_led_on, BIN); Serial.println();
      Serial.print("mask_led_off     ="); Serial.print(mask_led_off, BIN); Serial.println();
      Serial.print("LEDSTATE>>i&1    ="); Serial.print(LEDSTATE>>i&1, BIN); Serial.println(); 
      Serial.print("LEDSTATE         ="); Serial.print(LEDSTATE, BIN); Serial.println(); 
      Serial.print("LEDSTATE & mask_led_off ="); Serial.print(LEDSTATE & mask_led_off, BIN); Serial.println(); 
      Serial.print("LEDSTATE | mask_led_on  ="); Serial.print(LEDSTATE | mask_led_on, BIN); Serial.println(); 
      */
      
      if( (LEDSTATE>>i)&1 == 1){
        LEDSTATE = LEDSTATE & mask_led_off;  //關燈
      }
      else{
        LEDSTATE = LEDSTATE | mask_led_on;  //亮燈
      }
      showLED(LEDSTATE);
      delay(100);
    }    
  }

  // 檢查是否播放完畢
  if (myDFPlayer.available()){
    //printDetail(myDFPlayer.readType(), myDFPlayer.read()); 
    
    if(myDFPlayer.readType() == DFPlayerPlayFinished){
      while(myDFPlayer.available()){ //清除殘留訊息
        printDetail(myDFPlayer.readType(), myDFPlayer.read()); 
      } 
      
      if(LEDSTATE<63){
        LEDSTATE++;
        showLED(LEDSTATE);          
      }
      else{
        LEDSTATE=1;
      }
    }    
  }    
  mp3_no_new = LEDSTATE;

  //如果有換首 
  if(mp3_no_new>0 && mp3_no_new != mp3_no_old){
    mp3_no_old = mp3_no_new;
    myDFPlayer.pause();    
    myDFPlayer.volume(now_vol); 
    myDFPlayer.playMp3Folder(mp3_no_new);    
    //myDFPlayer.loopFolder(mp3_no);  
    Serial.print("Play No: "); Serial.println(mp3_no_new);
    delay(100);
  }


  if(mp3_no_new == 0 && mp3_no_new != mp3_no_old){
    mp3_no_old = mp3_no_new;
    myDFPlayer.stop();    
    Serial.println("Stop");
    delay(100);
  }


}

void showLED(int iValue){
    digitalWrite(latchPin, LOW);
    delayMicroseconds(5);
    shiftOut(dataPin, clockPin, MSBFIRST, iValue);
    delayMicroseconds(5);
    digitalWrite(latchPin, HIGH);  
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  
}
