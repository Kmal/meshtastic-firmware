# Mesh validation: M5Stack StickS3 + DX-LR30-900M22SP Pin Header / 插针款

Node A:
- Hardware: M5Stack StickS3 + DX-LR30-900M22SP Pin Header / 插针款
- Hardware identity boundary: DX-LR30-900M22SP Pin Header / 插针款 only; no STM32, SX1262 controlled over SPI
- Firmware env: m5stack-sticks3-dx-lr30-900m22sp-pin-header
- Commit: NOT_RUN_DEV_ONLY
- Region: NOT_RUN_OPERATOR_FORBID_TESTS
- RF switch decision: EXPLICIT_RXEN_TXEN_FROM_HOST
- Power source: EXTERNAL_5V_LIMITED_SUPPLY_PENDING_BENCH

Node B:
- Hardware: NOT_RUN_OPERATOR_FORBID_TESTS
- Firmware version: NOT_RUN_OPERATOR_FORBID_TESTS
- Region: NOT_RUN_OPERATOR_FORBID_TESTS

Preflight:
- Antenna attached Node A: NOT_RUN_OPERATOR_FORBID_TESTS
- Antenna attached Node B: NOT_RUN_OPERATOR_FORBID_TESTS
- Same LoRa region/channel: NOT_RUN_OPERATOR_FORBID_TESTS
- Same modem preset/channel settings: NOT_RUN_OPERATOR_FORBID_TESTS

Test A -> B:
- Message: NOT_RUN_OPERATOR_FORBID_TESTS
- Received by Node B: NOT_RUN_OPERATOR_FORBID_TESTS
- RSSI/SNR: NOT_RUN_OPERATOR_FORBID_TESTS

Test B -> A:
- Message: NOT_RUN_OPERATOR_FORBID_TESTS
- Received by Node A: NOT_RUN_OPERATOR_FORBID_TESTS
- RSSI/SNR: NOT_RUN_OPERATOR_FORBID_TESTS

Result:
- BIDIRECTIONAL_MESH_VALIDATED: NO
- Notes: HIL validation was not run because the operator requested dev-only work and forbade test execution.
