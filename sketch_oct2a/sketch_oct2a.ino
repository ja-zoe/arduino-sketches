#include <Wire.h>

void setup() {
    Serial.begin(115200);
    delay(5000);
    Wire.begin(6, 7);
    
    Serial.println("Scanning I2C bus...");
    for (byte addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {
            Serial.print("Device found at 0x");
            Serial.println(addr, HEX);
        }
    }
    Serial.println("Scan complete");

    pinMode(D7, OUTPUT);
}

void loop() {
  digitalWrite(D7, HIGH);
  delay(1000);
  digitalWrite(D7, LOW);
  delay(1000);
}