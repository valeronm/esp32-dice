#include <esp_sleep.h>
#include <pins_arduino.h>

void deepSleep() {
  esp_sleep_enable_timer_wakeup(60 * 1000000); // 60 seconds sleep
  esp_sleep_enable_ext0_wakeup((gpio_num_t)WS_IMU_INT2, 0);

  esp_deep_sleep_start();
}