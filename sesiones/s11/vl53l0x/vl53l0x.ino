#include <Wire.h>
#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!lox.begin()) {
    while(1) { Serial.println("No se detectó el sensor"); delay(1000); }
  }
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  
  lox.rangingTest(&measure, false); // false = no imprimir debug

  if (measure.RangeStatus != 4) {  // 4 = out of range
    Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println(0); // sin lectura válida
  }
  
  delay(50); // 20 lecturas por segundo aprox.
}