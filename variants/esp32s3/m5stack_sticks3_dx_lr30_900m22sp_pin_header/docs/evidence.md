# Evidence: M5Stack StickS3 + SZDX DX-LR30-900M22SP Pin Header / 插针款 firmware port

MESHTASTIC_FIRMWARE_URL: https://github.com/meshtastic/firmware
MESHTASTIC_NATIVE_TESTS_URL: https://github.com/meshtastic/firmware/tree/develop/test

M5STACK_STICKS3_HARDWARE_EVIDENCE: variants/esp32s3/m5stack_sticks3_dx_lr30_900m22sp_pin_header/docs/m5stack_sticks3/evidence.md
DXLR30_HARDWARE_EVIDENCE: variants/esp32s3/m5stack_sticks3_dx_lr30_900m22sp_pin_header/docs/dx_lr30_900m22sp_pin_header/evidence.md
DISPLAY_VALIDATION_DOC: variants/esp32s3/m5stack_sticks3_dx_lr30_900m22sp_pin_header/docs/display_validation.md
RADIO_SPI_VALIDATION_DOC: variants/esp32s3/m5stack_sticks3_dx_lr30_900m22sp_pin_header/docs/radio_spi_validation.md
MESH_VALIDATION_DOC: variants/esp32s3/m5stack_sticks3_dx_lr30_900m22sp_pin_header/docs/mesh_validation.md

## Firmware-port decisions

FIRMWARE_ENV: m5stack-sticks3-dx-lr30-900m22sp-pin-header
TARGET_HARDWARE_IDENTITY: DX-LR30-900M22SP Pin Header / 插针款 only
TARGET_HARDWARE_VENDOR_LABEL: DX-LR30-900M22SP Pin Header / 插针款
TARGET_HAS_STM32: NO
DISPLAY_DRIVER_DECISION: ST7789_DRIVER_REUSE_ONLY_NOT_PINMAP_REUSE
RADIO_DRIVER_DECISION: SX1262_DIRECT_SPI
RADIO_RF_SWITCH_DRIVER_DECISION: SX126X_RXEN_TXEN_HOST_CONTROL
FIRMWARE_POWER_POLICY: DO_NOT_ENABLE_STICKS3_EXT_5V_BY_DEFAULT

Do not shorten this firmware target to `DX-LR30-900M22SP`. This variant is for the
`DX-LR30-900M22SP Pin Header / 插针款` hardware identity only. The target has no
STM32; M5Stack StickS3 controls the SX1262 over SPI.
