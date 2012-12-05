/**
 * @file dac.h
 * @author Alexander Rössler
 * @brief DAC Library
 * @date 20-11-2012
 */

#ifndef _ADC_H_
#define _ADC_H_

#include <types.h>
#include <LPC17xx.h>
#include <dacDriver.h>

/** Initializes the DAC */
void initializeDac();
/** Sets the DAC to a specific value 
 *  @param value Value the DAC should be set to, must be between 0 and DAC_STEPS-1
 */
void setDacValue(uint16 value);
/** Sets the DAC to specific value given in Volts
 *  @param mV Value in millivolts, must be between VREFN and VREFP
 */
void setDacVolt(uint16 mV);

#endif
