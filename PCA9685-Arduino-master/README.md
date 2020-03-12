# PCA9685-Arduino - Version 1.0.0

I used this library to do some experiment and uploaded it to my github page that just only added some examples for myself.
i'm very appreciate Kasper Skårhøj for sharing this code that helping me to finish many expermient, and now if you have any questions about the examples, just feel free and contact me. thanks all!

Created by Kasper Skårhøj, August 3rd, 2012.  
Forked by Vitska, June 18th, 2016.  
Forked by NachtRaveVL, July 29th, 2016.

## Servo Control Note
許多180度受控的數字伺服器以20ms脈衝寬度（更新頻率為50Hz）的佔空比運行，並且沒有將整個脈衝寬度用於其0至180度控制。 通常，將20ms脈衝寬度（0.5ms）的2.5％視為0度，將20ms脈衝寬度（2.5ms）的12.5％視為180度。 對於0至180度的控制。
PCA9685的PWM解析度為12bit，也就是4096級，換算成以上對定的2.5%以及12.5%則為
4096 * 2.5% = 102 表示0度
4096 * 12.5% = 512 表示180度\n
這大致可轉換為102和512的原始PCA9685 PWM值，但可能需要進行調整以適合您的特定舵機（我也試過一些是90 ~ 450的，各種都有）。 
另請注意，某些舵機超過它們的0或180度運動可能會導致塑料限位器銷折斷卡住，也就是導致堵轉，此有可能會損壞舵機，每一品牌的舵機的電位器、內部IC皆不同，因此再請利用servo_initSerial這個範例來尋找你舵機相對空佔比所對應的角度。

## Example Usage

Below are several examples of library usage.

### I2C_scanner
將線路安裝好後，可透過利用I2C的ACK機制，測試該I2C地址是否有設備，用於確認是否有將線路安裝妥當
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
      Serial.println(address - 64);           //PCA9685的預設起始地址為0x40，表示64，因此減去64代表為第0張板子
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
注意要開啟串口的新行(new line)功能

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


### multiServoSerialTest

此範例為servo_initSerial進階延伸，用於串聯兩張PCA9685控制板，透過輸入A, B, C字符來切換三種模式分別控制不同板上的舵機
注意要開啟串口的新行(new line)功能

```Arduino
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

```

### servoBatching

此案例用了解於批次控制每一通道的舵機

```Arduino
#include <Wire.h>
#include "PCA9685.h"
#define angleA 160    //設定角度A
#define angleB 500    //設定角度B
#define servoNum 16   //設定舵機的數量，需要從0的通道開始安裝舵機

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

```
