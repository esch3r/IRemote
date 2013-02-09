/**
 * @file ssp.h
 * @author Johannes Wimmer & Alexander Rössler
 * @brief SSP Library
 * @date 16-10-2012
 */
/** @defgroup SSP SSP (Synchronous Serial Port)
 * @ingroup Peripheral
 * @{
 */
#pragma once

#include <types.h>
#include <LPC17xx.h>
#include <circularbuffer.h>
#include <gpio.h>
#include <pincon.h>

/* SSP Status register */
#define SSPSR_TFE               (1 << 0)
#define SSPSR_TNF               (1 << 1) 
#define SSPSR_RNE               (1 << 2)
#define SSPSR_RFF               (1 << 3) 
#define SSPSR_BSY               (1 << 4)

/* SSP CR0 register */
#define SSPCR0_DSS              (1 << 0)
#define SSPCR0_FRF              (1 << 4)
#define SSPCR0_SPO              (1 << 6)
#define SSPCR0_SPH              (1 << 7)
#define SSPCR0_SCR              (1 << 8)

/* SSP CR1 register */
#define SSPCR1_LBM              (1 << 0)
#define SSPCR1_SSE              (1 << 1)
#define SSPCR1_MS               (1 << 2)
#define SSPCR1_SOD              (1 << 3)

/* SSP Interrupt Mask Set/Clear register */
#define SSPIMSC_RORIM   (1 << 0)
#define SSPIMSC_RTIM    (1 << 1)
#define SSPIMSC_RXIM    (1 << 2)
#define SSPIMSC_TXIM    (1 << 3)

/* SSP0 Interrupt Status register */
#define SSPRIS_RORRIS   (1 << 0)
#define SSPRIS_RTRIS    (1 << 1)
#define SSPRIS_RXRIS    (1 << 2)
#define SSPRIS_TXRIS    (1 << 3)

/* SSP0 Masked Interrupt register */
#define SSPMIS_RORMIS   (1 << 0)
#define SSPMIS_RTMIS    (1 << 1)
#define SSPMIS_RXMIS    (1 << 2)
#define SSPMIS_TXMIS    (1 << 3)

/* SSP0 Interrupt clear register */
#define SSPICR_RORIC    (1 << 0)
#define SSPICR_RTIC             (1 << 1)

/* ATMEL SEEPROM command set */
#define WREN            0x06            /* MSB A8 is set to 0, simplifying test */
#define WRDI            0x04
#define RDSR            0x05
#define WRSR            0x01
#define READ            0x03
#define WRITE           0x02

/* RDSR status bit definition */
#define RDSR_RDY        0x01
#define RDSR_WEN        0x02

/** Size of circular buffer for reading */
#ifndef SSP_READ_BUFFER_SIZE
#define SSP1_READ_BUFFER_SIZE       100
#define SSP0_READ_BUFFER_SIZE       100
#endif
/** Size of circular buffer for writing */
#ifndef SSP_WRITE_BUFFER_SIZE
#define SSP0_WRITE_BUFFER_SIZE      100
#define SSP1_WRITE_BUFFER_SIZE      100
#endif
/** Defines wheter the buffer should be used or not */
#ifndef USE_SSP_BUFFER
#define USE_SSP_BUFFER 1
#endif

/** Defines how many SEL lines are possible */
#define SSP_MAX_SEL_PINS   10

#define SSP_RXIM     (1 << 2)    //Interrupt when RxFifo half full
#define SSP_TXIM    (1 << 3)     //Interrupt Enable TxFifo half empty
#define SSP_RORIM   (1 << 0)     //Interrupt when receive overrun
#define SSP_RTIM    (1 << 1)     //Interrupt when receive timeout

#define SYSTEM_CLK      SystemCoreClock
#define SSP_CLK         SYSTEM_CLK/4
#define SSP_FIFOSIZE    8

#define SSP0_SET_CPSR(x)                      LPC_SSP0->CPSR = x
#define SSP0_SET_CORE_CLK_DIVISOR_1()         LPC_SC->PCLKSEL1 &= ~(0b11<< 10); LPC_SC->PCLKSEL1 |= (0b01<< 10);
#define SSP0_SET_CORE_CLK_DIVISOR_2()         LPC_SC->PCLKSEL1 &= ~(0b11<< 10); LPC_SC->PCLKSEL1 |= (0b10<< 10);
#define SSP0_SET_CORE_CLK_DIVISOR_4()         LPC_SC->PCLKSEL1 &= ~(0b11<< 10); 
#define SSP0_SET_CORE_CLK_DIVISOR_8()         LPC_SC->PCLKSEL1 |= (0b11<< 10);

#define SSP0_ENABLE_POWER()         LPC_SC->PCONP |= (1 << 21)            //enable PCONP for SSPI
#define SSP0_SET_SCR(x)             LPC_SSP0->CR0  |= (x << 8)
#define SSP0_SET_DATA_SIZE(x)       LPC_SSP0->CR0 |= (x << 0)
#define SSP0_SET_FRAME_FORMAT(x)    LPC_SSP0->CR0 |= (x << 4)
#define SSP0_SET_CPOL(x)            LPC_SSP0->CR0 |= (x << 6)
#define SSP0_SET_CPHA(x)            LPC_SSP0->CR0 |= (x << 7)
#define SSP0_SET_SOD(x)             LPC_SSP0->CR1 |= (x << 3)
#define SSP0_ENABLE_SSP()           LPC_SSP0->CR1 |= SSPCR1_SSE
#define SSP0_DISABLE_SSP()          LPC_SSP0->CR1 &= ~SSPCR1_SSE
#define SSP0_SET_MASTER()           LPC_SSP0->CR1 &= ~SSPCR1_MS
#define SSP0_CLEAR_MASTER()         LPC_SSP0->CR1 = SSPCR1_MS
#define SSP0_SET_LOOPBACK()         LPC_SSP0->CR1 |= SSPCR1_LBM
#define SSP0_CLEAR_LOOPBACK()       LPC_SSP0->CR1 &= ~SSPCR1_LBM
#define SSP0_INIT_SSEL()            LPC_PINCON->PINSEL3  |= (0b11 << 10); // LPC_PINCON->PINMODE3 &= ~(0b11 << 10)  // P1.21 pull-up Resistor
#define SSP0_DEACTIVATE_SSEL()      LPC_PINCON->PINSEL3  &= ~(0b11 << 10);  LPC_GPIO1->FIODIR |= (1 << 21)
#define SSP0_INIT_SCK()             LPC_PINCON->PINSEL3  |= (0b11 << 8); // LPC_PINCON->PINMODE3 &= ~(0b11 << 8)  // P1.20 pull-up Resistor
#define SSP0_INIT_MISO()            LPC_PINCON->PINSEL3  |= (0b11 << 14); // LPC_PINCON->PINMODE3 &= ~(0b11 << 14)  // P1.23 pull-up Resistor
#define SSP0_INIT_MOSI()            LPC_PINCON->PINSEL3  |= (0b11 << 16); // LPC_PINCON->PINMODE3 &= ~(0b11 << 16)  // P1.24 pull-up Resistor
#define SSP0_ENABLE_IRQ()           NVIC_EnableIRQ(SSP0_IRQn)
#define SSP0_SET_IRQS()             NVIC_SetPendingIRQ(SSP0_IRQn)
#define SSP0_SET_RXIM_AND_TXIM_INTERRUPT()   LPC_SSP0->IMSC |= SSP_RXIM | SSP_TXIM
#define SSP0_SET_TIM_AND_RORIM_INTERRUPT()   LPC_SSP0->IMSC |= SSP_RORIM | SSP_RTIM;

#define SSP0_TRANSMIT_BUFFER_NOT_EMPTY_OR_BUSY()    ((LPC_SSP0->SR & (SSPSR_TNF|SSPSR_BSY)) != SSPSR_TNF)
#define SSP0_RECEIVE_BUFFER_EMPTY_OR_BUSY()         ((LPC_SSP0->SR & (SSPSR_BSY|SSPSR_RNE)) != SSPSR_RNE )
#define SSP0_RECEIVE_BUFFER_NOT_EMPTY()     (LPC_SSP0->SR & SSPSR_RNE)
#define SSP0_BUSY()                         (LPC_SSP0->SR & SSPSR_BSY)
#define SSP0_WRITE_DATA_REGISTER(x)         LPC_SSP0->DR = x
#define SSP0_READ_DATA_REGISTER()           LPC_SSP0-> DR

#define SSP0_READ_CHAR()            LPC_SSP0->DR
#define SSP0_SET_CHAR(c)            LPC_SSP0->DR  =  (c << 0)
#define SSP0_FIFO_TRANSMIT_EMPTY    (LPC_SSP0->SR &  (1 << 0))
#define SSP0_FIFO_RECEIVE_EMPTY     (LPC_SSP0->SR &  (0 << 3))
#define SSP0_IRQHANDLER()           SSP0_IRQHandler(void)
#define SSP0_RESET_IRQS()           LPC_SSP0->ICR |= 0b11
#define SSP0_RESET_RORIC_INTERRUPT() LPC_SSP0->ICR = SSPICR_RORIC
#define SSP0_RESET_RTIC_INTERRUPT() LPC_SSP0->ICR = SSPICR_RTIC
#define SSP0_GET_INTERRUPT_STATUS() LPC_SSP0->MIS

#define SSP1_SET_CPSR(x)                      LPC_SSP1->CPSR = x
#define SSP1_SET_CORE_CLK_DIVISOR_1()         LPC_SC->PCLKSEL0 &= ~(0b11<< 20); LPC_SC->PCLKSEL0 |= (0b01<< 20);
#define SSP1_SET_CORE_CLK_DIVISOR_2()         LPC_SC->PCLKSEL0 &= ~(0b11<< 20); LPC_SC->PCLKSEL0 |= (0b10<< 20);
#define SSP1_SET_CORE_CLK_DIVISOR_4()         LPC_SC->PCLKSEL0 &= ~(0b11<< 20); 
#define SSP1_SET_CORE_CLK_DIVISOR_8()         LPC_SC->PCLKSEL0 |= (0b11<< 20);

#define SSP1_ENABLE_POWER()         LPC_SC->PCONP |= (1 << 10)            //enable PCONP for SSPI
#define SSP1_SET_SCR(x)             LPC_SSP1->CR0  |= (x << 8)
#define SSP1_SET_DATA_SIZE(x)       LPC_SSP1->CR0 |= (x << 0)
#define SSP1_SET_FRAME_FORMAT(x)    LPC_SSP1->CR0 |= (x << 4)
#define SSP1_SET_CPOL(x)            LPC_SSP1->CR0 |= (x << 6)
#define SSP1_SET_CPHA(x)            LPC_SSP1->CR0 |= (x << 7)
#define SSP1_SET_SOD(x)             LPC_SSP1->CR1 |= (x << 3)
#define SSP1_ENABLE_SSP()           LPC_SSP1->CR1 |= SSPCR1_SSE
#define SSP1_DISABLE_SSP()          LPC_SSP1->CR1 &= ~SSPCR1_SSE
#define SSP1_SET_MASTER()           LPC_SSP1->CR1 &= ~SSPCR1_MS
#define SSP1_CLEAR_MASTER()         LPC_SSP1->CR1 = SSPCR1_MS
#define SSP1_SET_LOOPBACK()         LPC_SSP1->CR1 |= SSPCR1_LBM
#define SSP1_CLEAR_LOOPBACK()       LPC_SSP1->CR1 &= ~SSPCR1_LBM
#define SSP1_INIT_SSEL()            LPC_PINCON->PINSEL0  &= ~(0b11 << 12); LPC_PINCON->PINSEL0  |= (0b10 << 12);//P0_6
#define SSP1_DEACTIVATE_SSEL()      LPC_PINCON->PINSEL3  &= ~(0b11 << 10);  LPC_GPIO1->FIODIR |= (1 << 21)
#define SSP1_INIT_SCK()             LPC_PINCON->PINSEL0  &= ~(0b11 << 14); LPC_PINCON->PINSEL0  |= (0b10 << 14);//P0_7
#define SSP1_INIT_MISO()            LPC_PINCON->PINSEL0  &= ~(0b11 << 16); LPC_PINCON->PINSEL0  |= (0b10 << 16);//P0_8
#define SSP1_INIT_MOSI()            LPC_PINCON->PINSEL0  &= ~(0b11 << 18); LPC_PINCON->PINSEL0  |= (0b10 << 18);//P0_9
#define SSP1_ENABLE_IRQ()           NVIC_EnableIRQ(SSP1_IRQn)
#define SSP1_SET_IRQS()             NVIC_SetPendingIRQ(SSP1_IRQn)
#define SSP1_SET_RXIM_AND_TXIM_INTERRUPT()   LPC_SSP1->IMSC |= SSP_RXIM | SSP_TXIM
#define SSP1_SET_TIM_AND_RORIM_INTERRUPT()   LPC_SSP1->IMSC |= SSP_RORIM | SSP_RTIM;

#define SSP1_TRANSMIT_BUFFER_NOT_EMPTY_OR_BUSY()    ((LPC_SSP1->SR & (SSPSR_TNF|SSPSR_BSY)) != SSPSR_TNF)
#define SSP1_RECEIVE_BUFFER_EMPTY_OR_BUSY()         ((LPC_SSP1->SR & (SSPSR_BSY|SSPSR_RNE)) != SSPSR_RNE )
#define SSP1_RECEIVE_BUFFER_NOT_EMPTY()     (LPC_SSP1->SR & SSPSR_RNE)
#define SSP1_BUSY()                         (LPC_SSP1->SR & SSPSR_BSY)
#define SSP1_WRITE_DATA_REGISTER(x)         LPC_SSP1->DR = x
#define SSP1_READ_DATA_REGISTER()           LPC_SSP1-> DR

#define SSP1_READ_CHAR()            LPC_SSP1->DR
#define SSP1_SET_CHAR(c)            LPC_SSP1->DR  =  (c << 0)
#define SSP1_FIFO_TRANSMIT_EMPTY    (LPC_SSP1->SR &  (1 << 0))
#define SSP1_FIFO_RECEIVE_EMPTY     (LPC_SSP1->SR &  (0 << 3))
#define SSP1_IRQHANDLER()           SSP1_IRQHandler(void)
#define SSP1_RESET_IRQS()           LPC_SSP1->ICR |= 0b11
#define SSP1_RESET_RORIC_INTERRUPT() LPC_SSP1->ICR = SSPICR_RORIC
#define SSP1_RESET_RTIC_INTERRUPT() LPC_SSP1->ICR = SSPICR_RTIC
#define SSP1_GET_INTERRUPT_STATUS() LPC_SSP1->MIS

/** Maintains the data size of one transmission byte */
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
  
/** The frame format which should be used for the SSP bus */
typedef enum
{
  SspFrameFormatSPI       = 0b00,
  SspFrameFormatTI        = 0b01,
  SspFrameFormatMicrowire = 0b10,
}SspFrameFormat;

/** Connect to the bus a slave or master */
typedef enum
{
    SspMasterMode = 0,
    SspSlaveMode = 1
} SspMasterSlave;
    
/** With loopback enabled input is taken from the output pin */
typedef enum
{
    SspLoopbackEnabled = 1,
    SspLoopbackDisabled = 0
} SspLoopbackMode;

/** Only necessary for SPI frame format.
 * Low Clock Out Polarity means that the clock is kept low between frames
 * High Clock Out Polarity means that the clock is kept high between frames */
typedef enum
{
    SspLowClockOutPolarity = 0,
    SspHighClockOutPolarity = 1
} SspClockOutPolarity;

/** Only necessary for SPI frame format.
 *  First Clock Out Phase means that the ssp controller captures the signal on the first transition of the clock
 *  Second Clock Out Phase means that the ssp controller captures the signal on the second transition of the clock */
typedef enum 
{
    SspFirstClockOutPhase = 0,
    SspSecondClockOutPhase = 1
} SspClockOutPhase;

/** Defines wheter a slave should output something on the bus or not */
typedef enum
{
    SspSlaveOutputEnabled = 0,
    SspSlaveOutputDisabled = 1
} SspSlaveOutput;

extern void initializeSsp1(uint32 baudrate,
                           SspDataSize dataSize,
                           SspFrameFormat frameFormat, 
                           SspMasterSlave masterSlave, 
                           SspLoopbackMode loopbackMode,
                           SspSlaveOutput slaveOutput,
                           SspClockOutPolarity clockOutPolarity,
                           SspClockOutPhase clockOutPhase
                          );
extern void initializeSelSsp1(uint8 id, uint8 port, uint8 pin);
extern void putcharSsp1(uint8 selId, uint16 data);
extern void getcharSsp1(uint8 selId, uint16* data);
extern void readWriteSsp1(uint8 selId, uint16 writeData, uint16* readData);

extern void initializeSsp0(uint32 baudrate,
                           SspDataSize dataSize,
                           SspFrameFormat frameFormat, 
                           SspMasterSlave masterSlave, 
                           SspLoopbackMode loopbackMode,
                           SspSlaveOutput slaveOutput,
                           SspClockOutPolarity clockOutPolarity,
                           SspClockOutPhase clockOutPhase
                          );
extern void initializeSelSsp0(uint8 id, uint8 port, uint8 pin);
extern void putcharSsp0(uint8 selId, uint16 data);
extern void getcharSsp0(uint8 selId, uint16* data);
extern void readWriteSsp0(uint8 selId, uint16 writeData, uint16* readData);

/**
 * @}
 */
