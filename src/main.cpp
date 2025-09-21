#include "accelerometer.h"
#include "led_matrix.h"

// Configuration constants
static const int timeoutDuration = 5000;     // 5 seconds timeout to clear display
static const float sleepPreventMotion = 0.2; // 0.2g threshold to prevent sleep
static const float diceTriggerMotion = 1.0;  // 1.0g threshold for motion detection
static const uint32_t diceColor = strip.Color(255, 255, 255);

void setup() {
  Serial.begin(115200);

  initMatrix();
  initIMU();

  Serial.println("✅ Setup complete");
}

void showRandomDice(uint32_t color) {
  int value = random(0, 6); // Random value between 0 and 5
  int rotation = random(0, 4); // Random rotation between 0 and 3 (0, 90, 180, 270 degrees)
  displaySymbol(value, rotation, color);
}

void generateSymbol() {
  // Rotating dice
  for (int i = 0; i < 10; i++) {
    int randomHue = random(0, 65536);
    uint32_t rotatingColor = strip.ColorHSV(randomHue);
    showRandomDice(rotatingColor);
    delay(i * 25); // Increasing delay for slowing effect
  }

  // Final symbol
  showRandomDice(diceColor);
}

int lastActionTime = 0;

// Generate new symbol on 1g motion, clear display and sleep after timeout
void loop() {
  if (lastActionTime == 0)
    lastActionTime = millis();

  float accel = getAbsoluteAcceleration();

  if (accel > diceTriggerMotion) {
    generateSymbol();
    lastActionTime = millis();
  } else if (accel > sleepPreventMotion) {
    lastActionTime = millis();
  }

  if (millis() - lastActionTime > timeoutDuration) {
    clearDisplay();
    lastActionTime = 0;

    deepSleepAndWakeOnMotion();
  }

  delay(100);
}
