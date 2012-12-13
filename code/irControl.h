/**
 * @file irControl.h
 * @author Alexander Rössler
 * @brief Implements various function for IR capturing and playing
 * @date 10-12-2012
 */

#ifndef _IRCONTROL_H_
#define _IRCONTROL_H_

#define IR_TIMEOUT  20000   // Time that should pass until a timeout (end of frame) occurs
#define IR_CAPTURE_PORT 2
#define IR_CAPTURE_PIN 6

#include <pwm.h>
#include <timer.h>
#include <gpio.h>
#include <pincon.h>
#include <uart.h>

#include "irData.h"

#include <stdio.h>
#include <string.h>

void initializeIrControl();
void startIrCapture();
void stopIrCapture();
void captureFunction();
void processData();
void saveIrFrame(CircularBuffer *buffer, IrCommand *command);

#endif
