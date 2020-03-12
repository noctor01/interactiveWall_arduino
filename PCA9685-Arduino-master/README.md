# PCA9685-Arduino - Version 1.0.0

I used this library to do some experiment and uploaded it to my github page that just only added some examples for myself.
i'm very appreciate Kasper Skårhøj for sharing this code that helping me to finish many expermient, and now if you have any questions about the examples, just feel free and contact me. thanks all!

Created by Kasper Skårhøj, August 3rd, 2012.  
Forked by Vitska, June 18th, 2016.  
Forked by NachtRaveVL, July 29th, 2016.

## Servo Control Note

Many 180 degree controlled digital servos run on a 20ms pulse width (50Hz update frequency) based duty cycle, and do not utilize the entire pulse width for their -90/+90 degree control. Typically, 2.5% of the 20ms pulse width (0.5ms) is considered -90 degrees, and 12.5% of the 20ms pulse width (2.5ms) is considered +90 degrees. This roughly translates to raw PCA9685 PWM values of 102 and 512 (out of the 4096 value range) for -90 to +90 degree control, but may need to be adjusted to fit your specific servo (e.g. some I've tested run ~130 to ~525 for their -90/+90 degree control). Also be aware that driving some servos past their -90/+90 degrees of movement can cause a little plastic limiter pin to break off and get stuck inside of the gearing, which could potentially cause the servo to become jammed. See the PCA9685_ServoEvaluator class to assist with calculating PWM values from Servo angle values.

## Example Usage

Below are several examples of library usage.

### I2C_scanner
將線路安裝好後，可透過以下代碼測試I2C地址
```Arduino
#include <Wire.h>
void setup() {
  Serial.begin(115200);
  Wire.begin();
}

void loop() {
  scanner();
}

void scanner() {
  Serial.println("scannin....");
  byte error, address;
  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address ");
      if (address < 16) Serial.print("0");
      Serial.println(address - 64);
    }
  }
  delay(1000);
}


```

### simpleTest

用於簡單測試

```Arduino
#include <Wire.h>
#include "PCA9685.h"
#define _angleA 160   //設定角度A
#define _angleB 500   //設定角度B

PCA9685 BOARD;
word angleA[16] = {_angleA, _angleA, _angleA, _angleA, _angleA, _angleA, _angleA, _angleA, _angleA, _angleA, _angleA, _angleA, _angleA, _angleA, _angleA, _angleA};	//角度A
word angleB[16] = {_angleB, _angleB, _angleB, _angleB, _angleB, _angleB, _angleB, _angleB, _angleB, _angleB, _angleB, _angleB, _angleB, _angleB, _angleB, _angleB};	//角度B


void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(100000);

  BOARD.resetDevices();                     //重設總線上設備
  BOARD.init(0);                            //設定地址
  BOARD.setPWMFrequency(50);                //無需要則不需改動

  BOARD.setChannelsPWM(0, 16, angleB);      //歸零角度
  Serial.println("device has started");
  delay(500);
}

void loop() {
  BOARD.setChannelsPWM(0, 16, angleA);
  delay(3000);
  BOARD.setChannelsPWM(0, 16, angleB);
  delay(3000);
}

```

### servo_initSerial

此範例用於測試舵機的角度數值，透過串口輸入一個百位數，經典值一般介於100 ~ 550區間，去調整合適的空佔比來控制角度
記住要開啟串口的新行(new line)功能

```Arduino
#include <Wire.h>
#include "PCA9685.h"
int num;                      //用於接收後續之空占比值
PCA9685 BOARD;                //宣告一PCA9685
word pwm_init[16] = {0};      //設定初始的空占比, 於setup中輸出0值, 讓舵機不啟動



void setup() {
  Serial.begin(115200);
  Serial.println("STARTED!");
  Wire.begin();                           // Wire must be started first
  delay(10);
  Wire.setClock(100000);                  //設定I2C速度, 100,000bit/sec
  BOARD.resetDevices();                   //
  BOARD.init(0);                          //初始化I2C地址為B000000
  BOARD.setPWMFrequency(50);              //設定PWM的頻率週期為50ms
  BOARD.setChannelsPWM(0, 16, pwm_init);  //將16路的腳位輸出0值
  delay(100);
}

void loop() {
  char data[5];                           //最多包含四個字符資料與一個'\n'
  byte i = 0;                             
  char chr;
  word pwms[16] = {0};                    //定義一pwms用於接收串口輸入的數值, 並透過atoi指令轉出
  
  if (Serial.available()) {
    while ((chr = Serial.read()) != '\n') {
      if (chr >= '0' && chr <= '9' && i < 5) {
        data[i] = chr;
        i++;
      }
      else if (chr >= 10) {
        data[i] = chr;
        i++;
      }
    }

    data[i] = '\0';                       //最後一位加上'\0'表示該字符串結束
    num = atoi(data);                     //將該字符串通過array to int轉成整數型
    Serial.println(num);

    for (byte x = 0; x < 16; x++) {       //賦值
      pwms[x] = num;
    }
    
    BOARD.setChannelsPWM(0, 16, pwms);    //輸出
  }
}

```
:
