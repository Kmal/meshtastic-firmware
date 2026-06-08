# Evidence: M5Stack StickS3 hardware

M5STACK_STICKS3_DOC_URL: https://docs.m5stack.com/en/core/StickS3
M5STACK_STICKS3_PM1_DOC_URL: https://docs.m5stack.com/en/arduino/m5sticks3/m5pm1
M5STACK_M5PM1_LIBRARY_URL: https://github.com/m5stack/M5PM1
M5STACK_M5GFX_STICKS3_REFERENCE: https://github.com/m5stack/M5GFX/blob/master/src/M5GFX.cpp

## Source documents

M5Stack StickS3 documentation and pin map: https://docs.m5stack.com/en/core/StickS3
M5Stack StickS3 Arduino programming documentation: https://docs.m5stack.com/en/arduino/m5sticks3/program
M5Stack StickS3 Battery Arduino documentation: https://docs.m5stack.com/en/arduino/m5sticks3/battery
M5Stack StickS3 Button Arduino documentation: https://docs.m5stack.com/en/arduino/m5sticks3/button
M5Stack StickS3 Display Arduino documentation: https://docs.m5stack.com/en/arduino/m5sticks3/display
M5Stack StickS3 IMU Arduino documentation: https://docs.m5stack.com/en/arduino/m5sticks3/imu
M5Stack StickS3 IR NEC Arduino documentation: https://docs.m5stack.com/en/arduino/m5sticks3/ir_nec
M5Stack StickS3 Microphone Arduino documentation: https://docs.m5stack.com/en/arduino/m5sticks3/mic
M5Stack StickS3 Speaker Arduino documentation: https://docs.m5stack.com/en/arduino/m5sticks3/speaker
M5Stack StickS3 Wakeup Arduino documentation: https://docs.m5stack.com/en/arduino/m5sticks3/wakeup
M5Stack StickS3 M5PM1 Arduino documentation: https://docs.m5stack.com/en/arduino/m5sticks3/m5pm1
M5Stack M5PM1 source repository: https://github.com/m5stack/M5PM1
M5Stack M5Unified source repository: https://github.com/m5stack/M5Unified
M5Stack M5GFX source repository: https://github.com/m5stack/M5GFX
M5Stack M5GFX StickS3 initialization source: https://github.com/m5stack/M5GFX/blob/master/src/M5GFX.cpp
StickS3 schematic PDF: https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/1207/K150_Stick_S3_PRJ_V0.6_20251111_2025_11_17_16_10_24.pdf
ES8311 datasheet: https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/products/atom/Atomic%20Echo%20Base/ES8311.pdf
BMI270 datasheet: https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/core/K128%20CoreS3/BMI270.PDF
ESP32-S3 technical reference manual: https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/477/esp32-s3_technical_reference_manual_cn.pdf
Espressif Bluetooth architecture documentation: https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/bt-architecture/overview.html

## M5Stack StickS3 facts

STICKS3_SOC: ESP32-S3-PICO-1-N8R8
STICKS3_FLASH: 8MB
STICKS3_PSRAM: 8MB
STICKS3_LCD_DRIVER: ST7789P3
STICKS3_LCD_RESOLUTION: 135x240
STICKS3_LCD_PINS: MOSI=GPIO39,SCK=GPIO40,RS=GPIO45,CS=GPIO41,RST=GPIO21,BL=GPIO38
STICKS3_INTERNAL_I2C: SCL=GPIO48,SDA=GPIO47
STICKS3_BUTTONS: KEY1=GPIO11,KEY2=GPIO12
STICKS3_IR: TX=GPIO46,RX=GPIO42
STICKS3_AUDIO_PINS: GPIO18,GPIO14,GPIO17,GPIO15,GPIO16,GPIO48,GPIO47
STICKS3_HAT2_AVAILABLE_PINS: GPIO5,GPIO4,GPIO6,GPIO1,GPIO7,GPIO8,GPIO43,GPIO44,GPIO2,GPIO3
STICKS3_EXT_5V_POLICY: DEFAULT_INPUT_DISABLED_FOR_OUTPUT_DO_NOT_USE_UNTIL_VERIFIED
STICKS3_M5PM1_L3B_CONTROL: M5PM1 PYG2_L3B_EN powers LCD backlight, MIC, and speaker rail
STICKS3_M5PM1_PYG2_DISTINCT_FROM_ESP32_GPIO2: YES
STICKS3_M5GFX_L3B_BOOT_VALUE: M5PM1 PYG2 output HIGH
STICKS3_M5GFX_I2C_IDLE_SLEEP_BOOT_VALUE: PM1 I2C_CFG=0x00

## Cardputer-Adv comparison facts

CARDPUTER_ADV_LCD_DRIVER: ST7789V2
CARDPUTER_ADV_LCD_RESOLUTION: 240x135
CARDPUTER_ADV_LCD_PINS: BL=GPIO38,RST=GPIO33,RS=GPIO34,DAT=GPIO35,SCK=GPIO36,CS=GPIO37
LCD_CONCLUSION: ST7789_DRIVER_REUSE_ONLY_NOT_PINMAP_REUSE
