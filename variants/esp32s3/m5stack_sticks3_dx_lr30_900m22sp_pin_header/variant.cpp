#include "variant.h"

#include "PowerStatus.h"
#include "concurrency/OSThread.h"
#include <Preferences.h>
#include <Wire.h>

#define MESH_XSTR(s) MESH_YSTR(s)
#define MESH_YSTR(s) #s
#define MESH_OPTSTR(s) (MESH_XSTR(s)[0] ? MESH_XSTR(s) : "unset")

#ifdef M5STACK_STICKS3_DX_LR30_900M22SP_PIN_HEADER
namespace
{
constexpr uint8_t M5PM1_ADDR = 0x6e;
constexpr uint8_t M5PM1_PWR_SRC = 0x04;
constexpr uint8_t M5PM1_I2C_CFG = 0x09;
constexpr uint8_t M5PM1_GPIO_MODE = 0x10;
constexpr uint8_t M5PM1_GPIO_OUT = 0x11;
constexpr uint8_t M5PM1_GPIO_IN = 0x12;
constexpr uint8_t M5PM1_GPIO_DRV = 0x13;
constexpr uint8_t M5PM1_GPIO_FUNC0 = 0x16;
constexpr uint8_t M5PM1_BAT_L = 0x22;
constexpr uint8_t M5PM1_PWR_SRC_MASK = 0x07;
constexpr uint8_t M5PM1_PWR_SRC_BATTERY = 0x02;
constexpr uint8_t M5PM1_GPIO0_CHG_STAT_BIT = 1 << 0;
constexpr uint8_t M5PM1_PYG2_L3B_EN_BIT = 1 << 2;
constexpr uint8_t M5PM1_GPIO0_CHG_STAT_FUNC_MASK = 1 << 0;
constexpr uint8_t M5PM1_PYG2_L3B_EN_FUNC_MASK = 0b11 << 4;
constexpr uint16_t M5PM1_MIN_BATTERY_MV = 2500;
constexpr uint16_t STICKS3_BATTERY_EMPTY_MV = 3300;
constexpr uint16_t STICKS3_BATTERY_FULL_MV = 4150;
constexpr uint32_t M5PM1_POWER_UPDATE_MS = 5000;

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

void enableInternalPeripheralPower()
{
    Wire.begin(I2C_SDA, I2C_SCL);
    pm1Write(M5PM1_I2C_CFG, 0);
    pm1Update(M5PM1_GPIO_FUNC0, M5PM1_PYG2_L3B_EN_FUNC_MASK, 0);
    pm1Update(M5PM1_GPIO_MODE, 0, M5PM1_PYG2_L3B_EN_BIT);
    pm1Update(M5PM1_GPIO_DRV, M5PM1_PYG2_L3B_EN_BIT, 0);
    pm1Update(M5PM1_GPIO_OUT, 0, M5PM1_PYG2_L3B_EN_BIT);
    Wire.end();
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
        meshtastic::PowerStatus status(hasBattery ? OptTrue : OptFalse, hasUsb ? OptTrue : OptFalse,
                                       isCharging ? OptTrue : OptFalse, hasBattery ? mv : -1,
                                       hasBattery ? batteryPercent(mv) : 0);
        powerStatus->updateStatus(&status);
        return M5PM1_POWER_UPDATE_MS;
    }
};

} // namespace

void earlyInitVariant()
{
    enableInternalPeripheralPower();

    Preferences preferences;
    preferences.begin("meshtastic", false);
    if (!preferences.isKey("firmwareVersion"))
        preferences.putString("firmwareVersion", MESH_OPTSTR(APP_VERSION));
    preferences.end();
}

void lateInitVariant()
{
    new M5PM1PowerStatusThread();
}
#endif
