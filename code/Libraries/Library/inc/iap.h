/**
 * @file iap.h
 * @author Alexander Rössler
 * @brief Library for controlling the IAP functions
 * @date 12-12-2012
 */

#pragma once
#include <types.h>

void initializeIap(void);
void *getIapPointer(uint32 sector, uint32 offset);
int32 readIap(uint32 sector, uint32 offset, void *buffer, uint32 size);
int32 writeIap(uint32 sector, uint32 offset, const void *buffer, uint32 size);
int32 compareIap(uint32 sector, uint32 offset, const void *buffer, uint32 size);
int32 copyIap(uint32 destinationSector, uint32 sourceSector, uint32 offset, uint32 size);
int32 eraseIap(uint32 sector);
int32 checkBlankIap(uint32 sector);
int32 readSerialIap(void);
int32 readVersionIap(void);
int32 readIdIap(void);
void flashFirmware(const void* data, uint32 size);

int8 saveSettings(void* data, uint32 size);
int8 loadSettings(void* data, uint32 size);
