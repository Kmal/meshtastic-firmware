# Hardware spec audit: M5Stack StickS3 + DX-LR30-900M22SP Pin Header / 插针款

This audit records the hardware-spec constraints used by the variant. Treat
older comments or bring-up notes as stale when they conflict with this file.

## Sources checked

- M5Stack StickS3 product documentation: `https://docs.m5stack.com/en/core/StickS3`
- M5Stack StickS3 schematic PDF:
  `https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/1207/K150_Stick_S3_PRJ_V0.6_20251111_2025_11_17_16_10_24.pdf`
- M5Stack M5PM1 source/register definitions: `https://github.com/m5stack/M5PM1`
- SZDX DX-LR30-900M22SP Pin Header / 插针款 vendor manual:
  `docs/dx_lr30_900m22sp_pin_header/DX-LR30-900M22SP_Pin_Header_vendor_manual.pdf`
- Semtech SX1261/SX1262 datasheet:
  `docs/dx_lr30_900m22sp_pin_header/DS_SX1261-2_V2_1.pdf`

## Corrected decisions

| Area | Decision | Hardware-spec basis |
| --- | --- | --- |
| Radio board identity | Raw direct-SPI SX1262 board; no STM32 host on this target | SZDX manual describes DX-LR30-900M22S as pure RF hardware driven by an MCU or SPI debug tool. |
| Radio VCC | StickS3 `3V3_L2`, not `EXT_5V`, not `5V_IN`, not `BAT` | SZDX manual lists VCC/VBAT recommended range 1.8-3.7 V, typical 3.3 V; StickS3 Hat2 exposes `3V3_L2` on pin 13. |
| 3V3_L2 firmware enable | Set M5PM1 `PWR_CFG` bit 1 (`DCDC_EN`) during early init | StickS3 schematic ties `3V3_L2_EN` to the PMIC `DCDC3V3_EN_PP` output; M5PM1 register docs define `PWR_CFG` bit 1 as `DCDC_EN`. |
| Logic level | 3.3 V logic only | SZDX manual lists communication level as 3.3 V and warns 5 V TTL can burn the module. |
| RF switch control | Host-controlled `RXEN`/`TXEN`; leave DX `DIO2` unconnected | SZDX Pin Header external-port table exposes separate `RXEN` and `TXEN`; `DIO2` is optional RF-switch control and may be left floating when unused. |
| TCXO | No `SX126X_DIO3_TCXO_VOLTAGE` | SZDX manual says the module uses a non-TCXO crystal; Semtech DIO3 TCXO control is not applicable. |
| RF output limit | `SX126X_MAX_POWER=22` dBm | SZDX manual lists max transmit power as +22 dBm. |
| SPI rate | Meshtastic shared SX126x SPI rate, 4 MHz | SZDX manual allows SPI 0-10 Mbps, so the shared 4 MHz rate is inside spec. |

## Final wiring table

| DX-LR30 Pin Header / 插针款 pin | DX signal | StickS3 Hat2 pin | StickS3 signal | Firmware define |
| ---: | --- | ---: | --- | --- |
| 1 | VCC | 13 | `3V3_L2` | hardware rail |
| 2 | GND | 1 | `GND` | hardware rail |
| 3 | NSS | 2 | `GPIO5` | `LORA_CS` / `SX126X_CS` |
| 4 | NRST | 4 | `GPIO4` | `LORA_RESET` / `SX126X_RESET` |
| 5 | MOSI | 8 | `GPIO7` | `LORA_MOSI` |
| 6 | SCK | 6 | `GPIO6` | `LORA_SCK` |
| 7 | DIO1 | 7 | `GPIO1` | `LORA_DIO1` / `SX126X_DIO1` |
| 8 | MISO | 9 | `GPIO8` | `LORA_MISO` |
| 9 | DIO2 | NC | NC | `LORA_DIO2=RADIOLIB_NC` |
| 10 | BUSY | 14 | `GPIO2` | `LORA_BUSY` / `SX126X_BUSY` |
| 11 | RXEN | 12 | `GPIO44` | `SX126X_RXEN` |
| 12 | TXEN | 16 | `GPIO3` | `SX126X_TXEN` |

Jumper routing note: use DX pin 11 (`RXEN`) to Hat2 pin 12 (`GPIO44`) and DX
pin 12 (`TXEN`) to Hat2 pin 16 (`GPIO3`). This avoids using the Hat2 boot strap
pin 5 (`GPIO0`) and keeps Hat2 pin 10 (`GPIO43`) free for debug UART TX output.

Do not use StickS3 Hat2 pin 3 (`EXT_5V`), pin 11 (`BAT`), or pin 15 (`5V_IN`)
for DX VCC. Hat2 pin 5 (`Boot` / `GPIO0`) is not used for radio wiring. GPIO43
remains available for ESP32-S3 debug UART TX output; debug UART RX on GPIO44 is
not needed for log capture.

## Validation checklist before RF transmit

- Antenna attached to the DX-LR30 RF connector.
- DX VCC measured at 3.3 V on StickS3 `3V3_L2` with common GND after early init sets M5PM1 `DCDC_EN` / StickS3 `DCDC3V3_EN_PP`.
- No DX signal pin is driven by 5 V logic.
- Continuity matches the wiring table above by DX pin number and StickS3 Hat2 pin number.
- `EXT_5V` output remains disabled; powering the radio from `EXT_5V` is out of spec.
