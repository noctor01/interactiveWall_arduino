#include <Wire.h>
#include "PCA9685.h"
#define angleA 160   //設定角度A
#define angleB 500   //設定角度B
#define servoNum 16

PCA9685 BOARD;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(100000);

  BOARD.resetDevices();                     //重設總線上設備
  BOARD.init(0);                            //設定地址
  BOARD.setPWMFrequency(50);                //無需要則不需改動

  BOARD.setChannelsPWM(0, 16, angleB);      //歸零角度
  Serial.println("device has started");
  delay(1500);
}

void loop() {
  for (byte x = 0; x < servoNum; x++) {
    BOARD.setChannelPWM(x, angleA);
    delay(200);
  }
  delay(3000);
  for (byte x = 0; x < servoNum; x++) {
    BOARD.setChannelPWM(x, angleB);
    delay(200);
  }
  delay(3000);
}


