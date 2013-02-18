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

#include <sspDriver.h>
#include <types.h>
#include <LPC17xx.h>
#include <gpio.h>
#include <pincon.h>

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

/** Initializes SSP1 hardware device
 *  @param baudrate The baudrate the SSP device should be initialized with.
 *  @param dataSize The size of one chunk that is transfered.
 *  @param frameFormat The frame format the hardware should be initialized with.
 *  @param masterSlave Slave or master mode.
 *  @param loopbackMode Enable or disables the loopback mode.
 *  @param slaveOutput Defines wheter a slave should output bytes or not.
 *  @param clockOutPolarity Defines the clock out polarity if device is in SPI mode.
 *  @param clockOutPhase Defines the clock out phase if device is in SPI mode.
 */
extern void initializeSsp1(uint32 baudrate,
                           SspDataSize dataSize,
                           SspFrameFormat frameFormat, 
                           SspMasterSlave masterSlave, 
                           SspLoopbackMode loopbackMode,
                           SspSlaveOutput slaveOutput,
                           SspClockOutPolarity clockOutPolarity,
                           SspClockOutPhase clockOutPhase
                          );
/** Initializes a SEL pin for the SSP1 device.
 *  @param id Id of the SEL pin.
 *  @param port The port of the pin.
 *  @param pin The pin number.
 */
extern void initializeSelSsp1(uint8 id, uint8 port, uint8 pin);
/** Puts a character to the SSP1 bus and discards the received data.
 *  @param selId Id of the SEL pin.
 *  @param data The data to write.
 */
extern void putcharSsp1(uint8 selId, uint16 data);
/** Gets a character from the SSP1 bus and writes 0 to the bus 
 *  @param selId Id of the SEL pin.
 *  @param data A pointer to the data to read.
 */
extern void getcharSsp1(uint8 selId, uint16* data);
/** Reads and writes data to the SSP1 bus
 *  @param selId Id of the SEL pin.
 *  @param writeData The data to write.
 *  @param readData A pointer to the data to read.
 */
extern void readWriteSsp1(uint8 selId, uint16 writeData, uint16* readData);

/** Initializes SSP0 hardware device
 *  @param baudrate The baudrate the SSP device should be initialized with.
 *  @param dataSize The size of one chunk that is transfered.
 *  @param frameFormat The frame format the hardware should be initialized with.
 *  @param masterSlave Slave or master mode.
 *  @param loopbackMode Enable or disables the loopback mode.
 *  @param slaveOutput Defines wheter a slave should output bytes or not.
 *  @param clockOutPolarity Defines the clock out polarity if device is in SPI mode.
 *  @param clockOutPhase Defines the clock out phase if device is in SPI mode.
 */
extern void initializeSsp0(uint32 baudrate,
                           SspDataSize dataSize,
                           SspFrameFormat frameFormat, 
                           SspMasterSlave masterSlave, 
                           SspLoopbackMode loopbackMode,
                           SspSlaveOutput slaveOutput,
                           SspClockOutPolarity clockOutPolarity,
                           SspClockOutPhase clockOutPhase
                          );
/** Initializes a SEL pin for the SSP0 device.
 *  @param id Id of the SEL pin.
 *  @param port The port of the pin.
 *  @param pin The pin number.
 */
extern void initializeSelSsp0(uint8 id, uint8 port, uint8 pin);
/** Puts a character to the SSP0 bus and discards the received data.
 *  @param selId Id of the SEL pin.
 *  @param data The data to write.
 */
extern void putcharSsp0(uint8 selId, uint16 data);
/** Gets a character from the SSP0 bus and writes 0 to the bus 
 *  @param selId Id of the SEL pin.
 *  @param data A pointer to the data to read.
 */
extern void getcharSsp0(uint8 selId, uint16* data);
/** Reads and writes data to the SSP0 bus
 *  @param selId Id of the SEL pin.
 *  @param writeData The data to write.
 *  @param readData A pointer to the data to read.
 */
extern void readWriteSsp0(uint8 selId, uint16 writeData, uint16* readData);

/**
 * @}
 */
