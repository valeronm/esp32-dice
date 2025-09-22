#include <BtHomeV2Device.h>
#include "NimBLEDevice.h"

NimBLEAdvertising *pAdvertising = nullptr;

// set up the buffer for the advertisement data
uint8_t advertisementData[MAX_ADVERTISEMENT_SIZE];
uint8_t size = 0;

void setAdvertisingPayload(float batteryVoltage, int batteryPercentage) {
  if (!pAdvertising) {
    Serial.println("Advertising not initialized!");
    return;
  }

  // Prepare BTHome V2 advertisement data
  BtHomeV2Device device("ESP32_DICE", "ESP32_DICE", false);
  device.addBatteryPercentage(batteryPercentage);
  device.addVoltage_0_to_65_resolution_0_001(batteryVoltage);
  size = device.getAdvertisementData(advertisementData);

  // Set the advertisement data
  NimBLEAdvertisementData pAdvData = BLEAdvertisementData();
  std::vector<uint8_t> data(advertisementData, advertisementData + size);
  pAdvData.addData(data);
  pAdvertising->setAdvertisementData(pAdvData);
}

void startAdvertising(float batteryVoltage, int batteryPercentage) {
  NimBLEDevice::init("");
  pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->setConnectableMode(0);
  setAdvertisingPayload(batteryVoltage, batteryPercentage);
  Serial.println("Starting advertising...");
  pAdvertising->start();
}

void stopAdvertising() {
  if (pAdvertising && pAdvertising->isAdvertising()) {
    Serial.println("Stopping advertising...");
    pAdvertising->stop();
  }
  // NimBLEDevice::deinit(false);
}