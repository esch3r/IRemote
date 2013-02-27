#include "led.h"

LedConfig leds[MAXLEDS];

uint8 ledCount = 0;

void Led_initialize(uint8 port, uint8 pin, bool lowActive)
{
    setGpioDirection(port, pin, GpioDirectionOutput);
    leds[ledCount].port = port;
    leds[ledCount].pin = pin;
    leds[ledCount].lowActive = lowActive;
    ledCount++;
}

void Led_set(uint8 id)
{
    if (leds[id].lowActive == FALSE)
        setGpio(leds[id].port, leds[id].pin);
    else
        clearGpio(leds[id].port, leds[id].pin);
}

void Led_clear(uint8 id)
{
     if (leds[id].lowActive == FALSE)
        clearGpio(leds[id].port, leds[id].pin);
    else
        setGpio(leds[id].port, leds[id].pin);
}

void Led_toggle(uint8 id)
{
    toggleGpio(leds[id].port, leds[id].pin);
}

bool Led_read(uint8 id)
{
    return readGpio(leds[id].port, leds[id].pin);
}

void Led_blink(uint8 id)
{
    Led_set(id);
    Timer_delayMs(LED_BLINK_TIME);
    Led_clear(id);
    
    return;
}

void Led_blink2(uint8 id)
{
    Led_set(id);
    Timer_delayMs(LED_BLINK_TIME);
    Led_clear(id);
    Timer_delayMs(LED_BLINK_TIME);
    Led_set(id);
    Timer_delayMs(LED_BLINK_TIME);
    Led_clear(id);
    
    return;
}

void Led_blink3(uint8 id)
{
    Led_set(id);
    Timer_delayMs(LED_BLINK_TIME);
    Led_clear(id);
    Timer_delayMs(LED_BLINK_TIME);
    Led_set(id);
    Timer_delayMs(LED_BLINK_TIME);
    Led_clear(id);
    Timer_delayMs(LED_BLINK_TIME);
    Led_set(id);
    Timer_delayMs(LED_BLINK_TIME);
    Led_clear(id);
    
    return;
}

void Led_clearAll()
{
    uint8 i;
    for (i = 0; i < ledCount; i++)
    {
        Led_clear(i);
    }
    
    return;
}

void Led_setAll()
{
    uint8 i;
    for (i = 0; i < ledCount; i++)
    {
        Led_set(i);
    }
    
    return;
}

void Led_allUp()
{
    uint8 i;
    
    for (i = 0; i < ledCount; i++)
    {
        Led_clearAll();
        Led_set(i);
        /* busy waiting */
        Timer_delayMs(LED_RUNNING_TIME);
    }
    
    return;
}

void Led_allDown()
{
    uint8 i;
    
    for (i = ledCount-1; i >= 0; i--)
    {
        Led_clearAll();
        Led_set(i);
        /* busy waiting */
        Timer_delayMs(LED_RUNNING_TIME);
    }
    
    return;
}
