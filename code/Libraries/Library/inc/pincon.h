 /**
 * @file pincon.h
 * @author Alexander Rössler
 * @brief PINCON Library
 * @date 08-12-2012
 */

#ifndef _PIN_H_
#define _PIN_H_ 

#include <pinconDriver.h>
#include <types.h>

enum PinFunction { PinPrimaryFunction = 0,
                   PinFirstAlternateFunction = 1,
                   PinSecondAlternateFunction = 2,
                   PinThirdAlternateFunction = 3};
enum PinMode { PinModePullUp = 0,
               PinModeRepeater = 1,
               PinModeNoPullUpDown = 2,
               PinModePullDown = 3};
enum PinOpenDrainMode { PinDisableOpenDrain = 0,
                        PinEnableOpenDrain = 1};

/** Set the function of the selected pin, lookup the functions in the manual (PINSEL)
 *  @param port Port of the pin.
 *  @param pin Pin number.
 *  @param function Function to be selected.
 */
void setPinFunction(uint8 port, uint8 pin, enum PinFunction function);
/** Set the mode of the selected pin, lookup the modes in the manual (PINMODE)
 *  @param port Port of the pin.
 *  @param pin Pin number.
 *  @param mode Mode that should be set on the pin.
 */
void setPinMode(uint8 port, uint8 pin, enum PinMode mode);
/** Set open drain mode of the selected pin, lookup the open drain modes in the manul (PINMODE_OD)
 *  @param port Port of the pin.
 *  @param pin Pin number.
 *  @param mode Open drain mode that should be set on the pin (enabled or not).
 */
void setPinOpenDrainMode(uint8 port, uint8 pin, enum PinOpenDrainMode mode);

#endif
