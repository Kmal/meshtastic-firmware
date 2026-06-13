# Evidence: SZDX DX-LR30-900M22SP Pin Header / 插针款 hardware

SZDX_DX_LR30_SPI_REFERENCE_URL: https://en.szdx-smart.com/EN/zwfa/SX1262/190.html
SEMTECH_SX1262_URL: https://www.semtech.com/products/wireless-rf/lora-connect/sx1262

## Target identity boundary

TARGET_HARDWARE_IDENTITY: DX-LR30-900M22SP Pin Header / 插针款 only
TARGET_HARDWARE_VENDOR_LABEL: DX-LR30-900M22SP Pin Header / 插针款
TARGET_HAS_STM32: NO
TARGET_CONTROL_INTERFACE: SPI_TO_SX1262

DX-LR30-900M22SP Pin Header / 插针款 has no STM32 on the target board
hardware. M5Stack StickS3 controls the SX1262 over SPI. Firmware agents must
not shorten the target name to `DX-LR30-900M22SP`.

## Tracked source documents

TRACKED_SOURCE_DOCS_STATUS: TRACKED_CURATED_SOURCE_DOCS_NO_AGENT_EXTRACTION_REQUIRED
TRACKED_SOURCE_DOCS_ROOT: variants/esp32s3/m5stack_sticks3_dx_lr30_900m22sp_pin_header/docs/dx_lr30_900m22sp_pin_header

Firmware agents must use the tracked files in this directory for normal
firmware development.

Tracked source documents:
- `DS_SX1261-2_V2_1.pdf` - SX1261/SX1262 radio datasheet.
- `DX-LR30-900M22SP_Pin_Header_vendor_manual.pdf` - vendor reference manual used for DX-LR30-900M22SP Pin Header / 插针款 pinout, SPI, BUSY/DIO, RXEN/TXEN, supply, and IO-level evidence.
- `DX-LR20&30_PCB_Footprint_AD.PcbDoc` - vendor Altium footprint.
- `DX-LR20&30_SCH_Part_AD.SchDoc` - vendor Altium schematic symbol.
- `DX-LR20&30_PCB_Footprint_PADS.pcb` - vendor PADS footprint.
- `DX-LR20&30_SCH_Part_PADS.sch` - vendor PADS schematic symbol.

## DX-LR30-900M22SP Pin Header / 插针款 classification

DXLR30_BOARD_CLASSIFICATION: RAW_DIRECT_SPI_SX1262
DXLR30_SPI_DIRECT_CONTROL_DECISION: YES_DIRECT_SPI_AVAILABLE

Evidence:
- The operator-supplied purchase image identifies the target as the DX-LR30-900M22SP Pin Header / 插针款 SX1262 board hardware.
- The vendor product page lists SX1262 and SPI as the hardware interface.
- The vendor manual says DX-LR30-900M22S is pure RF hardware that must be driven by an MCU or SPI debug tool.
- The vendor manual lists SPI communication as 0-10 Mbps, which covers Meshtastic's shared 4 MHz SX126x SPI rate.
- The vendor manual external-port table exposes VCC, GND, NSS, NRST, MOSI, SCK, DIO1, MISO, DIO2, BUSY, RXEN, and TXEN.
- The AD schematic symbol for LR20/30 has direct pins for DIO3, RXEN, TXEN, DIO2, VCC, DIO1, BUSY, RST, MISO, MOSI, SCK, and NSS.

## Power decision

Allowed values:
- RADIO_VCC_3V3_ONLY
- CARRIER_VCC_5V_INPUT_WITH_ONBOARD_REGULATOR_AND_3V3_IO_SAFE
- CARRIER_VCC_5V_INPUT_WITH_ONBOARD_REGULATOR_BUT_IO_3V3_ONLY
- UNUSABLE_OR_UNRESOLVED_STOP

DXLR30_VCC_DECISION: RADIO_VCC_3V3_ONLY
DXLR30_IO_LEVEL_DECISION: 3V3_ONLY
DXLR30_POWER_SOURCE_FOR_BRINGUP: STICKS3_3V3_L2_HAT2_PIN_13
DXLR30_POWER_ENABLE_FOR_BRINGUP: M5PM1_PWR_CFG_DCDC3V3_EN

Power evidence:
- The vendor DX-LR30-900M22S manual lists VBAT recommended operating range as 1.8V to 3.7V, typical 3.3V.
- The vendor manual lists communication level as 3.3V and warns that 5V TTL has module-damage risk.
- The StickS3 Hat2 bus exposes 3V3_L2 on pin 13 for the radio VCC connection; firmware must enable M5PM1 `DCDC_EN` / StickS3 `DCDC3V3_EN_PP` for that rail and must not enable M5Stack StickS3 external 5V output for this carrier.
- The StickS3 Hat2 EXT_5V, 5V_IN, and BAT pins are not valid DX VCC sources for this target.

Bench limits for first power-on:
- Power rail: StickS3 3V3_L2, enabled by M5PM1 PWR_CFG DCDC_EN / StickS3 DCDC3V3_EN_PP
- Antenna attached before TX: YES_REQUIRED
- Default firmware may enable M5Stack StickS3 EXT_5V: NO

## RF switch decision

Allowed values:
- EXPLICIT_RXEN_TXEN_FROM_HOST
- SX1262_DIO2_RF_SWITCH_INTERNAL
- UNRESOLVED_STOP

DXLR30_RF_SWITCH_DECISION: EXPLICIT_RXEN_TXEN_FROM_HOST
DXLR30_RF_SWITCH_EVIDENCE: The vendor manual external-port table exposes RXEN as receive switch control and TXEN as transmit switch control; DIO2 is documented separately as a multi-purpose IO/RF-band switch-control pin that may be left floating when unused.
DXLR30_TCXO_DECISION: XTAL_ONLY
DXLR30_RF_MAX_POWER_DBM: 22
DXLR30_SPI_MAX_MBPS: 10

## Proposed M5Stack StickS3 HAT2 mapping

This mapping requires 3.3V-safe GPIO signaling and powers the radio from the StickS3 3V3_L2 rail. Do not power the radio from M5Stack StickS3 EXT_5V.

| DX pin | DX-LR30-900M22SP Pin Header / 插针款 signal | StickS3 Hat2 pin | StickS3 signal | Status |
|---:|---|---:|---|---|
| 1 | VCC | 13 | 3V3_L2 | SELECTED |
| 2 | GND | 1 | GND | SELECTED |
| 3 | NSS | 2 | GPIO5 | SELECTED |
| 4 | NRST | 4 | GPIO4 | SELECTED |
| 5 | MOSI | 8 | GPIO7 | SELECTED |
| 6 | SCK | 6 | GPIO6 | SELECTED |
| 7 | DIO1 | 7 | GPIO1 | SELECTED |
| 8 | MISO | 9 | GPIO8 | SELECTED |
| 9 | DIO2 | NC | NC | EXPLICIT_RXEN_TXEN_MODE |
| 10 | BUSY | 14 | GPIO2 | SELECTED |
| 11 | RXEN | 12 | GPIO44 | SELECTED_UART_TX_ONLY_DEBUG |
| 12 | TXEN | 16 | GPIO3 | SELECTED_DEBUG_UART_FREE |

Routing rationale: debug logging only needs the ESP32-S3 UART TX line, so keep
Hat2 pin 10 (`GPIO43`) free and use Hat2 pin 12 (`GPIO44`) for DX `RXEN` instead
of the boot strap on Hat2 pin 5 (`GPIO0`). Keep DX `TXEN` on Hat2 pin 16
(`GPIO3`) so the RF-switch jumpers stay on the same end of the Hat2 connector.

## Stop conditions

- If DX VCC is connected to EXT_5V, 5V_IN, or BAT, stop and move it to StickS3 Hat2 pin 13 (`3V3_L2`).
- If GPIO signaling is exposed to 5V logic, stop and add level shifting before connecting M5Stack StickS3 GPIOs.
- If `DXLR30_RF_SWITCH_DECISION` changes to `UNRESOLVED_STOP`, stop before radio TX/RX tests.
- If the display is not readable with the radio disconnected, stop before radio wiring.
- If SX1262 SPI initialization fails, stop before TX/RX validation.
