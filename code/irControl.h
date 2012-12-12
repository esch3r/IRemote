/**
 * @file irControl.h
 * @author Alexander Rössler
 * @brief Implements various function for IR capturing and playing
 * @date 16-10-2012
 */

#ifndef _IRCONTROL_H_
#define _IRCONTROL_H_

#include <pwm.h>
#include <timer.h>
#include <gpio.h>
#include <pincon.h>
#include <uart.h>

#include <stdio.h>
#include <string.h>

CircularBuffer buffer0;
char stringBuffer[1000];

void initializeIrControl();
void startIrCapture();
void stopIrCapture();
void captureFunction();
void processData();

#endif
