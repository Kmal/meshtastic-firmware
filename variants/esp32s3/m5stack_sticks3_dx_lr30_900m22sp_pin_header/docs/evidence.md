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

## CI failure 27139716800 root cause

- The failed job was `Compile m5stack-sticks3-dx-lr30-900m22sp-pin-header / build-esp32s3` for run `27139716800`.
- The regression was introduced when the StickS3 M5PM1 battery/power-status thread was added: `runOnce()` read
  `M5PM1_PWR_SRC` and `M5PM1_GPIO_IN`, but the variant only declared the GPIO mode/output/drive/function and
  battery-voltage registers.
- The missing register constants are part of the same M5PM1 register table already used by the variant. M5PM1 power source is
  register `0x04`; GPIO input state is register `0x12`; battery voltage starts at `0x22`/`0x23`.
- This is the same class of issue as the earlier StickS3 workflow/build fixes: variant-local additions referenced build-time
  symbols that were not included or declared in that variant translation unit, so CI failed before firmware linking or hardware
  validation could run.

## CI failure 27140775395 root cause

- The failed job was again `Compile m5stack-sticks3-dx-lr30-900m22sp-pin-header / build-esp32s3` for run `27140775395`.
- The previous fix added the missing M5PM1 register constants, but it did not account for the variant init linkage model:
  `src/platform/extra_variants/m5stack_sticks3_dx_lr30_900m22sp_pin_header.cpp` already defines the strong `lateInitVariant()` used to install
  the StickS3 button handlers.
- Adding a second strong `lateInitVariant()` in the board's variant translation unit creates duplicate board-init entry points for this target, so the
  ESP32-S3 build still fails before hardware validation can run.
- The corrected ownership is: the board variant owns the M5PM1 helper (`initM5PM1PowerStatus()`), while the existing extra-variant `lateInitVariant()`
  remains the single late-init entry point and calls the M5PM1 helper before setting up the buttons.
