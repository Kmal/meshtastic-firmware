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
- The vendor manual external-port table exposes VCC, GND, NSS, NRST, MOSI, SCK, DIO1, MISO, DIO2, BUSY, RXEN, and TXEN.
- The AD schematic symbol for LR20/30 has direct pins for DIO3, RXEN, TXEN, DIO2, VCC, DIO1, BUSY, RST, MISO, MOSI, SCK, and NSS.

## Power decision

Allowed values:
- RADIO_VCC_3V3_ONLY
- CARRIER_VCC_5V_INPUT_WITH_ONBOARD_REGULATOR_AND_3V3_IO_SAFE
- CARRIER_VCC_5V_INPUT_WITH_ONBOARD_REGULATOR_BUT_IO_3V3_ONLY
- UNUSABLE_OR_UNRESOLVED_STOP

DXLR30_VCC_DECISION: CARRIER_VCC_5V_INPUT_WITH_ONBOARD_REGULATOR_BUT_IO_3V3_ONLY
DXLR30_IO_LEVEL_DECISION: 3V3_ONLY
DXLR30_POWER_SOURCE_FOR_BRINGUP: EXTERNAL_5V_LIMITED_SUPPLY

Power evidence:
- The operator-supplied pin-header board image lists the DX-LR30-900M22SP Pin Header / 插针款 working voltage as 5V.
- The vendor DX-LR30-900M22S manual lists VBAT recommended operating range as 1.8V to 3.7V, typical 3.3V.
- The vendor manual warns that 5V communication lines require series resistance and are still not recommended because of damage risk.
- The firmware variant must not enable M5Stack StickS3 external 5V output by default.

Bench limits for first power-on:
- Current limit: 150 mA pending bench confirmation
- Antenna attached before TX: YES_REQUIRED
- Default firmware may enable M5Stack StickS3 EXT_5V: NO

## RF switch decision

Allowed values:
- EXPLICIT_RXEN_TXEN_FROM_HOST
- SX1262_DIO2_RF_SWITCH_INTERNAL
- UNRESOLVED_STOP

DXLR30_RF_SWITCH_DECISION: EXPLICIT_RXEN_TXEN_FROM_HOST
DXLR30_RF_SWITCH_EVIDENCE: The vendor manual external-port table exposes RXEN as receive switch control and TXEN as transmit switch control; DIO2 is documented separately as a multi-purpose IO/RF-band switch-control pin.
DXLR30_TCXO_DECISION: XTAL_ONLY

## Proposed M5Stack StickS3 HAT2 mapping

This mapping requires 3.3V-safe GPIO signaling and an external verified radio supply. Do not power the radio from M5Stack StickS3 EXT_5V unless a later hardware validation phase proves that path safe.

| DX-LR30-900M22SP Pin Header / 插针款 signal | M5Stack StickS3 GPIO | Status |
|---|---:|---|
| NSS | GPIO5 | SELECTED |
| NRST | GPIO4 | SELECTED |
| SCK | GPIO6 | SELECTED |
| DIO1 | GPIO1 | SELECTED |
| MOSI | GPIO7 | SELECTED |
| MISO | GPIO8 | SELECTED |
| BUSY | GPIO2 | SELECTED |
| RXEN | GPIO43 | SELECTED |
| TXEN | GPIO44 | SELECTED |
| DIO2 | NC | EXPLICIT_RXEN_TXEN_MODE |
| VCC | external verified supply | SELECTED |
| GND | GND | SELECTED |

## Stop conditions

- If GPIO signaling is exposed to 5V logic, stop and add level shifting before connecting M5Stack StickS3 GPIOs.
- If `DXLR30_RF_SWITCH_DECISION` changes to `UNRESOLVED_STOP`, stop before radio TX/RX tests.
- If the display is not readable with the radio disconnected, stop before radio wiring.
- If SX1262 SPI initialization fails, stop before TX/RX validation.
