#include <Wire.h>
#include "PCA9685.h"
#define _angleA 160   //設定角度A
#define _angleB 500   //設定角度B

PCA9685 BOARD;
word angleA[16] = {_angleA, _angleA, _angleA, _angleA, _angleA, _angleA, _angleA, _angleA, _angleA, _angleA, _angleA, _angleA, _angleA, _angleA, _angleA, _angleA};//角度A
word angleB[16] = {_angleB, _angleB, _angleB, _angleB, _angleB, _angleB, _angleB, _angleB, _angleB, _angleB, _angleB, _angleB, _angleB, _angleB, _angleB, _angleB};//角度B




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


