#include <Wire.h>
#include "PCA9685.h"
int num;
PCA9685 board_1;
PCA9685 board_2;
word pwm_init[16] = {0};
word pwm_initStart[16] = {160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160};
bool mode_a, mode_b, mode_c, a, b;



void setup() {
  Serial.begin(115200);

  Wire.begin();                   // Wire must be started first
  Wire.setClock(400000);          // Supported baud rates are 100kHz, 400kHz, and 1000kHz

  board_1.resetDevices();         // Software resets all PCA9685 devices on Wire line
  board_1.init(0);                // Address pins A5-A0 set to B000000
  board_1.setPWMFrequency(50);    // Default is 50Hz, supports 24Hz to 1526Hz

  board_2.init(B000010);          // Address pins A5-A0 set to B000010
  board_2.setPWMFrequency(50);    // Default is 50Hz, supports 24Hz to 1526Hz
  
  board_1.setChannelsPWM(0, 16, pwm_initStart);   //歸零, 確保起始位置
  board_2.setChannelsPWM(0, 16, pwm_initStart);   //歸零, 確保起始位置
  Serial.println("device has started");
  delay(500);
}

void loop() {


//輸入資料##################################################
  char data[5];       //接收資料的buffer
  byte i = 0;
  char chr;   
  word pwms[16] = {0};
  if (Serial.available()) {
    while ((chr = Serial.read()) != '\n') {
      if (chr >= '0' && chr <= '9' && i < 5) {
        data[i] = chr;
        i++;
        a = true;
        b = false;
      }

      else if (chr >= 10) {
        data[i] = chr;
        i++;
        b = true;
        a = false;
      }
    }


    if (a == true && b ==  false) {
      data[i] = '\0';
      num = atoi(data);
      Serial.println(num);

      for (byte x = 0; x < 16; x++) {
        pwms[x] = num;
      }
    }
    if (b == true && a == false) {
      data[i] = '\0';
      Serial.println(data);
    }


//模式切換##################################################
    switch (data[0]) {
      case 'A':
        mode_a = true;
        mode_b = false;
        mode_c = false;
        Serial.println("Mode A starting....");
        break;
      case 'B':
        mode_b = true;
        mode_a = false;
        mode_c = false;
        Serial.println("Mode B starting....");
        break;
      case 'C':
        mode_c = true;
        mode_a = false;
        mode_b = false;
        Serial.println("Mode C starting....");
        break;      

    }

//啟動SERVO##################################################

    if (mode_a == true) {
      board_1.setChannelsPWM(0, 16, pwms);
      delay(800);
    }
    else if (mode_b == true) {
      board_2.setChannelsPWM(0, 16, pwms);
      delay(800);
    }
    else if (mode_c == true) {
      board_1.setChannelsPWM(0, 16, pwms);
      board_2.setChannelsPWM(0, 16, pwms);
      delay(800);
    }
  }
  board_1.setChannelsPWM(0, 16, pwm_init);			//turn off servo
  board_2.setChannelsPWM(0, 16, pwm_init);			//turn off servo
}


