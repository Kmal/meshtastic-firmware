# Radio SPI validation: M5Stack StickS3 + DX-LR30-900M22SP Pin Header / 插针款

Firmware env: m5stack-sticks3-dx-lr30-900m22sp-pin-header
Commit: CURRENT_DEV_SESSION
Antenna attached: NOT_REPORTED
Power source: EXTERNAL_5V_LIMITED_SUPPLY_REQUIRED_NOT_CONFIRMED
Current limit: 150 mA pending bench confirmation
RF switch decision: EXPLICIT_RXEN_TXEN_FROM_HOST
VCC decision: CARRIER_VCC_5V_INPUT_WITH_ONBOARD_REGULATOR_BUT_IO_3V3_ONLY

Continuity check:
- NSS -> GPIO5: NOT_REPORTED
- NRST -> GPIO4: NOT_REPORTED
- SCK -> GPIO6: NOT_REPORTED
- DIO1 -> GPIO1: NOT_REPORTED
- MOSI -> GPIO7: NOT_REPORTED
- MISO -> GPIO8: NOT_REPORTED
- BUSY -> GPIO2: NOT_REPORTED
- RXEN -> GPIO43: NOT_REPORTED
- TXEN -> GPIO44: NOT_REPORTED
- GND common: NOT_REPORTED
- No GPIO exposed to 5 V unless verified tolerant: NOT_REPORTED

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
- The DX-LR30 radio carrier is still intentionally not powered from StickS3 EXT_5V by firmware. Bench validation must confirm external VCC, common GND, and the SPI/RF-switch wiring above.
