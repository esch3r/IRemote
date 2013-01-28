include "sspi.h"
int8 initializeSPI(uint32 freq,float duty, uint8 ch)
{
    SSPI_ENABLE_POWER(1);
    SSPI_SET_CORE_CLK();
    
   // PWM_PR(0);                      // set Prescaler to 0

    switch (ch)
    {
        case 0: SSPI0_SET_CORE_CLK();break;  //Enable PWM1.1
               
        case 1: SSPI1_SET_CORE_CLK();  //Enable PWM1.2
                SSPI_SET_SSEL1;
                SSPI_SET_SCK1;
                SSPI_SET_MISO1;
                SSPI_SET_MOSI1;break;
        default: break;
    }
   SPI_ENABLE_IRQ();
   
   return 0;
}
