# Evidence: M5Stack StickS3 hardware

## Source documents

- M5Stack StickS3 documentation and pin map: https://docs.m5stack.com/en/core/StickS3
- M5Stack StickS3 Arduino programming documentation: https://docs.m5stack.com/en/arduino/m5sticks3/program
- M5Stack StickS3 Battery Arduino documentation: https://docs.m5stack.com/en/arduino/m5sticks3/battery
- M5Stack StickS3 Button Arduino documentation: https://docs.m5stack.com/en/arduino/m5sticks3/button
- M5Stack StickS3 Display Arduino documentation: https://docs.m5stack.com/en/arduino/m5sticks3/display
- M5Stack StickS3 IMU Arduino documentation: https://docs.m5stack.com/en/arduino/m5sticks3/imu
- M5Stack StickS3 IR NEC Arduino documentation: https://docs.m5stack.com/en/arduino/m5sticks3/ir_nec
- M5Stack StickS3 Microphone Arduino documentation: https://docs.m5stack.com/en/arduino/m5sticks3/mic
- M5Stack StickS3 Speaker Arduino documentation: https://docs.m5stack.com/en/arduino/m5sticks3/speaker
- M5Stack StickS3 Wakeup Arduino documentation: https://docs.m5stack.com/en/arduino/m5sticks3/wakeup
- M5Stack StickS3 M5PM1 Arduino documentation: https://docs.m5stack.com/en/arduino/m5sticks3/m5pm1
- M5Stack M5PM1 source repository: https://github.com/m5stack/M5PM1
- M5Stack M5Unified source repository: https://github.com/m5stack/M5Unified
- M5Stack M5GFX source repository: https://github.com/m5stack/M5GFX
- M5Stack M5GFX StickS3 initialization source: https://github.com/m5stack/M5GFX/blob/master/src/M5GFX.cpp
- StickS3 schematic PDF: https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/1207/K150_Stick_S3_PRJ_V0.6_20251111_2025_11_17_16_10_24.pdf
- ES8311 datasheet: https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/products/atom/Atomic%20Echo%20Base/ES8311.pdf
- BMI270 datasheet: https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/core/K128%20CoreS3/BMI270.PDF
- ESP32-S3 technical reference manual: https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/477/esp32-s3_technical_reference_manual_cn.pdf
- Espressif Bluetooth architecture documentation:
  https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/bt-architecture/overview.html

## M5Stack StickS3 facts

| Fact | Value |
| --- | --- |
| SoC | ESP32-S3-PICO-1-N8R8 |
| Flash | 8MB |
| PSRAM | 8MB |
| Battery capacity | 250mAh |
| LCD driver | ST7789P3 |
| LCD resolution | 135x240 |
| LCD pins | MOSI=GPIO39, SCK=GPIO40, RS=GPIO45, CS=GPIO41, RST=GPIO21, BL=GPIO38 |
| Internal I2C | SCL=GPIO48, SDA=GPIO47 |
| Internal I2C devices | BMI270 at 0x68, M5PM1 at 0x6e, ES8311 at 0x18 |
| Buttons | KEY1=GPIO11, KEY2=GPIO12 |
| IR | TX=GPIO46, RX=GPIO42 |
| Audio pins | GPIO18, GPIO14, GPIO17, GPIO15, GPIO16, GPIO48, GPIO47 |
| HAT2 available GPIO pins | GPIO5, GPIO4, GPIO6, GPIO1, GPIO7, GPIO8, GPIO43, GPIO44, GPIO2, GPIO3 |
| EXT_5V policy | Default input mode; do not enable output until verified safe for this radio carrier |
| M5PM1 L3B control | PYG2_L3B_EN powers LCD backlight, MIC, and speaker rail |
| M5PM1 charge status | PYG0_CHG_STAT reports charging state |
| M5PM1 PYG2 distinct from ESP32 GPIO2 | Yes |
| M5GFX L3B boot value | M5PM1 PYG2 output HIGH |
| M5GFX I2C idle-sleep boot value | PM1 I2C_CFG=0x00 |

## Battery implementation notes

- M5Stack's StickS3 battery example reads charging state, battery percentage, and battery voltage through `M5.Power`.
- M5Unified's StickS3 power path configures M5PM1 GPIO0 as the charge-status input.
- M5Unified reads M5PM1 battery voltage from registers `0x22` (`BAT_L`) and `0x23` (`BAT_H`) in millivolts.
- Meshtastic therefore needs an M5PM1-backed power source for this variant; there is no ESP32 `BATTERY_PIN` ADC
  divider in the StickS3 pin map.

## Cardputer-Adv comparison facts

| Fact | Value |
| --- | --- |
| LCD driver | ST7789V2 |
| LCD resolution | 240x135 |
| LCD pins | BL=GPIO38, RST=GPIO33, RS=GPIO34, DAT=GPIO35, SCK=GPIO36, CS=GPIO37 |
| LCD conclusion | ST7789 driver reuse only; do not reuse Cardputer-Adv pin map |
