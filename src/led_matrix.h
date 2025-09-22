#include <Adafruit_NeoPixel.h>
#include "dice_symbols.h"

Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, WS_MATRIX_DIN, NEO_RGB + NEO_KHZ800);

void initMatrix() {
  Serial.println("📍 Initializing LED matrix...");
  bool success = strip.begin();
  if (!success) {
    Serial.println("❌ Failed to initialize LED matrix!");
    return;
  }
  Serial.println("✅ LED matrix initialized successfully!");
  strip.setBrightness(10);
  strip.show();
  Serial.println("✅ LED matrix ready!");
}

void displaySymbol(int symbolIndex, int rotation, uint32_t color) {
  for (int y = 0; y < matrixHeight; y++) {
    for (int x = 0; x < matrixWidth; x++) {
      int newX = x;
      int newY = y;

      // Apply rotation
      if (rotation == 1) {
        newX = matrixWidth - 1 - y;
        newY = x;
      } else if (rotation == 2) {
        newX = matrixWidth - 1 - x;
        newY = matrixHeight - 1 - y;
      } else if (rotation == 3) {
        newX = y;
        newY = matrixHeight - 1 - x;
      }

      if (diceSymbols[symbolIndex][y][x] == 1) {
        strip.setPixelColor(newY * matrixWidth + newX, color);
      } else {
        strip.setPixelColor(newY * matrixWidth + newX, 0);
      }
    }
  }
  strip.show();
}

void clearDisplay() {
  strip.clear();
  strip.show();
}
