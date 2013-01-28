#include <led.h>

LedConfig leds[MAXLEDS];

uint8 ledCount = 0;

void initializeLed(uint8 port, uint8 pin, bool lowActive)
{
    setGpioDirection(port, pin, GpioDirectionOutput);
    leds[ledCount].port = port;
    leds[ledCount].pin = pin;
    leds[ledCount].lowActive = lowActive;
    ledCount++;
}

uint8 initializeLeds(void)
{
    return initLeds();
}

void setGreenLedsEnabled(bool enabled)
{
    if (enabled)
        ENABLE_GREEN();
    else
        DISABLE_GREEN();
}

void setRedLedsEnabled(bool enabled)
{
    if (enabled)
        ENABLE_RED();
    else
        DISABLE_RED();
}

void setLed(uint8 id)
{
    if (leds[id].lowActive == FALSE)
        setGpio(leds[id].port, leds[id].pin);
    else
        clearGpio(leds[id].port, leds[id].pin);
}

void clearLed(uint8 id)
{
     if (leds[id].lowActive == FALSE)
        clearGpio(leds[id].port, leds[id].pin);
    else
        setGpio(leds[id].port, leds[id].pin);
}

void toggleLed(uint8 id)
{
    toggleGpio(leds[id].port, leds[id].pin);
}

bool readLed(uint8 id)
{
    return readGpio(leds[id].port, leds[id].pin);
}

void blinkLed(uint8 id)
{
    setLed(id);
    delayMs(LED_BLINK_TIME);
    clearLed(id);
    
    return;
}

void blinkLed2(uint8 id)
{
    setLed(id);
    delayMs(LED_BLINK_TIME);
    clearLed(id);
    delayMs(LED_BLINK_TIME);
    setLed(id);
    delayMs(LED_BLINK_TIME);
    clearLed(id);
    
    return;
}

void blinkLed3(uint8 id)
{
    setLed(id);
    delayMs(LED_BLINK_TIME);
    clearLed(id);
    delayMs(LED_BLINK_TIME);
    setLed(id);
    delayMs(LED_BLINK_TIME);
    clearLed(id);
    delayMs(LED_BLINK_TIME);
    setLed(id);
    delayMs(LED_BLINK_TIME);
    clearLed(id);
    
    return;
}

void clearAllLeds()
{
    uint8 i;
    for (i = 0; i < ledCount; i++)
    {
        clearLed(i);
    }
    
    return;
}

void setAllLeds()
{
    uint8 i;
    for (i = 0; i < ledCount; i++)
    {
        setLed(i);
    }
    
    return;
}

void allLedsUp()
{
    uint8 i;
    
    for (i = 0; i < ledCount; i++)
    {
        clearAllLeds();
        setLed(i);
        /* busy waiting */
        delayMs(LED_RUNNING_TIME);
    }
    
    return;
}

void allLedsDown()
{
    uint8 i;
    
    for (i = ledCount-1; i >= 0; i--)
    {
        clearAllLeds();
        setLed(i);
        /* busy waiting */
        delayMs(LED_RUNNING_TIME);
    }
    
    return;
}
