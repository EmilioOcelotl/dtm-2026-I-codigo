const int sensorPin = A0;   // Pin del sensor
const int threshold = 600;  // Umbral para encender LEDs (ajusta según tu sensor)

// Número de LEDs que quieres controlar (máximo 13)
const int numLeds = 3;

// Pines de los LEDs (modifica según tus conexiones)
int ledPins[] = {3, 5, 6};

void setup() {
  // Configura LEDs como salida
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  // Inicializa comunicación serial para depuración
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(sensorPin);

  // Imprime el valor para depuración
  Serial.print("Sensor: ");
  Serial.println(sensorValue); // Aquí pueden encontrar los valores del sensor para calibrar

  // Verifica umbral
  if (sensorValue >= threshold) {
    // Encender todos los LEDs
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], HIGH);
    }
  } else {
    // Apagar todos los LEDs
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], LOW);
    }
  }

  delay(50); // pequeña pausa
}
