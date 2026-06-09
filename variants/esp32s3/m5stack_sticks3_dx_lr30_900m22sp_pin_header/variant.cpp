#include "variant.h"

#include <Preferences.h>
#include <Wire.h>

#define MESH_XSTR(s) MESH_YSTR(s)
#define MESH_YSTR(s) #s
#define MESH_OPTSTR(s) (MESH_XSTR(s)[0] ? MESH_XSTR(s) : "unset")

#ifdef M5STACK_STICKS3_DX_LR30_900M22SP_PIN_HEADER
namespace
{
constexpr uint8_t M5PM1_ADDR = 0x6e;
constexpr uint8_t M5PM1_I2C_CFG = 0x09;
constexpr uint8_t M5PM1_GPIO_MODE = 0x10;
constexpr uint8_t M5PM1_GPIO_OUT = 0x11;
constexpr uint8_t M5PM1_GPIO_DRV = 0x13;
constexpr uint8_t M5PM1_GPIO_FUNC0 = 0x16;
constexpr uint8_t M5PM1_PYG2_L3B_EN_BIT = 1 << 2;
constexpr uint8_t M5PM1_PYG2_L3B_EN_FUNC_MASK = 0b11 << 4;

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

#endif
