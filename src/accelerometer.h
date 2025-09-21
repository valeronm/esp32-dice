#include <SensorQMI8658.hpp>

SensorQMI8658 qmi;

void initIMU() {
  Serial.println("📍 Initializing sensor...");
  bool success = qmi.begin(Wire, QMI8658_L_SLAVE_ADDRESS, WS_IMU_SDA, WS_IMU_SCL);
  
  if (!success) {
    Serial.println("❌ Failed to initialize QMI8658!");
    return;
  }  
  Serial.println("✅ QMI8658 initialized successfully!");

  qmi.configAccelerometer(SensorQMI8658::ACC_RANGE_8G, SensorQMI8658::ACC_ODR_1000Hz);
  qmi.enableAccelerometer();

  Serial.println("✅ Configuration complete!");
}

float getAbsoluteAcceleration() {
  IMUdata acc;

  float accel = 0;

  if (qmi.getAccelerometer(acc.x, acc.y, acc.z)) {
    float accelMg = sqrt(acc.x * acc.x + acc.y * acc.y + acc.z * acc.z);
    accel = abs(accelMg - 1);
  }

  return accel;
}

void deepSleepAndWakeOnMotion() {
    qmi.configWakeOnMotion();
    esp_sleep_enable_ext0_wakeup((gpio_num_t)WS_IMU_INT2, 0);
    delay(100);

    esp_deep_sleep_start();
}