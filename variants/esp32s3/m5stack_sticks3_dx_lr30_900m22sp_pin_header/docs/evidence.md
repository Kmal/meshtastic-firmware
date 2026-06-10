# Evidence: M5Stack StickS3 + SZDX DX-LR30-900M22SP Pin Header / 插针款 firmware port

MESHTASTIC_FIRMWARE_URL: https://github.com/meshtastic/firmware
MESHTASTIC_NATIVE_TESTS_URL: https://github.com/meshtastic/firmware/tree/develop/test

M5STACK_STICKS3_HARDWARE_EVIDENCE: variants/esp32s3/m5stack_sticks3_dx_lr30_900m22sp_pin_header/docs/m5stack_sticks3/evidence.md
DXLR30_HARDWARE_EVIDENCE: variants/esp32s3/m5stack_sticks3_dx_lr30_900m22sp_pin_header/docs/dx_lr30_900m22sp_pin_header/evidence.md
DISPLAY_VALIDATION_DOC: variants/esp32s3/m5stack_sticks3_dx_lr30_900m22sp_pin_header/docs/display_validation.md
RADIO_SPI_VALIDATION_DOC: variants/esp32s3/m5stack_sticks3_dx_lr30_900m22sp_pin_header/docs/radio_spi_validation.md
HARDWARE_SPEC_AUDIT_DOC: variants/esp32s3/m5stack_sticks3_dx_lr30_900m22sp_pin_header/docs/hardware_spec_audit.md
MESH_VALIDATION_DOC: variants/esp32s3/m5stack_sticks3_dx_lr30_900m22sp_pin_header/docs/mesh_validation.md

## Firmware-port decisions

FIRMWARE_ENV: m5stack-sticks3-dx-lr30-900m22sp-pin-header
TARGET_HARDWARE_IDENTITY: DX-LR30-900M22SP Pin Header / 插针款 only
TARGET_HARDWARE_VENDOR_LABEL: DX-LR30-900M22SP Pin Header / 插针款
TARGET_HAS_STM32: NO
DISPLAY_DRIVER_DECISION: ST7789_DRIVER_REUSE_ONLY_NOT_PINMAP_REUSE
RADIO_DRIVER_DECISION: SX1262_DIRECT_SPI
RADIO_RF_SWITCH_DRIVER_DECISION: SX126X_RXEN_TXEN_HOST_CONTROL
FIRMWARE_POWER_POLICY: RADIO_VCC_FROM_STICKS3_3V3_L2_DO_NOT_ENABLE_EXT_5V

Do not shorten this firmware target to `DX-LR30-900M22SP`. This variant is for the
`DX-LR30-900M22SP Pin Header / 插针款` hardware identity only. The target has no
STM32; M5Stack StickS3 controls the SX1262 over SPI.


## Hardware spec audit 2026-06-10

- Current code and docs were re-checked against the StickS3 product documentation, StickS3 schematic, SZDX DX-LR30 vendor manual, and Semtech SX1262 datasheet.
- Corrected invariant: DX VCC is StickS3 Hat2 pin 13 (`3V3_L2`) only, and early init must set M5PM1 `PWR_CFG` bit 1 (`DCDC_EN`) so that rail is enabled. StickS3 Hat2 `EXT_5V`, `5V_IN`, and `BAT` are not valid DX VCC sources for this target.
- Corrected invariant: all DX digital lines are 3.3 V logic. The SZDX manual explicitly warns that 5 V TTL has damage risk.
- Corrected invariant: DX Pin Header pin 9 (`DIO2`) stays unconnected in this wiring because the Pin Header exposes separate `RXEN` and `TXEN` controls.
- Corrected invariant: the module uses a 32 MHz non-TCXO crystal, so firmware must not define `SX126X_DIO3_TCXO_VOLTAGE`.
- Corrected code invariant: early init enables the M5PM1 `DCDC_EN` bit that drives StickS3 `DCDC3V3_EN_PP` for `3V3_L2`, and `SX126X_MAX_POWER` is explicitly set to 22 dBm, matching the DX-LR30-900M22S maximum transmit-power spec.

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

## CI failure 27145836464 root cause

- The failed job was again `Compile m5stack-sticks3-dx-lr30-900m22sp-pin-header / build-esp32s3` for run `27145836464`.
- The previous fix moved StickS3 M5PM1 startup into the existing extra-variant late-init hook, but the M5PM1 power-status thread still built its
  `meshtastic::PowerStatus` from unqualified `OptTrue`/`OptFalse` enum values.
- `OptionalBool`, `OptTrue`, and `OptFalse` are declared inside the `meshtastic` namespace in `src/PowerStatus.h`; unlike `src/Power.cpp`, the
  StickS3 variant translation unit does not import `using namespace meshtastic;`.
- The corrected code keeps the `PowerStatus` type and the `OptionalBool` values in the same explicit namespace (`meshtastic::PowerStatus`,
  `meshtastic::OptTrue`, `meshtastic::OptFalse`), avoiding a variant-local namespace lookup failure while preserving the M5PM1-backed battery path
  required by the StickS3 pin map.

## CI failure 27152132450 root cause

- The failed job was again `Compile m5stack-sticks3-dx-lr30-900m22sp-pin-header / build-esp32s3` for run `27152132450`.
- The StickS3 M5PM1 power-status implementation lived in the board variant translation unit and included `PowerStatus.h`.
  That header pulls in `configuration.h` and generated protobuf headers, but this variant source is compiled before PlatformIO's
  library dependency finder has selected the Nanopb include path for that translation unit.
- The build therefore failed while compiling `variants/esp32s3/m5stack_sticks3_dx_lr30_900m22sp_pin_header/variant.cpp` with
  `src/mesh/generated/meshtastic/mesh.pb.h:6:10: fatal error: pb.h: No such file or directory`.
- The corrected ownership is: `variant.cpp` remains limited to early StickS3 M5PM1 rail setup, while
  `src/platform/extra_variants/m5stack_sticks3_dx_lr30_900m22sp_pin_header.cpp` owns the late M5PM1 `PowerStatus` thread where
  normal firmware source dependencies are available.

## Operator boot log 2026-06-09 follow-up

- The reported early boot error occurs during this variant's early M5PM1 rail setup, before the shared firmware later starts
  the normal internal I2C bus on StickS3 GPIO47/GPIO48.
- StickS3 hardware evidence identifies internal I2C as GPIO47/GPIO48, M5PM1 `DCDC_EN` / StickS3 `DCDC3V3_EN_PP` as the 3V3_L2/Hat2 radio-power enable,
  and M5PM1 PYG2 as the L3B backlight/MIC/speaker enable. The variant therefore still needs an early M5PM1 transaction, but
  it now resets the Arduino `Wire` state, waits longer after `Wire.begin()`, and uses a shorter retry delay between attempts
  before handing the bus back to shared firmware initialization.
- The early variant init no longer writes the `firmwareVersion` preference. Shared ESP32 setup owns that NVS metadata later in
  boot, and keeping early init limited to M5PM1 rail setup avoids pre-populating setup/provisioning state before the default
  LoRa config can present the unset-region onboarding flow.
- This screen-equipped variant must not compile out the unset-region welcome picker. `DISABLE_WELCOME_UNSET` is undef'd in the
  variant header so a first boot with `config.lora.region == UNSET` continues to show the Meshtastic onboarding/region picker.
