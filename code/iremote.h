/**
 * @file iremote.h
 * @author Alexander Rössler
 * @brief IRemote specific functions
 * @date 4-01-2012
 */

#pragma once

#define PRINTF_BUFFER_SIZE  420

#include <wifly.h>
#include <uart.h>
#include <led.h>
#include <timer.h>
#include <stdarg.h>
#include <stdio.h>
#include <led.h>
#include <iap.h>
#include <timeout.h>
#include <button.h>
#include <pwm.h>
#include <crc.h>
#include <rfm12.h>
#include "irControl.h"

typedef enum {
    ApplicationStateIdle = 0,
    ApplicationStateCaptureCommand = 1,
    ApplicationStateRunCommand = 2,
    ApplicationStateFlashFirmware = 3,
    ApplicationStateWiFlyTest = 100
} ApplicationState;

typedef struct {
    uint8  firstStartIdentificator; // if this is not 40 then we have a first start
    uint32 irReceiveTimeout;
    uint32 irSendTimeout;
    uint32 irRepeatCount;
    char   wlanSsid[100];
    char   wlanPhrase[100];
    char   wlanKey[100];
    char   wlanHostname[100];
    uint8  wlanAuth;
    uint8  wlanDhcp;
    char   wlanIp[20];
    char   wlanMask[20];
    char   wlanGateway[20];
    
} ApplicationSettings;

typedef enum {
        SerialConnection = 0x01,
        NetworkConnection = 0x02
    } ActiveConnection;
    
    
int8 initializeHardware(void);
int8 initializeVariables(void);
int8 initializeSerialConnection(void);
int8 initializeNetworkConnection(void);

int8 startWlanInfrastructureMode(ApplicationSettings *settings);
int8 startWlanAdhocMode(void);
    
int8 printfData(char* format, ...);
int8 putcharData(char c);
int8 writeData(void *data, uint32 length);

void errorCommand();
void errorWiFly();
void printUnknownCommand(void);
void printParameterMissing(void);
void printAcknowledgement(void);
void printError(char *message);
void printAliveMessage(void);
bool compareBaseCommand(char *original, char *received);
bool compareExtendedCommand(char *original, char *received);
void processCommand(char *buffer);

void startState(ApplicationState state);

void mainTask(void);
void ledTask(void);
void buttonTask(void);

uint32 hex2int(char *a, unsigned int len);
