    #include "dac.h"

void initializeDac()
{
    DAC_SET_CLK();  // Set the clock the dac counter runs with
    DAC_SET_PIN();  // Set PIN to AOUT
    
    DAC_SET_COUNT_VALUE(255);                  // Set count value to 255, timer counts up to 255 and then reloads the value
    DAC_ENABLE_DOUBLE_BUFFER_AND_TIMEOUT();    // Enable double buffering and timout
    return;
}

void setDacValue(uint16 value)
{
    DAC_SET_VALUE(value);    // Set DAC value
    return;
}

void setDacVolt(uint16 mV)
{
    setDacValue((uint16)((mV-VREFN)/(VREFP-VREFN))*DAC_STEPS);
    return;
}
