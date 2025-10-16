#include <Adafruit_NeoPixel.h>

const int sensorPin = A1;      // Pin analógico para el sensor en Gemma M0
const int numPixels = 16;      // Cantidad de LEDs NeoPixel
const int pixelPin = 1;        // Pin digital para NeoPixels en Gemma M0

Adafruit_NeoPixel pixels(numPixels, pixelPin, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();              // Inicializa NeoPixel
  pixels.show();               // Apaga todos los LEDs inicialmente
  Serial.begin(9600);          // Serial para depuración
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  float sensorMap = map(sensorValue, 500, 600, 0, 1000) / 1000.0;  // normalizado 0-1

  if (sensorMap > 0.5) {  // ajuste de umbral
    for (int i = 0; i < 16; i++) {
      float gradiente = map(i, 0, 16, 0, 255);
      pixels.setPixelColor(i, pixels.Color(gradiente, sensorMap*255, 255 - (sensorMap*255)));
    }
  } else {
    for (int i = 0; i < numPixels; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
  }

  pixels.show();
  delay(50);
}