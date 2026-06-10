# Display validation: M5Stack StickS3 + Meshtastic

Firmware env: m5stack-sticks3-dx-lr30-900m22sp-pin-header
Commit: NOT_RUN_DEV_ONLY
Radio connected: NO
Power: NOT_RUN_DEV_ONLY

Expected from variant:
- ST7789_NSS=GPIO41
- ST7789_RS=GPIO45
- ST7789_SDA=GPIO39
- ST7789_SCK=GPIO40
- ST7789_RESET=GPIO21
- VTFT_LEDA=GPIO38
- TFT_WIDTH=240
- TFT_HEIGHT=135
- M5PM1 PWR_CFG register 0x06 bit 2 LDO_EN is set during early init for Hat2 3V3_L2
- M5PM1 I2C_CFG register 0x09 is cleared during early init
- M5PM1 PYG2_L3B_EN is set to GPIO output push-pull high during early init

Observed:
- Backlight turns on: NOT_RUN_NEEDS_OPERATOR_RETEST_AFTER_PM1_GPIO2_HIGH_FIX
- Meshtastic splash/log/status appears: NOT_RUN_OPERATOR_FORBID_TESTS
- Text readable: NOT_RUN_OPERATOR_FORBID_TESTS
- Text mirrored: NOT_RUN_OPERATOR_FORBID_TESTS
- Text upside down: NOT_RUN_OPERATOR_FORBID_TESTS
- Clipping left/right/top/bottom: NOT_RUN_OPERATOR_FORBID_TESTS
- Photo path: NOT_RUN_OPERATOR_FORBID_TESTS

Decision:
- DISPLAY_VALIDATED: NO
- DISPLAY_CHANGE_REQUIRED: RETEST_AFTER_PM1_GPIO2_HIGH_FIX
