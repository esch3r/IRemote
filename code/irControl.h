/**
 * @file irControl.h
 * @author Alexander Rössler
 * @brief Implements various function for IR capturing and playing
 * @date 10-12-2012
 */

#ifndef _IRCONTROL_H_
#define _IRCONTROL_H_

#define IR_TIMEOUT  30000   // Time that should pass until a timeout (end of frame) occurs
#define IR_WAIT_TIMEOUT 15   // Time that should pass until a capture event will be aborted
#define IR_CAPTURE_PORT 2
#define IR_CAPTURE_PIN 6
#define IR_PWM_PIN 0

#include <pwm.h>
#include <timer.h>
#include <gpio.h>
#include <pincon.h>
#include <uart.h>

#include "irData.h"
#include "iremote.h"

#include <stdio.h>
#include <string.h>

int8 initializeIrControl(void);

void startIrCapture(void);
void stopIrCapture(void);
IrCommand *getIrCommand(void);
int8 saveIrFrame(CircularBuffer* buffer, IrCommand* command);
void captureFunction(void);

void runIrCommand(IrCommand *command);
void stopIrCommand(void);
void runFunction(void);
uint8 isCommandRunning(void);

void outputCommand(IrCommand* command);

void setIrReceiveTimeout(uint32 timeout);
void setIrSendTimeout(uint32 timeout);
void setIrRepeatCount(uint8 count);

#endif
