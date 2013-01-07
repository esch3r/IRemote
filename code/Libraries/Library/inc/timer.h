/**
 * @file timer.h
 * @author Alexander Rössler
 * @brief Timer Library
 * @date 16-10-2012
 */

#ifndef _TIMERLIB_H_
#define _TIMERLIB_H_

/** Set this constant to 0 if you want to write your own interrupt function */
#ifndef TIMER0_IRQ_ENABLED
#define TIMER0_IRQ_ENABLED 1
#endif
/** Set this constant to 0 if you want to write your own interrupt function */
#ifndef TIMER1_IRQ_ENABLED
#define TIMER1_IRQ_ENABLED 1
#endif
/** Set this constant to 0 if you want to write your own interrupt function */
#ifndef TIMER2_IRQ_ENABLED
#define TIMER2_IRQ_ENABLED 1
#endif
/** Set this constant to 0 if you want to write your own interrupt function */
#ifndef TIMER3_IRQ_ENABLED
#define TIMER3_IRQ_ENABLED 1
#endif

#define TIMER0 0
#define TIMER1 1
#define TIMER2 2
#define TIMER3 3

#include <types.h>
#include <timerDriver.h>

/** Initializes some hardware timer.
 *  @param khz The frequency the timer should run with. 
 *  @param intervalUs Tick interval in µs.
 *  @return Returns the id of the timer that was initialized or -1 if all timers are already in use.
 */
int8 initializeTimer(uint32 khz, uint32 intervalUs);
/** Initializes hardware timer 0.
 *  @param khz The frequency the timer should run with. 
 *  @param intervalUs Tick interval in µs.
 *  @return Returns 0 if successful or -1 if timer is already running.
 */
int8 initializeTimer0(uint32 khz, uint32 intervalUs);
/** Initializes hardware timer 1.
 *  @param khz The frequency the timer should run with. 
 *  @param intervalUs Tick interval in µs.
 *  @return Returns 0 if successful or -1 if timer is already running.
 */
int8 initializeTimer1(uint32 khz, uint32 intervalUs);
/** Initializes hardware timer 2.
 *  @param khz The frequency the timer should run with. 
 *  @param intervalUs Tick interval in µs.
 *  @return Returns 0 if successful or -1 if timer is already running.
 */
int8 initializeTimer2(uint32 khz, uint32 intervalUs);
/** Initializes hardware timer 3.
 *  @param khz The frequency the timer should run with. 
 *  @param intervalUs Tick interval in µs.
 *  @return Returns 0 if successful or -1 if timer is already running.
 */
int8 initializeTimer3(uint32 khz, uint32 intervalUs);


/** Deinitializes timer id 
 *  @param id Id of the timer that should be deinitialized.
 *  @return Return 0 on successful or -1 if timer is running.
 */
int8 deinitializeTimer(uint8 id);
/** Deinitializes timer 0
 *  @param id Id of the timer that should be deinitialized.
 *  @return Return 0 on successful or -1 if timer is running.
 */
int8 deinitializeTimer0(void);
/** Deinitializes timer 1
 *  @param id Id of the timer that should be deinitialized.
 *  @return Return 0 on successful or -1 if timer is running.
 */
int8 deinitializeTimer1(void);
/** Deinitializes timer 2
 *  @param id Id of the timer that should be deinitialized.
 *  @return Return 0 on successful or -1 if timer is running.
 */
int8 deinitializeTimer2(void);
/** Deinitializes timer 3
 *  @param id Id of the timer that should be deinitialized.
 *  @return Return 0 on successful or -1 if timer is running.
 */
int8 deinitializeTimer3(void);


/** Starts timer id.
 *  @param id Id of the timer that should be started. 0-3
 */
void startTimer(uint8 id);
/** Starts timer 0. */
void startTimer0(void);
/** Starts timer 1. */
void startTimer1(void);
/** Starts timer 2. */
void startTimer2(void);
/** Starts timer 3. */
void startTimer3(void);

/** Stops timer id.
 *  @param id Id of the timer that should be stopped. 0-3
 */
void stopTimer(uint8 id);
/** Stops timer 0. */
void stopTimer0(void);
/** Stops timer 1. */
void stopTimer1(void);
/** Stops timer 2. */
void stopTimer2(void);
/** Stops timer 3. */
void stopTimer3(void);

/** Resets timer id.
 *  @param id Id of the timer that should be reset. 0-3
 */
void resetTimer(uint8 id);
/** Resets timer 0. */
void resetTimer0(void);
/** Resets timer 1. */
void resetTimer1(void);
/** Resets timer 2. */
void resetTimer2(void);
/** Resets timer 3. */
void resetTimer3(void);


/** Sets tick inteval of timer id in microseconds.
 *  @param id Id of the timer. 0-3
 *  @param us Tick interval in µs.
 */
void setTimerIntervalUs(uint8 id, uint32 us);
/** Sets tick inteval of timer 0 in microseconds.
 *  @param us Tick interval in µs.
 */
void setIntervalUsTimer0(uint32 us);
/** Sets tick inteval of timer 1 in microseconds.
 *  @param us Tick interval in µs.
 */
void setIntervalUsTimer1(uint32 us);
/** Sets tick inteval of timer 2 in microseconds.
 *  @param us Tick interval in µs.
 */
void setIntervalUsTimer2(uint32 us);
/** Sets tick inteval of timer 1 in microseconds.
 *  @param us Tick interval in µs.
 */
void setIntervalUsTimer3(uint32 us);

/** Sets tick inteval of timer id in milliseconds.
 *  @param id Id of the timer. 0-3
 *  @param ms Tick interval in ms.
 */
void setTimerIntervalMs(uint8 id, uint32 ms);
/** Sets tick inteval of timer 3 in milliseconds.
 *  @param ms Tick interval in ms.
 */
void setIntervalMsTimer0(uint32 ms);
/** Sets tick inteval of timer 1 in milliseconds.
 *  @param ms Tick interval in ms.
 */
void setIntervalMsTimer1(uint32 ms);
/** Sets tick inteval of timer 2 in milliseconds.
 *  @param ms Tick interval in ms.
 */
void setIntervalMsTimer2(uint32 ms);
/** Sets tick inteval of timer 3 in milliseconds.
 *  @param ms Tick interval in ms.
 */
void setIntervalMsTimer3(uint32 ms);

/** Sets interrupt priority of the timer id.
 *  @param id Id of the timer. 0-3
 *  @param priority Priority level, smaller value means higher priority. Look at __NVIC_PRIO_BITS to see how many bits can be used.
 */
void setTimerPriority(uint8 id, uint8 priority);
/** Sets interrupt priority of the timer 0.
 *  @param priority Priority level, smaller value means higher priority. Look at __NVIC_PRIO_BITS to see how many bits can be used.
 */
void setPriorityTimer0(uint8 priority);
/** Sets interrupt priority of the timer 1.
 *  @param priority Priority level, smaller value means higher priority. Look at __NVIC_PRIO_BITS to see how many bits can be used.
 */
void setPriorityTimer1(uint8 priority);
/** Sets interrupt priority of the timer 2.
 *  @param priority Priority level, smaller value means higher priority. Look at __NVIC_PRIO_BITS to see how many bits can be used.
 */
void setPriorityTimer2(uint8 priority);
/** Sets interrupt priority of the timer 3.
 *  @param priority Priority level, smaller value means higher priority. Look at __NVIC_PRIO_BITS to see how many bits can be used.
 */
void setPriorityTimer3(uint8 priority);



/** Connects a function to the interrupt of timer id.
 *  @param id Id of the timer. 0-3
 *  @param func A pointer to the function that should be connected. e.g,; &allLedsDown
 */
void connectTimerFunction(uint8 id, void (* func)(void));
/** Connects a function to the interrupt of timer 0.
 *  @param func A pointer to the function that should be connected. e.g,; &allLedsDown
 */
void connectFunctionTimer0(void (* func)(void));
/** Connects a function to the interrupt of timer 1.
 *  @param func A pointer to the function that should be connected. e.g,; &allLedsDown
 */
void connectFunctionTimer1(void (* func)(void));
/** Connects a function to the interrupt of timer 2.
 *  @param func A pointer to the function that should be connected. e.g,; &allLedsDown
 */
void connectFunctionTimer2(void (* func)(void));
/** Connects a function to the interrupt of timer 3.
 *  @param func A pointer to the function that should be connected. e.g,; &allLedsDown
 */
void connectFunctionTimer3(void (* func)(void));

/** A simple delay function that runs on some timer.
 *  @param us Delay time in milliseconds.
 */
int8 delayUs(uint32 ms);
/** A simple delay function that runs on timer 0.
 *  Per default all delay functions should run on timer 0.
 *  @param us Delay time in microseconds.
 *  @return Returns the id of the timer that was initialized or -1 if all timers are already in use.
 */
int8 delayUsTimer0(uint32 us);
/** A simple delay function that runs on timer 1.
 *  @param us Delay time in microseconds.
 *  @return Returns 0 if successful or -1 if timer is already running.
 */
int8 delayUsTimer1(uint32 us);
/** A simple delay function that runs on timer 2.
 *  @param us Delay time in microseconds.
 *  @return Returns 0 if successful or -1 if timer is already running.
 */
int8 delayUsTimer2(uint32 us);
/** A simple delay function that runs on timer 3.
 *  @param us Delay time in microseconds.
 *  @return Returns 0 if successful or -1 if timer is already running.
 */
int8 delayUsTimer3(uint32 us);
/** A simple delay function that runs on some timer.
 *  @param ms Delay time in milliseconds.
 *  @return Returns 0 if successful or -1 if timer is already running.
 */
int8 delayMs(uint32 ms);
/** A simple delay function that runs on timer 0.
 *  Per default all delay functions should run on timer 0.
 *  @param ms Delay time in milliseconds.
 *  @return Returns the id of the timer that was initialized or -1 if all timers are already in use.
 */
int8 delayMsTimer0(uint32 ms);
/** A simple delay function that runs on timer 1.
 *  @param ms Delay time in milliseconds.
 *  @return Returns 0 if successful or -1 if timer is already running.
 */
int8 delayMsTimer1(uint32 ms);
/** A simple delay function that runs on timer 2.
 *  @param ms Delay time in milliseconds.
 *  @return Returns 0 if successful or -1 if timer is already running.
 */
int8 delayMsTimer2(uint32 ms);
/** A simple delay function that runs on timer 3.
 *  @param ms Delay time in milliseconds.
 *  @return Returns 0 if successful or -1 if timer is already running.
 */
int8 delayMsTimer3(uint32 ms);

/** Sets timer id to run only one interval and then call a function.
 *  @param ms Interval in milliseconds.
 *  @param func A pointer to the function that should be connected. e.g,; &allLedsDown
 *  @return Returns the id of the timer that was initialized or -1 if all timers are already in use.
 */
int8 singleShotTimer(uint32 ms, void (* func)(void));
/** Sets timer 0 to run only one interval and then call a function.
 *  @param ms Interval in milliseconds.
 *  @param func A pointer to the function that should be connected. e.g,; &allLedsDown
 *  @return Returns 0 if successful or -1 if timer is already running.
 */
int8 singleShotTimer0(uint32 ms, void (* func)(void));
/** Sets timer 1 to run only one interval and then call a function.
 *  @param ms Interval in milliseconds.
 *  @param func A pointer to the function that should be connected. e.g,; &allLedsDown
 *  @return Returns 0 if successful or -1 if timer is already running.
 */
int8 singleShotTimer1(uint32 ms, void (* func)(void));
/** Sets timer 2 to run only one interval and then call a function.
 *  @param ms Interval in milliseconds.
 *  @param func A pointer to the function that should be connected. e.g,; &allLedsDown
 *  @return Returns 0 if successful or -1 if timer is already running.
 */
int8 singleShotTimer2(uint32 ms, void (* func)(void));
/** Sets timer 3 to run only one interval and then call a function.
 *  @param ms Interval in milliseconds.
 *  @param func A pointer to the function that should be connected. e.g,; &allLedsDown
 *  @return Returns 0 if successful or -1 if timer is already running.
 */
int8 singleShotTimer3(uint32 ms, void (* func)(void));

/** Returns the counter value of a timer.
 *  @param id Id of the target timer
 *  @return counter value
 */
inline uint32 getCounterValueTimer(uint8 id);
/** Returns the counter value of TIMER0.
 *  @return counter value
 */
inline uint32 getCounterValueTimer0();
/** Returns the counter value of TIMER1.
 *  @return counter value
 */
inline uint32 getCounterValueTimer1();
/** Returns the counter value of TIMER2.
 *  @return counter value
 */
inline uint32 getCounterValueTimer2();
/** Returns the counter value of TIMER3.
 *  @return counter value
 */
inline uint32 getCounterValueTimer3();

int8 initializeCapCom0(uint32 khz, uint8 pin, void (* func)(void));
int8 initializeCapCom1(uint32 khz, uint8 pin, void (* func)(void));
int8 initializeCapCom2(uint32 khz, uint8 pin, void (* func)(void));
int8 initializeCapCom3(uint32 khz, uint8 pin, void (* func)(void));

#endif

/** timerLib.h */
