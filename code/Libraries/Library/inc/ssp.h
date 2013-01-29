/**
 * @file ssp.h
 * @author Johannes Wimmer & Alexander Rössler
 * @brief PWM Library
 * @date 16-10-2012
 */

#ifndef _PWM_DRIVER_H_
#define _PWM_DRIVER_H_

#include <types.h>
#include <LPC17xx.h>
#include <circularbuffer.h>

/** Size of circular buffer for reading */
#ifndef SSP0_READ_BUFFER_SIZE
#define SSP1_READ_BUFFER_SIZE       100
#endif
/** Size of circular buffer for writing */
#ifndef SSP1_WRITE_BUFFER_SIZE
#define SSP1_WRITE_BUFFER_SIZE      100
#endif
/** Defines wheter the buffer should be used or not */
#ifndef USE_SSP_BUFFER
#define USE_SSP_BUFFER 1
#endif

#define SSP_RXIM     (1 << 2)    //Interrupt when RxFifo half full
#define SSP_TXIM    (1 << 3)     //Interrupt Enable TxFifo half empty
#define SYSTEM_CLK      SystemCoreClock
#define SSP_CLK         SYSTEM_CLK/4

#define SSP0_SET_PR(x)       LPC_SSP0->CPSR = (uint8)(SSP_CLK/x)-1
#define SSP1_SET_PR(x)       LPC_SSP1->CPSR = (uint8)(SSP_CLK/x)-1
#define PCONP_SSP(ch)     (1 << (8+2*ch)) 		//set clock control bit
#define SSP0_SET_CORE_CLK()  LPC_SC->PCLKSEL0 |= (0b00<< 10); //set 11:10 for SSPI
#define SSP1_SET_CORE_CLK()  LPC_SC->PCLKSEL1 |= (0b00<< 20); //set 16:17 for SSPI


#define SSP_ENABLE_POWER(ch)  LPC_SC->PCONP |= PCONP_SSP(ch)             //enable PCONP for SSPI
#define SSP_DISABLE_POWER(ch) LPC_SC->PCONP &= ~PCONP_SSPI(ch)              //set PCLKSEL for SSPI
#define SSP0_SET_DATA_SIZE(x)     LPC_SSP0->CR0 |= (x << 0)
#define SSP0_SET_FRAME_FORMAT(x)  LPC_SSP0->CR0 |= (x << 4)
#define SSP0_ENABLE_SSP           LPC_SSP0->CR1 |= (1 << 1)
#define SSP1_SET_DATA_SIZE(x)     LPC_SSP1->CR0 |= (x << 0)
#define SSP1_SET_FRAME_FORMAT(x)  LPC_SSP1->CR0 |= (x << 4)
#define SSP1_ENABLE_SSP           LPC_SSP1->CR1 |= (1 << 1)

#define SSP0_SET_SSEL         LPC_PINCON->PINSEL3  |= (0b11 << 10); LPC_PINCON->PINMODE3 |= (0b11 << 10)  // P1.21 pull-down Resistor
#define SSP0_SET_SCK          LPC_PINCON->PINSEL3  |= (0b11 << 8); LPC_PINCON->PINMODE3 |= (0b11 << 8)  // P1.20 pull-down Resistor
#define SSP0_SET_MISO         LPC_PINCON->PINSEL3  |= (0b11 << 14); LPC_PINCON->PINMODE3 |= (0b11 << 14)  // P1.23 pull-down Resistor
#define SSP0_SET_MOSI         LPC_PINCON->PINSEL3  |= (0b11 << 16); LPC_PINCON->PINMODE3 |= (0b11 << 16)  // P1.24 pull-down Resistor
#define SSP1_SET_SSEL         LPC_PINCON->PINSEL0  |= (0b10 << 12); LPC_PINCON->PINMODE0 |= (0b11 << 12)  // P0.6 pull-down Resistor
#define SSP1_SET_SCK          LPC_PINCON->PINSEL0  |= (0b10 << 14); LPC_PINCON->PINMODE0 |= (0b11 << 14)  // P0.7 pull-down Resistor
#define SSP1_SET_MISO         LPC_PINCON->PINSEL0  |= (0b10 << 16); LPC_PINCON->PINMODE0 |= (0b11 << 16)  // P0.8 pull-down Resistor
#define SSP1_SET_MOSI         LPC_PINCON->PINSEL0  |= (0b10 << 18); LPC_PINCON->PINMODE0 |= (0b11 << 18)  // P0.9 pull-down Resistor
#define SSP0_ENABLE_IRQ()     NVIC_EnableIRQ(SSP0_IRQn)
#define SSP0_SET_IRQS()       NVIC_SetPendingIRQ(SSP0_IRQn)
#define SSP1_ENABLE_IRQ()      NVIC_EnableIRQ(SSP1_IRQn)
#define SSP1_SET_IRQS()       NVIC_SetPendingIRQ(SSP1_IRQn)
#define SSP0_SET_RXIM_AND_TXIM_INTERRUPT()   LPC_SSP0->IMSC = SSP_RXIM | SSP_TXIM
#define SSP1_SET_RXIM_AND_TXIM_INTERRUPT()   LPC_SSP1->IMSC = SSP_RXIM | SSP_TXIM

#define SSP1_READ_CHAR()           LPC_SSP1->DR
#define SSP1_SET_CHAR(c)            LPC_SSP1->DR = (c << 0)
#define SSP1_FIFO_TRANSMIT_EMPTY    (LPC_SSP1->SR &  (1 << 0))
#define SSP1_FIFO_RECEIVE_EMPTY    (LPC_SSP1->SR &  (0 << 3))

typedef enum
{
  SspDataSize4Bit =  0b0011,
  SspDataSize5Bit =  0b0100,
  SspDataSize6Bit =  0b0101,
  SspDataSize7Bit =  0b0110,
  SspDataSize8Bit =  0b0111,
  SspDataSize9Bit =  0b1000,
  SspDataSize10Bit = 0b1001,
  SspDataSize11Bit = 0b1010,
  SspDataSize12Bit = 0b1011,
  SspDataSize13Bit = 0b1100,
  SspDataSize14Bit = 0b1101,
  SspDataSize15Bit = 0b1110,
  SspDataSize16Bit = 0b1111,
}SspDataSize;
  
  
typedef enum
{
  SspFrameFormatSPI       = 0b00,
  SspFrameFormatTI        = 0b01,
  SspFrameFormatMicrowire = 0b10,
}SspFrameFormat;

int8 initializeSSP0(uint32 baudrate,SspDataSize dataSize,SspFrameFormat frameFormat);
int8 initializeSSP1(uint32 baudrate,SspDataSize dataSize,SspFrameFormat frameFormat);
int8 putcharSsp1(uint16 c);
int8 getcharSsp1(uint16 *c);

#endif
