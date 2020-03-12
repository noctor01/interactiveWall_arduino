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
