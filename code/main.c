/**
 * This is a file 
 */

#include <led.h>
#include "irControl.h"

void testFunc();

int main(void)
{   
    initializeLeds();
    clearAllLeds();
    
    //Program started notifier
    delayMs(500);
    blinkLed3(1);
    delayMs(500);
    
    initializeUart0(115200);                    // Init the UART
    printfUart0("Welcome to IRemote!\n");    // Send a welcome message
    
    initializeIrControl();
    
    setPinMode(2,10,PinModeNoPullUpDown);       // button3
    setGpioDirection(2,10,GpioDirectionInput);
    enableGpioInterrupt(2,10,GpioInterruptRisingEdge,&testFunc);
    
    setGpioDirection(0,9,GpioDirectionOutput);   // Output pin for testing purposes
    
    for (;;) 
    {
        delayMs(10);
        processData();
     }

    return 0 ;
}



void testFunc()
{
    startPWM(1);
}
