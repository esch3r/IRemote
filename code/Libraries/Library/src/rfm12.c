#include <rfm12.h>

#define RFM12_WRITEDATA             writeDataUart1
#define RFXX_WRT_CMD                putcharUart1
#define RFM12_GETCHAR               getcharUart1
#define RFM12_INIT                  initializeSSP0

int8 initializeRfm12(void)
{
    //init ssp
    RFM12_INIT(115200, SspDataSize16Bit, SspFrameFormatSPI);
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
    
}
