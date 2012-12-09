/**
 * @file gpio.h
 * @author Alexander Rössler
 * @brief GPIO Library
 * @date 08-12-2012
 */

#ifndef _GPIO_H_
#define _GPIO_H_ 

/** Set this constant to 0 if you want to write your own interrupt function */
#ifndef GPIO_IRQ_ENABLED
#define GPIO_IRQ_ENABLED 1
#endif

#include <gpioDriver.h>
#include <types.h>

enum GpioDirection { GpioDirectionInput = 0, 
                     GpioDirectionOutput = 1};
enum GpioInterruptType { GpioInterruptRisingEdge = 0,
                         GpioInterruptFallingEdge = 1,
                         GpioInterruptFallingAndRisingEdge = 2};
                         
void (* functionPointers0[GPIO0_INT_PIN_COUNT])(void);
void (* functionPointers2[GPIO2_INT_PIN_COUNT])(void);

/** Set the direction of the GPIO pin
 *  @param port Port of the pin.
 *  @param pin Pin number.
 *  @param direction Input or output.
 */
inline void setGpioDirection(uint8 port, uint8 pin, enum GpioDirection direction);
/** Reads data from the  GPIO pin
 *  @param port Port of the pin.
 *  @param pin Pin number.
 *  @return Value that has been read from the pin.
 */
inline uint8 readGpio(uint8 port, uint8 pin);
/** Write data to the GPIO pin
 *  @param port Port of the pin.
 *  @param pin Pin number.
 *  @param value Value that should be written to the pin.
 */
inline void writeGpio(uint8 port, uint8 pin, uint8 value);
/** Set the GPIO pin
 *  @param port Port of the pin.
 *  @param pin Pin number.
 */
inline void setGpio(uint8 port, uint8 pin);
/** Clear the GPIO pin
 *  @param port Port of the pin.
 *  @param pin Pin number.
 */
inline void clearGpio(uint8 port, uint8 pin);
/** Toggle the GPIO pin
 *  @param port Port of the pin.
 *  @param pin Pin number.
 */
inline void toggleGpio(uint8 port, uint8 pin);
/** Enable interrupts on the GPIO pin
 *  @param port Port of the pin.
 *  @param pin Pin number.
 *  @param type Rising edge, falling edge or both.
 *  @param func Function that should be triggered if an interrupt happens.
 */
void enableGpioInterrupt(uint8 port, uint8 pin, enum GpioInterruptType type, void (* func)(void));
/** Disable interrupts on the GPIO pin
 *  @param port Port of the pin.
 *  @param pin Pin number.
*/
void disableGpioInterrupt(uint8 port, uint8 pin);

#endif
