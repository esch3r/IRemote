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

typedef enum {
        SerialConnection = 0x01,
        NetworkConnection = 0x02
    } ActiveConnection;
    
int8 initializeSerialConnection(void);
int8 initializeNetworkConnection(void);
    
int8 printfData(char* format, ...);
int8 putcharData(char c);
int8 writeData(void *data, uint32 length);

void printUnknownCommand(void);
void printParameterMissing(void);
void printAcknowledgement(void);
void printError(char *message);
void printAliveMessage(void);

void errorCommand();
void errorWiFly();

bool compareBaseCommand(char *original, char *received);
bool compareExtendedCommand(char *original, char *received);

void ledTask(void);

uint32 hex2int(char *a, unsigned int len);
