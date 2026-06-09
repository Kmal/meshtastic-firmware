#include "configuration.h"

#ifdef M5STACK_STICKS3_DX_LR30_900M22SP_PIN_HEADER
#include "PowerStatus.h"
#include "concurrency/OSThread.h"
#include "input/ButtonThread.h"
#include "variant.h"
#include <Wire.h>

namespace
{
constexpr uint8_t M5PM1_ADDR = 0x6e;
constexpr uint8_t M5PM1_PWR_SRC = 0x04;
constexpr uint8_t M5PM1_GPIO_MODE = 0x10;
constexpr uint8_t M5PM1_GPIO_IN = 0x12;
constexpr uint8_t M5PM1_GPIO_FUNC0 = 0x16;
constexpr uint8_t M5PM1_BAT_L = 0x22;
constexpr uint8_t M5PM1_PWR_SRC_MASK = 0x07;
constexpr uint8_t M5PM1_PWR_SRC_BATTERY = 0x02;
constexpr uint8_t M5PM1_GPIO0_CHG_STAT_BIT = 1 << 0;
constexpr uint8_t M5PM1_GPIO0_CHG_STAT_FUNC_MASK = 1 << 0;
constexpr uint16_t M5PM1_MIN_BATTERY_MV = 2500;
constexpr uint16_t STICKS3_BATTERY_EMPTY_MV = 3300;
constexpr uint16_t STICKS3_BATTERY_FULL_MV = 4150;
constexpr uint16_t STICKS3_DOWN_DOUBLE_PRESS_MS = 250;
constexpr uint16_t STICKS3_DOWN_LONG_PRESS_MS = 600;
constexpr uint32_t M5PM1_POWER_UPDATE_MS = 5000;
constexpr int STICKS3_BUTTON_DEBOUNCE_MS = 8;

ButtonThread *selectButtonThread = nullptr;
ButtonThread *downButtonThread = nullptr;

bool pm1Read(uint8_t reg, uint8_t &val)
{
    Wire.beginTransmission(M5PM1_ADDR);
    Wire.write(reg);
    if (Wire.endTransmission(true) != 0)
        return false;
    if (Wire.requestFrom(M5PM1_ADDR, static_cast<uint8_t>(1)) != 1)
        return false;
    val = Wire.read();
    return true;
}

bool pm1ReadBuffer(uint8_t reg, uint8_t *buf, uint8_t len)
{
    Wire.beginTransmission(M5PM1_ADDR);
    Wire.write(reg);
    if (Wire.endTransmission(true) != 0)
        return false;
    if (Wire.requestFrom(M5PM1_ADDR, len) != len)
        return false;
    for (uint8_t i = 0; i < len; i++)
        buf[i] = Wire.read();
    return true;
}

bool pm1Write(uint8_t reg, uint8_t val)
{
    Wire.beginTransmission(M5PM1_ADDR);
    Wire.write(reg);
    Wire.write(val);
    return Wire.endTransmission() == 0;
}

bool pm1Update(uint8_t reg, uint8_t clearMask, uint8_t setMask)
{
    for (uint8_t attempt = 0; attempt < 2; attempt++) {
        uint8_t val = 0;
        if (pm1Read(reg, val) && pm1Write(reg, (val & ~clearMask) | setMask))
            return true;
    }
    return false;
}

uint16_t readBatteryVoltageMv()
{
    uint8_t buf[2] = {};
    if (!pm1ReadBuffer(M5PM1_BAT_L, buf, sizeof(buf)))
        return 0;
    return static_cast<uint16_t>(buf[0]) | (static_cast<uint16_t>(buf[1]) << 8);
}

int8_t batteryPercent(uint16_t mv)
{
    if (mv <= STICKS3_BATTERY_EMPTY_MV)
        return 0;
    if (mv >= STICKS3_BATTERY_FULL_MV)
        return 100;
    return static_cast<int8_t>(((mv - STICKS3_BATTERY_EMPTY_MV) * 100) / (STICKS3_BATTERY_FULL_MV - STICKS3_BATTERY_EMPTY_MV));
}

class M5PM1PowerStatusThread : public concurrency::OSThread
{
  public:
    M5PM1PowerStatusThread() : concurrency::OSThread("M5PM1Power")
    {
        pm1Update(M5PM1_GPIO_FUNC0, M5PM1_GPIO0_CHG_STAT_FUNC_MASK, 0);
        pm1Update(M5PM1_GPIO_MODE, M5PM1_GPIO0_CHG_STAT_BIT, 0);
    }

  protected:
    int32_t runOnce() override
    {
        uint16_t mv = readBatteryVoltageMv();
        uint8_t pwrSrc = M5PM1_PWR_SRC_BATTERY;
        uint8_t gpioIn = M5PM1_GPIO0_CHG_STAT_BIT;
        pm1Read(M5PM1_PWR_SRC, pwrSrc);
        pm1Read(M5PM1_GPIO_IN, gpioIn);

        const bool hasBattery = mv > M5PM1_MIN_BATTERY_MV;
        const bool hasUsb = (pwrSrc & M5PM1_PWR_SRC_MASK) != M5PM1_PWR_SRC_BATTERY;
        const bool isCharging = (gpioIn & M5PM1_GPIO0_CHG_STAT_BIT) == 0;
        meshtastic::PowerStatus status(
            hasBattery ? meshtastic::OptTrue : meshtastic::OptFalse, hasUsb ? meshtastic::OptTrue : meshtastic::OptFalse,
            isCharging ? meshtastic::OptTrue : meshtastic::OptFalse, hasBattery ? mv : -1, hasBattery ? batteryPercent(mv) : 0);
        powerStatus->updateStatus(&status);
        return M5PM1_POWER_UPDATE_MS;
    }
};

void wakeInputBrokerFromISR(ButtonThread *buttonThread)
{
    buttonThread->userButton.tick();
    buttonThread->setIntervalFromNow(0);
    runASAP = true;
    BaseType_t higherWake = 0;
    concurrency::mainDelay.interruptFromISR(&higherWake);
}

void IRAM_ATTR selectButtonInterrupt()
{
    wakeInputBrokerFromISR(selectButtonThread);
}

void IRAM_ATTR downButtonInterrupt()
{
    wakeInputBrokerFromISR(downButtonThread);
}
} // namespace

void initM5PM1PowerStatus()
{
    new M5PM1PowerStatusThread();
}

void lateInitVariant()
{
    initM5PM1PowerStatus();

    if (!inputBroker)
        return;

    selectButtonThread = new ButtonThread("StickS3SelectButton");
    ButtonConfig selectConfig;
    selectConfig.pinNumber = STICKS3_SELECT_BUTTON_PIN;
    selectConfig.activeLow = true;
    selectConfig.activePullup = true;
    selectConfig.intRoutine = selectButtonInterrupt;
    selectConfig.singlePress = INPUT_BROKER_SELECT;
    selectButtonThread->initButton(selectConfig);
    selectButtonThread->userButton.setDebounceMs(STICKS3_BUTTON_DEBOUNCE_MS);

    downButtonThread = new ButtonThread("StickS3DownButton");
    ButtonConfig downConfig;
    downConfig.pinNumber = STICKS3_DOWN_BUTTON_PIN;
    downConfig.activeLow = true;
    downConfig.activePullup = true;
    downConfig.intRoutine = downButtonInterrupt;
    downConfig.singlePress = INPUT_BROKER_USER_PRESS;
    downConfig.doublePress = INPUT_BROKER_ALT_PRESS;
    downConfig.longPress = INPUT_BROKER_BACK;
    downConfig.longPressTime = STICKS3_DOWN_LONG_PRESS_MS;
    downButtonThread->initButton(downConfig);
    downButtonThread->userButton.setClickMs(STICKS3_DOWN_DOUBLE_PRESS_MS);
    downButtonThread->userButton.setDebounceMs(STICKS3_BUTTON_DEBOUNCE_MS);
}
#endif
