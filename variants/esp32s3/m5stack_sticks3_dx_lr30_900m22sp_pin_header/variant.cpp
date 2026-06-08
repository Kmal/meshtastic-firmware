#include "variant.h"

#include <Preferences.h>

#define MESH_XSTR(s) MESH_YSTR(s)
#define MESH_YSTR(s) #s
#define MESH_OPTSTR(s) (MESH_XSTR(s)[0] ? MESH_XSTR(s) : "unset")

#ifdef M5STACK_STICKS3_DX_LR30_900M22SP_PIN_HEADER
void earlyInitVariant()
{
    Preferences preferences;
    preferences.begin("meshtastic", false);
    if (!preferences.isKey("firmwareVersion"))
        preferences.putString("firmwareVersion", MESH_OPTSTR(APP_VERSION));
    preferences.end();
}
#endif
