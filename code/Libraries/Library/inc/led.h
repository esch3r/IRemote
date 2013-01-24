/**
 * @file led.h
 * @author Alexander Rössler
 * @brief Led Library
 * @date 16-10-2012
 */

#ifndef _LEDLIB_H_
#define _LEDLIB_H

/* With these values you can modify the behavior of the Led functions */
#define LED_BLINK_TIME 100
#define LED_RUNNING_TIME 30

#include <ledDriver.h>
#include <gpio.h>
#include <types.h>

typedef struct {
    uint8 port;
    uint8 pin;
    bool  lowActive;
} LedConfig;

void initializeLed(uint8 port, uint8 pin, bool lowActive);

/** Initializes the Leds. Per default red and green Leds will be enabled. */
uint8 initializeLeds(void);
/** Enables or disables the green Leds if possible.
 *  @param enabled
 */
void setGreenLedsEnabled(bool enabled);
/** Enables or disables the red Leds if possible.
 *  @param enabled
 */
void setRedLedsEnabled(bool enabled);
/** Sets a Led.
 *  @param id Id of the Led to set.
 */
void setLed(uint8 id);
/** Clears a Led.
 *  @param id Id of the Led to clear.
 */
void clearLed(uint8 id);
/** Toggles a Led.
 *  @param id Id of the Led to toggle.
 */
void toggleLed(uint8 id);
/** Reads wheter a Led is set or not.
 *  @param id Id of the Led to read.
 *  \return true or false 
 */
bool readLed(uint8 id);
/** Let the Led blink for a short time. Uses timer 0.
 *  @param id Id of the Led to blink.
 */
void blinkLed(uint8 id);
/** Let the Led blink 2 times. Uses timer 0.
 *  @param id Id of the Led to blink.
 */
void blinkLed2(uint8 id);
/** Let the Led blink 3 times. Uses timer 0.
 *  @param id Id of the Led to blink.
 */
void blinkLed3(uint8 id);
/** Clears all Leds. */
void clearAllLeds();
/** Sets all Leds */
void setAllLeds();
/** Make a running light from Led 1 to LEDCOUNT. Uses timer 0. */
void allLedsUp();
/** Make a running light from Led LEDCOUNT to 1. Uses timer 0. */
void allLedsDown();

#endif

/** ledLib.h */
