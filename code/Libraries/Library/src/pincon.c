#include "pincon.h"

void setPinFunction(uint8 port, uint8 pin, enum PinFunction function)
{
    uint8 functionBits;
    
    switch (function)
    {
        case PinPrimaryFunction: functionBits = PINCON_PRIMARY_FUNCTION;
             break;
        case PinFirstAlternateFunction: functionBits = PINCON_FIRST_ALTERNATE_FUNCTION;
             break;
        case PinSecondAlternateFunction: functionBits = PINCON_SECOND_ALTERNATE_FUNCTION;
             break;
        case PinThirdAlternateFunction: functionBits = PINCON_THIRD_ALTERNATE_FUNCTION;
             break;
        default: break;
    }
    
    switch (port)
    {
        case 0: PINSEL_CLEAR_PORT0(pin,functionBits);
                PINSEL_SET_PORT0(pin,functionBits);
            break;
        case 1: PINSEL_CLEAR_PORT1(pin,functionBits);
                PINSEL_SET_PORT1(pin,functionBits);
            break;
        case 2: PINSEL_CLEAR_PORT2(pin,functionBits);
                PINSEL_SET_PORT2(pin,functionBits);
            break;
        case 3: PINSEL_CLEAR_PORT3(pin,functionBits);
                PINSEL_SET_PORT3(pin,functionBits);
            break;
        default: break;
    }
    
    return;
}

void setPinMode(uint8 port, uint8 pin, enum PinMode mode)
{
    uint8 functionBits;
    
    switch (mode)
    {
        case PinModePullUp: functionBits = PINCON_MODE_PULL_UP;
             break;
        case PinModeRepeater: functionBits = PINCON_MODE_REPEATER;
             break;
        case PinModeNoPullUpDown: functionBits = PINCON_MODE_NO_PULL_UP_DOWN;
             break;
        case PinModePullDown: functionBits = PINCON_MODE_PULL_DOWN;
             break;
        default: break;
    }
    
    switch (port)
    {
        case 0: PINMODE_CLEAR_PORT0(pin,functionBits);
                PINMODE_SET_PORT0(pin,functionBits);
            break;
        case 1: PINMODE_CLEAR_PORT1(pin,functionBits);
                PINMODE_SET_PORT1(pin,functionBits);
            break;
        case 2: PINMODE_CLEAR_PORT2(pin,functionBits);
                PINMODE_SET_PORT2(pin,functionBits);
            break;
        case 3: PINMODE_CLEAR_PORT3(pin,functionBits);
                PINMODE_SET_PORT3(pin,functionBits);
            break;
        default: break;
    }
    
    return;
}

void setPinOpenDrainMode(uint8 port, uint8 pin, enum PinOpenDrainMode mode)
{
    switch (port)
    {
        case 0: PINODMODE_CLEAR_PORT0(pin);
                PINODMODE_SET_PORT0(pin,mode);
            break;
        case 1: PINODMODE_CLEAR_PORT1(pin);
                PINODMODE_SET_PORT1(pin,mode);
            break;
        case 2: PINODMODE_CLEAR_PORT2(pin);
                PINODMODE_SET_PORT2(pin,mode);
            break;
        case 3: PINODMODE_CLEAR_PORT3(pin);
                PINODMODE_SET_PORT3(pin,mode);
            break;
        default: break;
    }
    
    return;
}
