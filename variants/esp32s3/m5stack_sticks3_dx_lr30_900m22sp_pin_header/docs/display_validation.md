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

Observed:
- Backlight turns on: NOT_RUN_OPERATOR_FORBID_TESTS
- Meshtastic splash/log/status appears: NOT_RUN_OPERATOR_FORBID_TESTS
- Text readable: NOT_RUN_OPERATOR_FORBID_TESTS
- Text mirrored: NOT_RUN_OPERATOR_FORBID_TESTS
- Text upside down: NOT_RUN_OPERATOR_FORBID_TESTS
- Clipping left/right/top/bottom: NOT_RUN_OPERATOR_FORBID_TESTS
- Photo path: NOT_RUN_OPERATOR_FORBID_TESTS

Decision:
- DISPLAY_VALIDATED: NO
- DISPLAY_CHANGE_REQUIRED: UNKNOWN_UNTIL_HIL
