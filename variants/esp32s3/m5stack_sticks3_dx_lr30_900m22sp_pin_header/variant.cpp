#include "configuration.h"

#include <Preferences.h>

#ifdef M5STACK_STICKS3_DX_LR30_900M22SP_PIN_HEADER
void earlyInitVariant()
{
    Preferences preferences;
    preferences.begin("meshtastic", false);
    if (!preferences.isKey("firmwareVersion"))
        preferences.putString("firmwareVersion", optstr(APP_VERSION));
    preferences.end();
}
#endif
