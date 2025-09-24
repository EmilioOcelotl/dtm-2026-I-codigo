
const int sensorPin = A0;    // Pin analógico para el sensor
const int numLeds = 1;       // Cambia este valor: 1, 2 o 3 LEDs

// Define aquí los pines donde conectarás los LEDs
int ledPins[] = {3};  // Usa pines con PWM (~) máximo 6 (3, 5, 6, 9, 10 u 11)

void setup() {
  // Configura los pines de los LEDs como salida
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  // Leer sensor (0 a 1023)
  int sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue); // después pueden comentar esta linea para que no estorbe 

  // Mapear a rango PWM (0 a 255)
  // 0 a 1023 es el rango total
  // Pero es importante revisar en el monitor serial valores máximos y mínimos de entrada. 
  // Se ajustan en el segundo y tercer valor.
  // En mi caso 506 min 640 max funciona 
  
  int intensity = map(sensorValue, 0, 1023, 0, 255);

  // Escribir intensidad en todos los LEDs definidos
  for (int i = 0; i < numLeds; i++) {
    analogWrite(ledPins[i], intensity);
  }

  delay(10); // Pequeña pausa para estabilidad
}
