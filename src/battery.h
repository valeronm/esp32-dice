#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h>

static const int gndPin = 35; // GND pin for battery measurement
static const int SDA_PIN = 34;
static const int SCL_PIN = 33;

SFE_MAX1704X lipo;
TwoWire wire(1);

double voltage = 0; // Variable to keep track of LiPo voltage
double soc = 0; // Variable to keep track of LiPo state-of-charge (SOC)

void initBattery() {
  pinMode(gndPin, OUTPUT);
  digitalWrite(gndPin, LOW); // Connect GND pin to ground to enable battery measurement
  
  if (!wire.begin(SDA_PIN, SCL_PIN)) {
    Serial.println("⚠️ I2C bus for MAX1704x initialization failed. Check wiring.");
    return;
  }

  if (lipo.begin(wire) == false) {
    Serial.println("⚠️ MAX1704x not detected. Check wiring.");
    return;
  }

  lipo.quickStart();
  Serial.println("✅ Battery monitor initialized");
}

void updateBatteryStatus() {
  voltage = lipo.getVoltage();
  soc = lipo.getSOC();

  Serial.printf("🔋 Voltage: %.2f V, SOC: %.2f%%\n", voltage, soc);
}