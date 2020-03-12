#include <Wire.h>
#define TCAADDR 0x70

void TCA_select(uint8_t i) {
  if (i > 7) return;

  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

void setup() {
  Wire.begin();
  Serial.begin(115200);
}


void loop() {
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  for (uint8_t t = 0; t < 8; t++) {
    TCA_select(t);
    Serial.print("TCA Port #"); Serial.println(t);

    nDevices = 0;
    for (address = 1; address < 127; address++ ) {
      if (address == TCAADDR) continue;
      // The i2c_scanner uses the return value of
      // the Write.endTransmisstion to see if
      // a device did acknowledge to the address.
      Wire.beginTransmission(address);
      error = Wire.endTransmission();

      if (error == 0) {
        Serial.print("I2C device found at address 0x");
        if (address < 16)Serial.print("0");
        Serial.println(address, HEX);   
        nDevices++;

      } else if (error == 4) {
        Serial.print("Unknown error at address 0x");
        if (address < 16)
          Serial.print("0");
        Serial.println(address, HEX);
      }
    }
  }
  if (nDevices == 0)Serial.println("No I2C devices found\n");
  else {
    Serial.println("done\n");
  }
  delay(5000);           // wait 5 seconds for next scan
}
