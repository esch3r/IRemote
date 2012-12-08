/**
 * @file gpio.h
 * @author Alexander Rössler
 * @brief GPIO Library
 * @date 08-12-2012
 */

#ifndef _GPIO_H_
#define _GPIO_H_ 

#include <gpioDriver.h>
#include <types.h>
#include <led.h>

enum GpioDirection { GpioDirectionInput = 0, 
                     GpioDirectionOutput = 1};
enum GpioInterruptType { GpioInterruptRisingEdge = 0,
                         GpioInterruptFallingEdge = 1,
                         GpioInterruptFallingAndRisingEdge = 2};
                         
void (* functionPointers0[GPIO0_INT_PIN_COUNT])(void);
void (* functionPointers2[GPIO2_INT_PIN_COUNT])(void);

inline void setGpioDirection(uint8 port, uint8 pin, enum GpioDirection direction);
inline uint8 readGpio(uint8 port, uint8 pin);
void writeGpio(uint8 port, uint8 pin, uint8 value);
inline void setGpio(uint8 port, uint8 pin);
inline void clearGpio(uint8 port, uint8 pin);
inline void toggleGpio(uint8 port, uint8 pin);
void enableGpioInterrupt(uint8 port, uint8 pin, enum GpioInterruptType type, void (* func)(void));
void disableGpioInterrupt(uint8 port, uint8 pin);

#endif
