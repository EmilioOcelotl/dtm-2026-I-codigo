const int sensorPin = A2;      // Pin analógico para el sensor (PB2 / A2)
const int threshold = 600;     // Umbral para encender/apagar LEDs

const int numLeds = 4;         // Cambia la cantidad de LEDs (máximo 5 si usas todos los pines digitales libres)
int ledPins[] = {0, 1, 3, 4};  // Pines digitales usados para LEDs (PB0, PB1, PB3, PB4)

void setup() {
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  // Serial opcional para depuración
  // Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(sensorPin);

  // Serial.println(sensorValue); // Descomenta si quieres depurar

  if (sensorValue >= threshold) {
    // Encender LEDs
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], HIGH);
    }
  } else {
    // Apagar LEDs
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], LOW);
    }
  }

  delay(50); // pequeña pausa
}
