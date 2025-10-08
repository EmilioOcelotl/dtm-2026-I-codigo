// Encender y apagar el LED incorporado en un Digispark (ATtiny85)

#define LED_BUILTIN 1   // El LED está conectado al pin 1 en Digispark

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // Configura el pin como salida
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH); // Enciende el LED
  delay(1000);                     // Espera 1 segundo
  digitalWrite(LED_BUILTIN, LOW);  // Apaga el LED
  delay(1000);                     // Espera 1 segundo
}