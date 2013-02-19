#include "rfm12.h"
#include <gpio.h>
#include <pincon.h>

//#define RFM12_WRITEDATA             writeDataSSP0
#define RFXX_WRT_CMD(x)               putcharSsp1(0, x)
#define RFM12_GETCHAR              // getcharSSP0
#define RFM12_INIT                  initializeSsp1

int8 initializeRfm12(uint8 id, GpioPair selPair, GpioPair dataPair)
{
    //init ssp
    initializeSsp1(2.5E6, 
               SspDataSize16Bit, 
               SspFrameFormatSPI, 
               SspMasterMode, 
               SspLoopbackDisabled,
               SspSlaveOutputEnabled,
               SspLowClockOutPolarity,
               SspFirstClockOutPhase
              );
    initializeSelSsp1(id, selPair.port, selPair.pin);
    //initializeSelSsp1(1, 0, 26);
    
    setGpioDirection(dataPair.port, dataPair.pin, GpioDirectionInput );   // nFS pin must be set
    setPinMode(dataPair.port, dataPair.pin, PinModeNoPullUpDown);
    
    return 0;
}

void prepareSendingRfm12(void)
{
    RFXX_WRT_CMD(0x80D7);//EL,EF,12.0pF
    RFXX_WRT_CMD(0x8239);//!er,!ebb,ET,ES,EX,!eb,!ew,DC
    RFXX_WRT_CMD(0xA640);//A140=430.8MHz
    RFXX_WRT_CMD(0xC647);//4.8kbps
    RFXX_WRT_CMD(0x94A0);//VDI,FAST,134kHz,0dBm,-103dBm
    RFXX_WRT_CMD(0xC2AC);//AL,!ml,DIG,DQD4
    RFXX_WRT_CMD(0xCA81);//FIFO8,SYNC,!ff,DR
    RFXX_WRT_CMD(0xC483);//@PWR,NO RSTRIC,!st,!fi,OE,EN
    RFXX_WRT_CMD(0x9850);//!mp,9810=30kHz,MAX OUT
    RFXX_WRT_CMD(0xE000);//NOT USE
    RFXX_WRT_CMD(0xC800);//NOT USE
    RFXX_WRT_CMD(0xC400);//1.66MHz,2.2V
}

void prepareReceivingRfm12(void)
{
  /*  RFXX_WRT_CMD(0x80D7);//EL,EF,11.5pF
    RFXX_WRT_CMD(0x82D9);//!er,!ebb,ET,ES,EX,!eb,!ew,DC
    RFXX_WRT_CMD(0xA640);//434MHz
    RFXX_WRT_CMD(0xC647);//4.8kbps
    RFXX_WRT_CMD(0x94A0);//VDI,FAST,134kHz,0dBm,-103dBm
    RFXX_WRT_CMD(0xC2AC);//AL,!ml,DIG,DQD4
    RFXX_WRT_CMD(0xCA81);//FIFO8,SYNC,!ff,DR
    RFXX_WRT_CMD(0xC483);//@PWR,NO RSTRIC,!st,!fi,OE,EN
    RFXX_WRT_CMD(0x9850);//!mp,9810=30kHz,MAX OUT
    RFXX_WRT_CMD(0xE000);//NOT USE
    RFXX_WRT_CMD(0xC800);//NOT USE
    RFXX_WRT_CMD(0xC400);//1.66MHz,2.2V*/
    RFXX_WRT_CMD(0x8027); // disable FIFO, 868MHz(!), 12.0pF
    RFXX_WRT_CMD(0x82C9); // synth off, PLL off, enable xtal, disable clk pin, disable Batt
    RFXX_WRT_CMD(0xA620); //   (0xA620 433.92MHz)
    RFXX_WRT_CMD(0xC647); // c647 4.8Kbps (38.4: 8, 19.2: 11, 9.6: 23, 4.8: 47)
    RFXX_WRT_CMD(0x9489); // VDI,FAST,BW200kHz,-6dBm,DRSSI -97dbm
    RFXX_WRT_CMD(0xC220); // datafiltercommand ; ** not documented command **
    RFXX_WRT_CMD(0xCA00); // FiFo and resetmode command ; FIFO fill disabeld
    RFXX_WRT_CMD(0xC4C3); // enable AFC ;enable frequency offset
    RFXX_WRT_CMD(0xCC67); //
    RFXX_WRT_CMD(0xC000); // clock output 1.00MHz, can be used to see if SPI works
    RFXX_WRT_CMD(0xE000); // disable wakeuptimer
    RFXX_WRT_CMD(0xC800); // disable low duty cycle
}
