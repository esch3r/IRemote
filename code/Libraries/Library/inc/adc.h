/**
 * @file adc.h
 * @author Alexander Rössler
 * @brief ADC Library
 * @date 14-11-2012
 */

#ifndef _ADC_H_
#define _ADC_H_

#include <types.h>
#include <LPC17xx.h>
#include <adcDriver.h>

/** Initializes the ADC
 *  @param clk Clock the ADC should be refreshed with in Hz.
 *  @param pin Pin the ADC should read from.
 *  @param burstMode If 1 the ADC runs in burst mode, continuously refreshing its data.
 *  @return Return 0 if successful, -1 if not.
 */
int8 initializeAdc(uint32 clk, uint8 pin, uint8 burstMode);
/** Deinitializes the ADC
 *  @return Returns 0 if successful, -1 if not.
 */
int8 deinitializeAdc();
/** Reads the value from the adc
 *  @param value A pointer to the variable the value should be written to.
 *  @return Returns 0 if successful, -1 if not.
 */
int8 readAdc(uint16 *value);
#endif
