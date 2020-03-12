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
