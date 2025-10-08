const int sensorPin = A0;  // Pin del sensor
//const int threshold = 600;  // Umbral para encender LEDs (ajusta según tu sensor)

// Número de LEDs que quieres controlar (máximo 13)
const int numLeds = 6;

// Pines de los LEDs (modifica según tus conexiones)
int ledPins[] = { 2, 3, 4, 5, 6, 7 };  // pueden agregar el resto de leds

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
  Serial.println(sensorValue);  // Aquí pueden encontrar los valores del sensor para calibrar

  float sensorMap = map(sensorValue, 0, 1023, 0, 1);  // 500, 600

  if (sensorMap > 0.5) {         // umbral ahora es un rango que va de 0 a 1
    for (int i = 0; i < 2; i++)  // dos pines
      digitalWrite(ledPins[i], HIGH);
  }
  for (int i = 2; i < 6; i++) {  // cuatro pines
    digitalWrite(ledPins[i], LOW);
  }
}
else {
  for (int i = 0; i < 2; i++) {  // dos pines
    digitalWrite(ledPins[i], LOW);
  }
  for (int i = 2; i < 6; i++) {  // cuatro pines
    digitalWrite(ledPins[i], HIGH);
  }
}

delay(50);  // pequeña pausa
}