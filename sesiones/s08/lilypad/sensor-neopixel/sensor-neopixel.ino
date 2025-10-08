#include <Adafruit_NeoPixel.h> // Importante: instalar la biblioteca Neopixel de Adafruit

const int sensorPin = A0;  // Pin analógico para el sensor
//const int threshold = 600;   // Umbral para encender/apagar NeoPixel

const int numPixels = 16;  // Cantidad de LEDs NeoPixel
const int pixelPin = 8;    // Pin digital conectado al NeoPixel (PB0 / pin 0)

Adafruit_NeoPixel pixels(numPixels, pixelPin, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();  // Inicializa NeoPixel
  pixels.show();   // Apaga todos los LEDs inicialmente 0, 0, 0

  // Serial opcional para depuración
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  float sensorMap = map(sensorValue, 500, 600, 0, 1);  // normalizado

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