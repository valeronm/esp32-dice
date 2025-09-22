#include "accelerometer.h"
#include "led_matrix.h"
#include "battery.h"
#include "sleep.h"
#include "bthome.h"

// Configuration constants
static const int timeoutDuration = 5000;     // 5 seconds timeout to clear display
static const float sleepPreventMotion = 0.2; // 0.2g threshold to prevent sleep
static const float diceTriggerMotion = 1.0;  // 1.0g threshold for motion detection
static const uint32_t diceColor = strip.Color(255, 255, 255);

enum WakeUpReason {
  EXT0_WAKEUP = 1,
  EXT1_WAKEUP,
  TIMER_WAKEUP,
  TOUCHPAD_WAKEUP,
  UNDEFINED_WAKEUP
};

WakeUpReason wakeupReason;

void checkWakeUpReason() {
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0:
      wakeupReason = EXT0_WAKEUP;
      strip.setPixelColor(0, strip.Color(0, 255, 0)); // Green for EXT0
      strip.show();
      break;
    case ESP_SLEEP_WAKEUP_EXT1:
      wakeupReason = EXT1_WAKEUP;
      strip.setPixelColor(0, strip.Color(255, 0, 0)); // Red for EXT1
      strip.show();
      break;
    case ESP_SLEEP_WAKEUP_TIMER:
    wakeupReason = TIMER_WAKEUP;
      strip.setPixelColor(0, strip.Color(0, 0, 255)); // Blue for TIMER
      strip.show();
      break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD:
      wakeupReason = TOUCHPAD_WAKEUP;
      strip.setPixelColor(0, strip.Color(255, 255, 0)); // Yellow for TOUCHPAD
      strip.show();
      break;
    default:
      wakeupReason = UNDEFINED_WAKEUP;
      strip.setPixelColor(0, strip.Color(128, 0, 128)); // Purple for unknown
      strip.show();
      break;
  }
}

void setup() {
  Serial.begin(115200);

  initMatrix();
  initIMU();
  initBattery();

  checkWakeUpReason();
  delay(100);
  clearDisplay();

  Serial.println("✅ Setup complete\n\n\n");
}

void showRandomDice(uint32_t color) {
  int value = random(0, 6); // Random value between 0 and 5
  int rotation = random(0, 4); // Random rotation between 0 and 3 (0, 90, 180, 270 degrees)
  displaySymbol(value, rotation, color);
}

enum AdvertisementState {
  STARTING,
  ADVERTISING,
  STOPPING,
  IDLE
};

void processAdvertisement() {
  static AdvertisementState state = STARTING;
  static unsigned long stateChangedTime = 0;

  switch (state) {
    case STARTING:
      updateBatteryStatus();
      startAdvertising(voltage, soc);

      Serial.println("📢 Started BLE advertising");
      state = ADVERTISING;
      stateChangedTime = millis();
      break;
    case ADVERTISING:
      updateBatteryStatus();
      setAdvertisingPayload(voltage, soc);
      if (millis() - stateChangedTime > 1000) {

        Serial.println("📢 Stopping BLE advertising after 1 second");
        state = STOPPING;
        stateChangedTime = millis();
      }
      break;
    case STOPPING:
      stopAdvertising();

      Serial.println("📢 Stopped BLE advertising");
      state = IDLE;
      stateChangedTime = millis();
      break;
    case IDLE:
      break;
  }
}

enum DiceState {
  SYSTEM_WAKEUP,
  WAITING,
  GENERATING,
  DISPLAYING,
  SLEEPING
};

void processDice() {
  static DiceState state = wakeupReason == TIMER_WAKEUP ? SYSTEM_WAKEUP : WAITING;
  static unsigned long stateChangedTime = 0;

  float accel = getAbsoluteAcceleration();

  switch (state) {
    case SYSTEM_WAKEUP:
      if (accel > sleepPreventMotion) {
        Serial.println("🎲 Motion detected, going to waiting");
        state = WAITING;
        break;
      }
      // For timer wakeup allow 1 second to detect motion before sleeping again
      if (millis() - stateChangedTime > 1000) {
        Serial.println("💤 No motion detected. Going to sleep...");
        state = SLEEPING;
        stateChangedTime = millis();
      }
      break;
    case WAITING:
      if (accel > diceTriggerMotion) {
        Serial.println("🎲 Motion detected, generating dice symbol");
        state = GENERATING;
        stateChangedTime = millis();
        break;
      }
      if (accel > sleepPreventMotion) {
        stateChangedTime = millis(); // Reset timer to prevent sleep
      }
      if (millis() - stateChangedTime > timeoutDuration) {
        Serial.println("💤 Timeout reached. Going to sleep...");
        state = SLEEPING;
        stateChangedTime = millis();
      }
      break;
    case GENERATING:
      for (int i = 0; i < 10; i++) {
        int randomHue = random(0, 65536);
        uint32_t rotatingColor = strip.ColorHSV(randomHue);
        showRandomDice(rotatingColor);
        delay(i * 25); // Increasing delay for slowing effect
      }
    
      Serial.println("🎲 Generated new dice symbol");
      state = DISPLAYING;
      stateChangedTime = millis();
      break;
    case DISPLAYING:
      showRandomDice(diceColor);

      Serial.println("🎲 Displaying dice symbol");
      state = WAITING;
      stateChangedTime = millis();
      break;
    case SLEEPING:
      configWakeOnMotionInterrupt();

      strip.setPixelColor(0, strip.Color(128, 128, 128));
      strip.show();
      delay(100);
      strip.clear();
      strip.show();

      deepSleep();
      break;
  }
}

void loop() {
  processAdvertisement();
  processDice();

  delay(100);
}
