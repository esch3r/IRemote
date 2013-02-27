/**
 * @file led.h
 * @author Alexander Rössler
 * @brief Led Library
 * @date 16-10-2012
 */
/** @defgroup LED LED Library
 * @ingroup Device
 * @{
 */
#pragma once

/* With these values you can modify the behavior of the Led functions */
#define LED_BLINK_TIME 100
#define LED_RUNNING_TIME 30
#define MAXLEDS 10

#include <types.h>
#include <timer.h>
#include <gpio.h>

typedef struct {
    uint8 port;
    uint8 pin;
    bool  lowActive;
} LedConfig;

typedef enum {
    Led0 = 0U,
    Led1 = 1U,
    Led2 = 2U,
    Led3 = 3U,
    Led4 = 4U,
    Led5 = 5U,
    Led6 = 6U,
    Led7 = 7U,
    Led8 = 8U,
    Led9 = 9U
} Led;

void Led_initialize(uint8 port, uint8 pin, bool lowActive);

/** Initializes the Leds. Per default red and green Leds will be enabled. */
uint8 initializeLeds(void);
/** Enables or disables the green Leds if possible.
 *  @param enabled
 */
/** Sets a Led.
 *  @param id Id of the Led to set.
 */
void Led_set(uint8 id);
/** Clears a Led.
 *  @param id Id of the Led to clear.
 */
void Led_clear(uint8 id);
/** Toggles a Led.
 *  @param id Id of the Led to toggle.
 */
void Led_toggle(uint8 id);
/** Reads wheter a Led is set or not.
 *  @param id Id of the Led to read.
 *  \return true or false 
 */
bool Led_read(uint8 id);
/** Let the Led blink for a short time. Uses timer 0.
 *  @param id Id of the Led to blink.
 */
void Led_blink(uint8 id);
/** Let the Led blink 2 times. Uses timer 0.
 *  @param id Id of the Led to blink.
 */
void Led_blink2(uint8 id);
/** Let the Led blink 3 times. Uses timer 0.
 *  @param id Id of the Led to blink.
 */
void Led_blink3(uint8 id);
/** Clears all Leds. */
void Led_clearAll();
/** Sets all Leds */
void Led_setAll();
/** Make a running light from Led 1 to LEDCOUNT. Uses timer 0. */
void Led_allUp();
/** Make a running light from Led LEDCOUNT to 1. Uses timer 0. */
void Led_allDown();

/**
 * @}
 */
