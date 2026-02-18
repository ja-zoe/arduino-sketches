void setup() {
    Serial.begin(115200);
    
    // Set attenuation for 0-3.3V range
    analogSetAttenuation(ADC_11db);  // Global setting
    // OR for specific pin:
    // analogSetPinAttenuation(A0, ADC_11db);
}

void loop() {
    uint32_t analogVolts = analogReadMilliVolts(A0) * 3;
    
    Serial.printf("ADC millivolts value = %d\n", analogVolts);
    delay(1000);
}