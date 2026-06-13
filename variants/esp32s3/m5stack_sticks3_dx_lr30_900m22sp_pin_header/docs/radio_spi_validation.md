# Radio SPI validation: M5Stack StickS3 + DX-LR30-900M22SP Pin Header / 插针款

Firmware env: m5stack-sticks3-dx-lr30-900m22sp-pin-header
Commit: CURRENT_DEV_SESSION
Antenna attached: NOT_REPORTED
Power source: STICKS3_3V3_L2_HAT2_PIN_13
Power rail: StickS3 3V3_L2, enabled by M5PM1 PWR_CFG DCDC_EN / StickS3 DCDC3V3_EN_PP; do not use EXT_5V, 5V_IN, or BAT
RF switch decision: EXPLICIT_RXEN_TXEN_FROM_HOST
VCC decision: RADIO_VCC_3V3_ONLY

Continuity check:
- DX pin 1 VCC -> StickS3 Hat2 pin 13 3V3_L2 after M5PM1 DCDC_EN / StickS3 DCDC3V3_EN_PP: NOT_REPORTED
- DX pin 2 GND -> StickS3 Hat2 pin 1 GND: NOT_REPORTED
- DX pin 3 NSS -> StickS3 Hat2 pin 2 GPIO5: NOT_REPORTED
- DX pin 4 NRST -> StickS3 Hat2 pin 4 GPIO4: NOT_REPORTED
- DX pin 5 MOSI -> StickS3 Hat2 pin 8 GPIO7: NOT_REPORTED
- DX pin 6 SCK -> StickS3 Hat2 pin 6 GPIO6: NOT_REPORTED
- DX pin 7 DIO1 -> StickS3 Hat2 pin 7 GPIO1: NOT_REPORTED
- DX pin 8 MISO -> StickS3 Hat2 pin 9 GPIO8: NOT_REPORTED
- DX pin 9 DIO2 -> NC: NOT_REPORTED
- DX pin 10 BUSY -> StickS3 Hat2 pin 14 GPIO2: NOT_REPORTED
- DX pin 11 RXEN -> StickS3 Hat2 pin 12 GPIO44: NOT_REPORTED
- DX pin 12 TXEN -> StickS3 Hat2 pin 16 GPIO3: NOT_REPORTED
- No DX VCC connection to EXT_5V, 5V_IN, or BAT: NOT_REPORTED
- No GPIO exposed to 5 V logic: NOT_REPORTED

Observed boot log from operator on 2026-06-09:

```text
ESP-ROM:esp32s3-20210327
Build:Mar 27 2021
rst:0x1 (POWERON),boot:0x8 (SPI_FAST_FLASH_BOOT)
SPIWP:0xee
mode:DIO, clock div:1
load:0x3fce2820,len:0x10cc
load:0x403c8700,len:0xc2c
load:0x403cb700,len:0x30c0
entry 0x403c88b8
[     2][I][esp32-hal-psram.c:106] psramAddToHeap(): PSRAM added to the heap.
[    15][I][esp32-hal-i2c-ng.c:112] i2cInit(): Initializing I2C Master: num=0 sda=47 scl=48 freq=100000
[  5138][I][esp32-hal-i2c-ng.c:112] i2cInit(): Initializing I2C Master: num=0 sda=47 scl=48 freq=100000
[  7102][I][esp32-hal-ledc.c:288] ledcAttachChannel(): LEDC attached to pin 38 (channel 0, resolution 8)
[ 20353][I][BLEDevice.cpp:293] init(): Initializing BLE stack: NimBLE
[ 20430][I][BLEDevice.cpp:1497] host_task(): NimBLE host task started
[ 20672][I][BLEDevice.cpp:997] getAdvertising(): create advertising
```

Observed display fault from operator on 2026-06-09:

```text
Critical fault #3
No LoRa radio
```

Result:
- SX1262 init result 0 or equivalent success: NOT_OBSERVED
- No SX1262 init -2: UNKNOWN_MESHTASTIC_RADIO_LOG_MISSING
- No BUSY timeout: UNKNOWN_MESHTASTIC_RADIO_LOG_MISSING
- No brownout: NOT_OBSERVED_IN_OPERATOR_LOG
- RADIO_SPI_VALIDATED: NO

Current firmware-side mitigations:
- Critical fault #3 now shows the last radio-init detail when one is available, such as an SX126x RadioLib init result or a region-skip reason.
- Saved `LORA_24` config is rejected for builds without a 2.4 GHz-capable radio and clamped back to `UNSET` during boot so SX1262 probing is not skipped.
- The DX-LR30 radio carrier VCC is wired to StickS3 Hat2 pin 13 (`3V3_L2`), and early init now sets M5PM1 `PWR_CFG` bit 1 (`DCDC_EN`) / StickS3 `DCDC3V3_EN_PP` for that rail. Do not use StickS3 EXT_5V, 5V_IN, or BAT. Bench validation must confirm the pin-numbered continuity table above.
- Firmware explicitly caps SX1262 output at 22 dBm and leaves DIO3 TCXO control undefined because the DX manual specifies +22 dBm maximum power and a non-TCXO crystal.
