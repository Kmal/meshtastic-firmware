#include "configuration.h"

#ifdef M5STACK_STICKS3_DX_LR30_900M22SP_PIN_HEADER
#include "input/ButtonThread.h"
#include "variant.h"

namespace
{
constexpr uint16_t STICKS3_DOWN_DOUBLE_PRESS_MS = 250;
constexpr uint16_t STICKS3_DOWN_LONG_PRESS_MS = 600;
constexpr int STICKS3_BUTTON_DEBOUNCE_MS = 8;

ButtonThread *selectButtonThread = nullptr;
ButtonThread *downButtonThread = nullptr;

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
