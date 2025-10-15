/*
 * ATtiny85 + ST7735
 * Imagen 80x160 horizontal (2 bits por píxel, 4 píxeles/byte)
 */

#define CS_PIN    0
#define DC_PIN    3
#define RST_PIN   4
#define SCK_PIN   2

#include <avr/pgmspace.h>
#include "attiny85_image.c"

const uint8_t SCREEN_WIDTH = 80;
const uint8_t SCREEN_HEIGHT = 160;

// ================= SPI =================
void setupSPI() {
  DDRB |= (1 << PB1) | (1 << PB2);  // MOSI, SCK
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

  setupSPI();

  digitalWrite(CS_PIN, HIGH);
  digitalWrite(RST_PIN, HIGH);
  delay(50);
  digitalWrite(RST_PIN, LOW);
  delay(50);
  digitalWrite(RST_PIN, HIGH);
  delay(120);

  sendCommand(0x01); delay(120);  // SW reset
  sendCommand(0x11); delay(120);  // Sleep out
  sendCommand(0x3A); sendData(0x05);  // 16-bit color
  
  // CORREGIR: Configurar MADCTL para orientación vertical
  sendCommand(0x36); 
  sendData(0x00);  // MX=1, MY=0, MV=1 (orientación vertical)
  
  sendCommand(0x29); delay(100);  // Display ON
}

void setAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
  sendCommand(0x2A);
  sendData(0x00); sendData(x0 + 26);  // Ajuste común para ST7735
  sendData(0x00); sendData(x1 + 26);
  sendCommand(0x2B);
  sendData(0x00); sendData(y0);
  sendData(0x00); sendData(y1);
  sendCommand(0x2C);
}

void displayImage() {
  setAddressWindow(0, 0, IMAGE_WIDTH - 1, IMAGE_HEIGHT - 1);
  digitalWrite(DC_PIN, HIGH);
  digitalWrite(CS_PIN, LOW);

  uint32_t total_pixels = (uint32_t)IMAGE_WIDTH * IMAGE_HEIGHT;
  uint32_t byte_count = total_pixels / 4;

  for (uint32_t i = 0; i < byte_count; i++) {
    uint8_t packed = pgm_read_byte(&image_data[i]);
    for (uint8_t n = 0; n < 4; n++) {
      uint8_t color_idx = (packed >> (6 - n * 2)) & 0x03;  // Orden corregido
      uint16_t color = pgm_read_word(&palette[color_idx]);
      SPI_write(color >> 8);
      SPI_write(color & 0xFF);
    }
  }

  digitalWrite(CS_PIN, HIGH);
}

// ================= MAIN =================
void setup() {
  initTFT();
  delay(200);
  displayImage();
}

void loop() {
  // nada
}
/*
 * ATtiny85 + ST7735
 * Imagen 80x160 horizontal (2 bits por píxel, 4 píxeles/byte)
 */

#define CS_PIN    0
#define DC_PIN    3
#define RST_PIN   4
#define SCK_PIN   2

#include <avr/pgmspace.h>
#include "attiny85_image.c"

const uint8_t SCREEN_WIDTH = 80;
const uint8_t SCREEN_HEIGHT = 160;

// ================= SPI =================
void setupSPI() {
  DDRB |= (1 << PB1) | (1 << PB2);  // MOSI, SCK
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

  setupSPI();

  digitalWrite(CS_PIN, HIGH);
  digitalWrite(RST_PIN, HIGH);
  delay(50);
  digitalWrite(RST_PIN, LOW);
  delay(50);
  digitalWrite(RST_PIN, HIGH);
  delay(120);

  sendCommand(0x01); delay(120);  // SW reset
  sendCommand(0x11); delay(120);  // Sleep out
  sendCommand(0x3A); sendData(0x05);  // 16-bit color
  
  // CORREGIR: Configurar MADCTL para orientación vertical
  sendCommand(0x36); 
  sendData(0x00);  // MX=1, MY=0, MV=1 (orientación vertical)
  
  sendCommand(0x29); delay(100);  // Display ON
}

void setAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
  sendCommand(0x2A);
  sendData(0x00); sendData(x0 + 26);  // Ajuste común para ST7735
  sendData(0x00); sendData(x1 + 26);
  sendCommand(0x2B);
  sendData(0x00); sendData(y0);
  sendData(0x00); sendData(y1);
  sendCommand(0x2C);
}

void displayImage() {
  setAddressWindow(0, 0, IMAGE_WIDTH - 1, IMAGE_HEIGHT - 1);
  digitalWrite(DC_PIN, HIGH);
  digitalWrite(CS_PIN, LOW);

  uint32_t total_pixels = (uint32_t)IMAGE_WIDTH * IMAGE_HEIGHT;
  uint32_t byte_count = total_pixels / 4;

  for (uint32_t i = 0; i < byte_count; i++) {
    uint8_t packed = pgm_read_byte(&image_data[i]);
    for (uint8_t n = 0; n < 4; n++) {
      uint8_t color_idx = (packed >> (6 - n * 2)) & 0x03;  // Orden corregido
      uint16_t color = pgm_read_word(&palette[color_idx]);
      SPI_write(color >> 8);
      SPI_write(color & 0xFF);
    }
  }

  digitalWrite(CS_PIN, HIGH);
}

// ================= MAIN =================
void setup() {
  initTFT();
  delay(200);
  displayImage();
}

void loop() {
  // nada
}
