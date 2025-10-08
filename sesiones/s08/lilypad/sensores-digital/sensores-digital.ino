// Pines de LEDs

// 0, 1, 2, 3, 4,  5,   6   7
int leds[] = {6, 7, 8, 9, 10, 11, 12, 13}; //
int numLeds = 8;
// leds.length = 8

// Pines del acelerómetro integrado (los marcados X, Y, Z en el LilyPad)
int pinX = A2;
int pinY = A3;
int pinZ = A4;

void setup()
{
    for (int i = 0; i < numLeds; i++)
    {
        pinMode(leds[i], OUTPUT);
        digitalWrite(leds[i], LOW);
    }
    // pinMOde 1
    Serial.begin(9600); //
}

void loop()
{
    // Leer valores del sensor (0–1023)
    int x = analogRead(pinX); // 500, 600
    int y = analogRead(pinY);
    int z = analogRead(pinZ);

    Serial.println(x); //
    Serial.println(y); //
    Serial.println(z); //

    // Ajustar este mapeo

    int mapX = map(x, 500, 600, 0, 100);
    int mapY = map(y, 500, 600, 0, 100);
    int mapZ = map(z, 500, 600, 0, 100);

    // Podemos obtener un promedio 
    int promedio = (mapX + mapY + mapZ) / 3; //

    Serial.println(promedio); // 40

    // Calcular intensidad del movimiento (comparado con un valor "en reposo")

    long movimiento = abs(x - 512) + abs(y - 512) + abs(z - 512);

    // Umbral para detectar si se agitó
    if (promedio > 100){ // sustituir movimiento por promedio
        parpadearLeds();
    }
    else{
        apagarLeds();
    }

    delay(50);
}

void parpadearLeds(){
    for (int i = 0; i < numLeds; i++){
        digitalWrite(leds[i], HIGH);
    }
    delay(1000);
    for (int i = 0; i < numLeds; i++){
        digitalWrite(leds[i], LOW);
    }
    delay(1000);
}

void apagarLeds(){
    for (int i = 0; i < numLeds; i++){
        digitalWrite(leds[i], LOW);
    }
}