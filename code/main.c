/**
 * This is a file 
 */

#include <led.h>
#include <iap.h>
#include "irControl.h"

void testFunc();

int main(void)
{   
    uint32 testVar;
    
    initializeLeds();
    clearAllLeds();
    
    //Program started notifier
    delayMs(500);
    blinkLed3(1);
    delayMs(500);
    
    initializeUart0(115200);                    // Init the UART
    printfUart0("Welcome to IRemote!\n");    // Send a welcome message
    printfUart0("Id: %i, Version: %i, Serial: %i\n",readIdIap(),readVersionIap(),readSerialIap());
    //printfUart0("Sector 26: %i, Sector 27: %i\n",checkBlankIap(26),checkBlankIap(27));
    testVar = 0;
    __disable_irq();
    //eraseIap(27);
    //writeIap(27,256*64,(void*)(&testVar),sizeof(testVar));
    readIap(27,256*64,(void*)(&testVar),sizeof(testVar));
    printfUart0("Test: %u\n",testVar);
    __enable_irq();
    
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
