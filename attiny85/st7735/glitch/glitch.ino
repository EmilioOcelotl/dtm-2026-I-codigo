/*
  ATTINY85 ST7735 - Noise líquido estilo cyberpunk aleatorio
  Sin librerías externas
*/

#define CS_PIN   0
#define DC_PIN   3
#define RST_PIN  4
#define SCK_PIN  2
// MOSI PB1 hardware

#include <stdlib.h> // para rand()

// ================= SPI =================
void setupSPI() {
  DDRB |= (1 << PB1) | (1 << PB2);
  USICR = (1 << USIWM0) | (1 << USICS1) | (1 << USICLK);
}

void SPI_write(uint8_t data) {
  USIDR = data;
  USISR = (1 << USIOIF);
  while ((USISR & (1 << USIOIF)) == 0) USICR |= (1 << USITC);
}

// ================= TFT =================
void sendCommand(uint8_t cmd) {
  digitalWrite(DC_PIN, LOW);
  digitalWrite(CS_PIN, LOW);
  SPI_write(cmd);
  digitalWrite(CS_PIN, HIGH);
}

void sendData(uint8_t data) {
  digitalWrite(DC_PIN, HIGH);
  digitalWrite(CS_PIN, LOW);
  SPI_write(data);
  digitalWrite(CS_PIN, HIGH);
}

void initTFT() {
  pinMode(CS_PIN, OUTPUT);
  pinMode(DC_PIN, OUTPUT);
  pinMode(RST_PIN, OUTPUT);
  pinMode(SCK_PIN, OUTPUT);
  pinMode(1, INPUT);

  setupSPI();

  digitalWrite(CS_PIN, HIGH);
  digitalWrite(RST_PIN, HIGH);
  delay(100);
  digitalWrite(RST_PIN, LOW);
  delay(100);
  digitalWrite(RST_PIN, HIGH);
  delay(120);

  sendCommand(0x01); delay(120);
  sendCommand(0x11); delay(120);
  sendCommand(0x3A); sendData(0x05); delay(10);
  sendCommand(0x36); sendData(0xC0);
  sendCommand(0x29); delay(100);
}

void setAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
  sendCommand(0x2A);
  sendData(0x00); sendData(x0);
  sendData(0x00); sendData(x1);
  sendCommand(0x2B);
  sendData(0x00); sendData(y0);
  sendData(0x00); sendData(y1);
  sendCommand(0x2C);
}

// ================= NOISE =================
uint8_t hashNoise(int x, int y, int z) {
  int n = x + y * 57 + z * 131;
  n = (n << 13) ^ n;
  return (uint8_t)((1.0 - ((n * (n * n * 15731 + 789221) + 1376312589L)
          & 0x7fffffff) / 1073741824.0) * 127 + 128);
}

// ================= COLOR CYBERPUNK =================
uint16_t colorMap(uint8_t v) {
  uint8_t r = (v > 128) ? 255 : v * 2;
  uint8_t g = (v < 128) ? v * 2 : 0;
  uint8_t b = 255 - v;
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// ================= VARIABLES =================
float zf = 0.0;
float zStep = 0.1;
const uint8_t tileW = 8;
const uint8_t tileH = 8;
const uint8_t tilesX = 128 / tileW;
const uint8_t tilesY = 160 / tileH;
const uint8_t tilesPerFrame = 4;

// ================= DRAW =================
void drawNoiseTiles() {
  int zi = (int)zf;

  for (uint8_t t = 0; t < tilesPerFrame; t++) {
    // tile aleatorio
    uint8_t tx = rand() % tilesX;
    uint8_t ty = rand() % tilesY;

    int x0 = tx * tileW;
    int y0 = ty * tileH;

    // offset pseudoaleatorio dentro del tile
    int xOff = rand() % 4;
    int yOff = rand() % 4;

    setAddressWindow(x0, y0, x0 + tileW - 1, y0 + tileH - 1);
    digitalWrite(DC_PIN, HIGH);
    digitalWrite(CS_PIN, LOW);

    for (int y = 0; y < tileH; y++) {
      for (int x = 0; x < tileW; x++) {
        uint8_t val = hashNoise((x0 + x + xOff)/4, (y0 + y + yOff)/4, zi);
        uint16_t c = colorMap(val);
        SPI_write(c >> 8);
        SPI_write(c & 0xFF);
      }
    }

    digitalWrite(CS_PIN, HIGH);
  }

  zf += zStep;
}

void setup() {
  initTFT();
  delay(500);
  srand(analogRead(0)); // semilla para aleatorio
}

void loop() {
  drawNoiseTiles();
  delay(50); // ajusta velocidad
}
