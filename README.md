# ESP32 Dice

This project is an ESP32-based electronic dice using a Waveshare ESP32-S3 Matrix board. It utilizes an accelerometer to detect motion and displays dice symbols on an LED matrix.

## Features
- Uses accelerometer for motion detection
- Automatic sleep mode to minimize power consumption

## Hardware

- Waveshare ESP32-S3 Matrix board ([Link](https://www.waveshare.com/esp32-s3-matrix.htm))

## Getting Started
1. **Install PlatformIO**: [PlatformIO Installation Guide](https://platformio.org/install)
2. **Clone the repository**:
   ```sh
   git clone https://github.com/valeronm/esp32-dice.git
   cd esp32-dice
   ```
3. **Connect your Waveshare ESP32-S3-Matrix board**
4. **Build and upload the firmware**:
   ```sh
   pio run --target upload
   ```
5. **Monitor serial output**:
   ```sh
   pio device monitor
   ```

## Usage
Shake the device to roll the dice. The result will be displayed on the LED matrix.

## Power Consumption
The device features an automatic sleep mode to minimize power consumption when not in use. However, due to the quiescent current of the LEDs, the total current consumption in deep sleep is approximately 25 mA.

## License
MIT
