#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Wire.h>
#include "PCA9685.h"
#define TIME 550
#define ServoInUse 10
#define BoardInUse 2
PCA9685 b[2];
uint16_t pwm_A[ServoInUse] = {140, 140, 140, 140, 140, 140, 140, 140, 140, 140};
uint16_t pwm_B[ServoInUse] = {480, 480, 480, 480, 480, 480, 480, 480, 480, 480};
uint16_t pwm_C[ServoInUse] = {510, 510, 510, 510, 510, 510, 510, 510, 510, 510};

//################################
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
//################################
int addr = 0;
byte num;
unsigned long value;
//################################
void setup() {

  //################################
  Wire.begin();
  Wire.setClock(100000);
  Serial.begin(115200);
  Serial.println("START");

  b[0].resetDevices();
  for (byte x = 0; x < BoardInUse; x++) {
    b[x].init(x);
    b[x].setPWMFrequency(50);
  }
  for (byte x = 0; x < BoardInUse; x++) {
    b[x].setChannelsPWM(0, ServoInUse, pwm_A);
  }
  delay(200);
  //################################
  lcd.begin(16, 2);
  lcd.print("Counting:");

  //################################
}

void loop() {
  //################################




  for (byte x = 0; x < 4; x++) {
    b[0].setChannelsPWM(0, ServoInUse, pwm_A);
    b[1].setChannelsPWM(0, ServoInUse, pwm_A);
    delay(TIME);
    b[0].setChannelsPWM(0, ServoInUse, pwm_B);
    b[1].setChannelsPWM(0, ServoInUse, pwm_C);
    delay(TIME);
  }
  

  //################################
  while (EEPROM.read(addr) == 255)addr += 1;
  num = EEPROM.read(addr);
  value = addr * 255L + num;
  Serial.println(value);
  if (num == 255)num = 0;
  EEPROM.write(addr, ++num);
  //################################
  lcd.setCursor(0, 1);
  lcd.print(value * 4L);
}
