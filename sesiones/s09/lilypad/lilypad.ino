// --- Configuración de pines ---
int sensorLuz = A2; // Pin analógico del LDR
int leds[] = {5, 6, 9, 10, 11}; // Pines de los 5 LEDs
int numLeds = 5;

// --- Rango de luz ---
int valorMin = 200;  // Valor cuando está muy oscuro
int valorMax = 800;  // Valor cuando hay mucha luz

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < numLeds; i++) {
    pinMode(leds[i], OUTPUT);
  }
}

void loop() {
  int valorLuz = analogRead(sensorLuz);
  Serial.println(valorLuz);

  // Mapeamos el valor de luz a una cantidad de LEDs encendidos
  // Entre 0 (oscuro) y numLeds (luz máxima)
  int nivel = map(valorLuz, valorMax, valorMin, 0, numLeds);
  nivel = constrain(nivel, 0, numLeds);

  // Enciende LEDs según el nivel calculado
  for (int i = 0; i < numLeds; i++) {
    if (i < nivel) {
      digitalWrite(leds[i], HIGH);
    } else {
      digitalWrite(leds[i], LOW);
    }
  }

  delay(100);
}
