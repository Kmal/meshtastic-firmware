# Bring-up log: M5Stack StickS3 + SZDX DX-LR30-900M22SP Pin Header / 插针款

## Phase 0 hardware inspection

Evidence observed for the DX-LR30-900M22SP Pin Header / 插针款 target from vendor manual and schematic-symbol files:

```text
DX-LR30-900M22S is pure RF hardware using SX1262 and SPI.
The DX-LR30-900M22SP Pin Header / 插针款 external port exposes VCC, GND, NSS, NRST, MOSI, SCK, DIO1, MISO, DIO2, BUSY, RXEN, and TXEN.
The DX-LR30-900M22SP Pin Header / 插针款 target has no STM32; M5Stack StickS3 controls the SX1262 over SPI.
The AD schematic symbol exposes direct pins for DIO3, RXEN, TXEN, DIO2, VCC, DIO1, BUSY, RST, MISO, MOSI, SCK, and NSS.
The vendor manual lists VCC/VBAT as 1.8-3.7 V, typical 3.3 V, and warns that 5V communication logic has module-damage risk.
```

Corrected Phase 0 decision for the DX-LR30-900M22SP Pin Header / 插针款 board hardware:

```text
DXLR30_BOARD_CLASSIFICATION: RAW_DIRECT_SPI_SX1262
DXLR30_SPI_DIRECT_CONTROL_DECISION: YES_DIRECT_SPI_AVAILABLE
DXLR30_RF_SWITCH_DECISION: EXPLICIT_RXEN_TXEN_FROM_HOST
DXLR30_IO_LEVEL_DECISION: 3V3_ONLY
DXLR30_POWER_SOURCE_FOR_BRINGUP: STICKS3_3V3_L2_HAT2_PIN_13
DXLR30_POWER_ENABLE_FOR_BRINGUP: M5PM1_PWR_CFG_LDO_EN
DXLR30_DO_NOT_USE_POWER_PINS: EXT_5V, 5V_IN, BAT
```

## Local test/build status

The repository test/build commands from the plan were not run after the operator instruction:

```text
NEVER install test env on local machine.
NEVER execute tests.
just dev
```

No dependency installation was attempted, and no tests were run during this dev-only pass.
