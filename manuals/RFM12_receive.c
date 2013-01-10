/********************************************************************
Beispiel für RFM12-Funkmodul (Empfangen)
IAR Embedded Workbench IDE, C
Prozessor: ATMEGA8
Frequenz: 434MHz
Datenrate: 4.8kbps

Diese Programm stellt eine kurze Testroutine für den ATMEGA8 in
Verbindung mit einem externen 8MHz Quarz, einem RFM12-Funkmodul, und
dem Pollin Electronic Funk-AVR-Evaluations-Board dar.
Es handelt sich ausdrücklich um eine Testroutine und ein Anwendungs-
beispiel. Elementare Elemente wie Fehlerbehandlungen usw. sind nicht
vorhanden.

Ablauf:
Wartet auf nIRQ des Funkmoduls, liest anschließend die empfangenen Daten
aus, bildet aus 16 Datenbyte die ChkSumme und vergleicht diese mit dem 17.
empfangenen Datenbyte. Erfolg wird mit LED2 signalisiert.
Dies geschieht in einer Endlosschleife.

Pinbelegung
-----------------------
ATMEGA8           RFM12
-----------------------
SCK               SCK
MISO              SDO
MOSI              SDI
SS                nSEL
INT0              nIRQ
PD6               LED1
PD5               LED2
PD4               Module-Power

PD7               FSK
PD3               DCLK,FFIT
PB0               NINT, VDI
**********************************************************************/

#include <iom8.h>
#define DDR_IN 0
#define DDR_OUT 1
#define PORT_SEL PORTB
#define PIN_SEL PINB
#define DDR_SEL DDRB
#define PORT_SDI PORTB
#define PIN_SDI PINB
#define DDR_SDI DDRB
#define PORT_SCK PORTB
#define PIN_SCK PINB
#define DDR_SCK DDRB
#define PORT_SDO PORTB
#define PIN_SDO PINB
#define DDR_SDO DDRB
#define PORT_IRQ PORTD
#define PIN_IRQ PIND
#define DDR_IRQ DDRD
#define PORT_DATA PORTD
#define PIN_DATA PIND
#define DDR_DATA DDRD
#define PB7 7//--\
#define PB6 6// |
#define RFXX_SCK 5// |
#define RFXX_SDO 4// |RF_PORT
#define RFXX_SDI 3// |
#define RFXX_SEL 2// |
#define NC 1// |
#define PB0 0//--/
#define SEL_OUTPUT() DDR_SEL |= (1<<RFXX_SEL)
#define HI_SEL() PORT_SEL|= (1<<RFXX_SEL)
#define LOW_SEL() PORT_SEL&=~(1<<RFXX_SEL)
#define SDI_OUTPUT() DDR_SDI |= (1<<RFXX_SDI)
#define HI_SDI() PORT_SDI|= (1<<RFXX_SDI)
#define LOW_SDI() PORT_SDI&=~(1<<RFXX_SDI)
#define SDO_INPUT() DDR_SDO&= ~(1<<RFXX_SDO)
#define LOW_SDO() PORT_SDO&= (1<<RFXX_SDO)
#define SDO_HI() PIN_SDO&(1<<RFXX_SDO)
#define SCK_OUTPUT() DDR_SCK |= (1<<RFXX_SCK)
#define HI_SCK() PORT_SCK|= (1<<RFXX_SCK)
#define LOW_SCK() PORT_SCK&=~(1<<RFXX_SCK)
#define RF12_IRQ 2
#define IRQ_IN() DDR_IRQ &=~(1<<RF12_IRQ)
#define WAIT_IRQ_LOW() while(PIND&(1<<RF12_IRQ))
#define RF12_DATA 7//PD7
#define DATA_OUT() DDR_DATA|=1<<RF12_DATA
#define HI_DATA() PORT_DATA|=1<<RF12_DATA
#define LED2_OUTPUT() DDRD|=((1<<5)|(1<<4))//+Module-Power
#define LED2_OFF() PORTD&=~(1<<5)
#define LED2_ON() PORTD|= (1<<5)
#define MODULE_ON() PORTD|=(1<<4)
#define MODULE_OFF() PORTD&=~(1<<4)

void RFXX_PORT_INIT(void){
  HI_SEL();
  HI_SDI();
  LOW_SCK();
  //SET nFFS pin HI when using FIFO
  HI_DATA();
  SEL_OUTPUT();
  SDI_OUTPUT();
  SDO_INPUT();
  SCK_OUTPUT();
  IRQ_IN();
  DATA_OUT();
}

unsigned int RFXX_WRT_CMD(unsigned int aCmd){
  unsigned char i;
  unsigned int temp;
  temp=0;
  LOW_SCK();
  LOW_SEL();
  for(i=0;i<16;i++){
    if(aCmd&0x8000){
      HI_SDI();
      }else{
      LOW_SDI();
    }
    HI_SCK();
    temp<<=1;
    if(SDO_HI()){
      temp|=0x0001;
    }
    LOW_SCK();
    aCmd<<=1;
  };
  HI_SEL();
  return(temp);
}

void RF12_INIT(void){
  RFXX_WRT_CMD(0x80D7);//EL,EF,11.5pF
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
  RFXX_WRT_CMD(0xC400);//1.66MHz,2.2V
}

unsigned char RF12_RECV(void){
  unsigned int FIFO_data;
  WAIT_IRQ_LOW();
  RFXX_WRT_CMD(0x0000);
  FIFO_data=RFXX_WRT_CMD(0xB000);
  return(FIFO_data&0x00FF);
}

void Delay_ms(unsigned char amS){
  unsigned char i;
  unsigned int j;
  for(i=0;i<amS;i++)for(j=0;j<914;j++);
}

int main(void)
{
  unsigned char i;
  unsigned char ChkSum;
  //POWER ON indication: LED blink 3 times
  LED2_OUTPUT();
  LED2_OFF();
  MODULE_OFF(); //for reset
  for(i=0;i<3;i++){
    Delay_ms(200);
    LED2_ON();
    Delay_ms(200);
    LED2_OFF();
  }
  LED2_OFF();
  MODULE_ON(); //for reset
  Delay_ms(200);

  //Initialize command port
  RFXX_PORT_INIT();
  //Initialize RF12 chip
  RF12_INIT();
  //Init FIFO
  RFXX_WRT_CMD(0xCA81);
  while(1){
    //Enable FIFO
    RFXX_WRT_CMD(0xCA83);
    ChkSum=0;
    //Receive payload data
    for(i=0;i<16;i++){
      ChkSum+=RF12_RECV();
    }
    //Receive Check sum
    i=RF12_RECV();
    //Disable FIFO
    RFXX_WRT_CMD(0xCA81);
    //Package chkeck
    if(ChkSum==i){
      LED2_ON();
      Delay_ms(200);
      LED2_OFF();
    }
  }
}