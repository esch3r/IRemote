/**
 * @file irControl.h
 * @author Alexander Rössler
 * @brief Implements various function for IR capturing and playing
 * @date 10-12-2012
 */

#ifndef _IRCONTROL_H_
#define _IRCONTROL_H_

//#define IR_TIMEOUT  30000   
//#define IR_WAIT_TIMEOUT 15   // Time that should pass until a capture event will be aborted
//#define IR_CAPTURE_PORT 2
//#define IR_CAPTURE_PIN 6
//#define IR_PWM_PIN 0
//#include <stdio.h>
//#include <string.h>

#include <pwm.h>
#include <timer.h>
#include <gpio.h>
#include <pincon.h>
#include <circularbuffer.h>
#include <rfm12.h>
#include <led.h>
#include "iremote.h"

#include "remotecommand.h"
#include "iremote.h"


static const uint32 IrTimeout = 30000;  // Time that should pass until a timeout (end of frame) occurs
static const uint32 IrWaitTimeout = 15000;  // Time that should pass until a capture event will be aborted
static const uint8  IrCapturePort = 2;
static const uint8  IrCapturePin = 6;
static const Pwm    IrPwm = Pwm0;

static const uint32 Radio433MhzTimeout = 9000;
static const uint32 Radio433MhzWaitTimeout = 15000;
static const uint8  Radio433MhzCapturePort = 2;
static const uint8  Radio433MhzCapturePin = 2;
static const uint8  Radio433MhzSelPort = 2;
static const uint8  Radio433MhzSelPin = 2;

static const uint32 Radio868MhzTimeout = 9000;
static const uint32 Radio868MhzWaitTimeout = 15000;
static const uint8  Radio868MhzCapturePort = 2;
static const uint8  Radio868MhzCapturePin = 2;
static const uint8  Radio868MhzSelPort = 2;
static const uint8  Radio868MhzSelPin = 2;

typedef enum {
    RemoteControl_Medium_Ir = 0,
    RemoteControl_Medium_433Mhz = 1,
    RemoteControl_Medium_868Mhz = 2
} RemoteControl_Medium;

int8 RemoteControl_initialize(void);

void RemoteControl_startCapture(RemoteControl_Medium medium);
void RemoteControl_stopCapture(void);
RemoteCommand* RemoteControl_command(void);

void RemoteControl_runCommand(RemoteCommand *command);
void RemoteControl_stopCommand(void);
uint8 RemoteControl_isCommandRunning(void);

void RemoteControl_outputCommand(RemoteCommand* command);

void RemoteControl_setReceiveTimeout(RemoteControl_Medium medium, uint32 timeout);
void RemoteControl_setSendTimeout(RemoteControl_Medium medium, uint32 timeout);
void RemoteControl_setRepeatCount(RemoteControl_Medium medium, uint8 count);

#endif
