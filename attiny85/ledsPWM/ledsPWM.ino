const int sensorPin = A2;    // Pin analógico para el sensor (PB2 / A2)
const int numLeds = 2;       // Cambia entre 1 y 3 LEDs según disponibilidad PWM

// Pines con PWM disponibles en ATtiny85: PB0, PB1, PB3
int ledPins[] = {0, 1};      // PB0=0, PB1=1, PB3=3

void setup() {
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  // Inicializamos Serial solo si usamos un adaptador para debug (opcional)
  // Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(sensorPin);

  // Serial.println(sensorValue); // Solo si tienes adaptador y quieres debug

  int intensity = map(sensorValue, 0, 1023, 0, 255);

  for (int i = 0; i < numLeds; i++) {
    analogWrite(ledPins[i], intensity);
  }

  delay(10);
}
