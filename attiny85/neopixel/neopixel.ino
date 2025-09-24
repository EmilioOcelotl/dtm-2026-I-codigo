#include <Adafruit_NeoPixel.h>

const int sensorPin = A2;    // Pin analógico para el sensor
const int threshold = 600;   // Umbral para encender/apagar NeoPixel

const int numPixels = 1;     // Cantidad de LEDs NeoPixel
const int pixelPin = 0;      // Pin digital conectado al NeoPixel (PB0 / pin 0)

Adafruit_NeoPixel pixels(numPixels, pixelPin, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin(); // Inicializa NeoPixel
  pixels.show();  // Apaga todos los LEDs inicialmente

  // Serial opcional para depuración
  // Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(sensorPin);

  // Serial.println(sensorValue); // Descomenta si quieres depurar

  if (sensorValue >= threshold) {
    // Encender NeoPixel en rojo (por ejemplo)
    for (int i = 0; i < numPixels; i++) {
      pixels.setPixelColor(i, pixels.Color(255, 0, 0));
    }
  } else {
    // Apagar NeoPixel
    for (int i = 0; i < numPixels; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
  }

  pixels.show();
  delay(50);
}
