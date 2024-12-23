#include "SdsDustSensor.h"
#include <FastLED.h>
#define NUM_LEDS 16
#define DATA_PIN 11
CRGB leds[NUM_LEDS];

int rxPin = 19;
int txPin = 18;
SdsDustSensor sds(Serial1);

void setup() {
  Serial.begin(9600);
  sds.begin();
  delay(1000);  // Allow time for the sensor to initialize

  Serial.println(sds.queryFirmwareVersion().toString());
  Serial.println(sds.setActiveReportingMode().toString()); // or setPassiveReportingMode()
  Serial.println(sds.setContinuousWorkingPeriod().toString());

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(10); 
  for (int i = 0; i < 16; i++) {
    leds[i] = CRGB::White; 
    FastLED.show(); 
    delay(100);
  }
  delay(1000);
}

void loop() {
  PmResult pm = sds.readPm();



  if (pm.isOk()) {
    float pm25 = pm.pm25;
    float pm10 = pm.pm10;
    Serial.print("PM2.5 = ");
    Serial.print(pm25);
    Serial.print(", PM10 = ");
    Serial.println(pm10);
    
    FastLED.setBrightness(50);
    if (pm25 > 500) {
      FastLED.showColor(CRGB::Red);
    } else if (pm25 > 250) {
      FastLED.showColor(CRGB::Yellow);
    } else {
      FastLED.showColor(CRGB::Green);
    }

    // Print measured values
    Serial.println(pm.toString());
  } else {
    // Handle error if sensor data is not available
    Serial.print("Could not read values from sensor, reason: ");
    Serial.println(pm.statusToString());
  }

  delay(2000);  // Delay for 1 second before the next reading
}
