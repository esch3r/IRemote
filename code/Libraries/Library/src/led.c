#include <led.h>
#include <timer.h>

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
    SET_LED(id);
}

void clearLed(uint8 id)
{
    CLEAR_LED(id);
}

void toggleLed(uint8 id)
{
    if (READ_LED(id))
        CLEAR_LED(id);
    else
        SET_LED(id);
}

bool readLed(uint8 id)
{
    return READ_LED(id);
}

void blinkLed(uint8 id)
{
    SET_LED(id);
    delayMs(LED_BLINK_TIME);
    CLEAR_LED(id);
    
    return;
}

void blinkLed2(uint8 id)
{
    SET_LED(id);
    delayMs(LED_BLINK_TIME);
    CLEAR_LED(id);
    delayMs(LED_BLINK_TIME);
    SET_LED(id);
    delayMs(LED_BLINK_TIME);
    CLEAR_LED(id);
    
    return;
}

void blinkLed3(uint8 id)
{
    SET_LED(id);
    delayMs(LED_BLINK_TIME);
    CLEAR_LED(id);
    delayMs(LED_BLINK_TIME);
    SET_LED(id);
    delayMs(LED_BLINK_TIME);
    CLEAR_LED(id);
    delayMs(LED_BLINK_TIME);
    SET_LED(id);
    delayMs(LED_BLINK_TIME);
    CLEAR_LED(id);
    
    return;
}

void clearAllLeds()
{
    uint8 i;
    for (i = 1; i <= LEDCOUNT; i++)
    {
        CLEAR_LED(i);
    }
    
    return;
}

void setAllLeds()
{
    uint8 i;
    for (i = 1; i <= LEDCOUNT; i++)
    {    clearAllLeds();
        SET_LED(i);
    }
    
    return;
}

void allLedsUp()
{
    uint8 i;
    
    for (i = 1; i <= LEDCOUNT; i++)
    {
        clearAllLeds();
        SET_LED(i);
        /* busy waiting */
        delayMs(LED_RUNNING_TIME);
    }
    
    return;
}

void allLedsDown()
{
    uint8 i;
    
    for (i = LEDCOUNT; i > 0; i--)
    {
        clearAllLeds();
        SET_LED(i);
        /* busy waiting */
        delayMs(LED_RUNNING_TIME);
    }
    
    return;
}
