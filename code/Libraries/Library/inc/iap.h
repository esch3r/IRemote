/**
 * @file iap.h
 * @author Alexander Rössler
 * @brief Library for controlling the IAP functions
 * @date 12-12-2012
 */
/** @defgroup IAP IAP (In Application Programming)
 * @ingroup Peripheral
 * @{
 */
#pragma once
#include <types.h>
#include <string.h>
#include <LPC17xx.h>

enum IapStatusCodes {
    IapStatusCommandSuccess = 0,
    IapStatusInvalidCommand = 1,
    IapStatusSourceAddressError = 2,
    IapStatusDestinationAddressError = 3,
    IapStatusSourceAddressNotMapped = 4,
    IapStatusDestinationAddressNotMapped = 5,
    IapStatusCountError = 6,
    IapStatusInvalidSector = 7,
    IapStatusSectorNotBlank = 8,
    IapStatusSectorNotPreparedForWriteOperation = 9,
    IapStatusCompareError = 10,
    IapStatusBusy = 11
};

/** Returns a pointer to the specific flash bank, this function is mostly for
 *  internal use.
 *  @param sector Specific sector in the flash memory.
 *  @param offset Offset from the beginning of the sector.
 *  @return Pointer to an address in the flash memory.
 */
void *getIapPointer(uint32 sector, uint32 offset);
/** Reads data from the flash memory.
 *  @param sector Specific sector in the flash memory.
 *  @param offset Offset from the beginning of the sector.
 *  @param buffer Pointer to the address where the data has to be stored.
 *  @param size   Size of the data to read.
 *  @return  Returns -1 on error or one of the IapStatusCodes if successful.
 */
int32 readIap(uint32 sector, uint32 offset, void *buffer, uint32 size);
/** Writes data to the flash memory.
 *  @param sector Specific sector in the flash memory.
 *  @param offset Offset from the beginning of the sector.
 *  @param buffer Pointer to the address where the data is stored.
 *  @param size   Size of the data to write.
 *  @return  Returns -1 on error or one of the IapStatusCodes if successful.
 */
int32 writeIap(uint32 sector, uint32 offset, const void *buffer, uint32 size);
/** Compares data from the flash memory with data in the RAM.
 *  @param sector Specific sector in the flash memory.
 *  @param offset Offset from the beginning of the sector.
 *  @param buffer Pointer to the address where the data is stored.
 *  @param size   Size of the data to compare.
 *  @return  Returns -1 on error or one of the IapStatusCodes if successful.
 */
int32 compareIap(uint32 sector, uint32 offset, const void *buffer, uint32 size);
/** Writes data to the flash memory.
 *  @param destinationSector Destination sector in the flash memory.
 *  @param sourceSector Source sector in the flash memory.
 *  @param offset Offset from the beginning of the sector.
 *  @param size   Size of the data to copy.
 *  @return  Returns -1 on error or one of the IapStatusCodes if successful.
 */
int32 copyIap(uint32 destinationSector, uint32 sourceSector, uint32 offset, uint32 size);
/** Erases one sector in the flash memory
 *  @param sector The sector the erase.
 *  @return  Returns -1 on error or one of the IapStatusCodes if successful.
 */
int32 eraseIap(uint32 sector);
/** Checks wheter an sector in the fash memory is empty or not.
 *  @param sector The sector the check.
 *  @return  Returns -1 on error or one of the IapStatusCodes if successful.
 */
int32 checkBlankIap(uint32 sector);

/** Reads the serial number of the device from the flash memory.
 *  @return Serialnumber
 **/
int32 readSerialIap(void);
/** Reads the version number of the IAP code from the flash memory.
 * @return Version number
 */
int32 readVersionIap(void);
/** Reads the id of the IAP code from the flash memory.
 * @return Id
 */
int32 readIdIap(void);

/** Work in progress */
void  flashFirmware(const void* data, uint32 size);

/** Saves application data to the flash memory. Sector 27 is used for this purpose.
 * @param Pointer to the data in RAM.
 * @param size Size of the data.
 * @return Return -1 on error or one of the IapStatusCodes if sucsessful.
 */
int8  saveSettings(void* data, uint32 size);
/** Loads application form the flash memory. Sector 27 is used for this purpose.
 * @param Pointer to the data in RAM.
 * @param size Size of the data.
 * @return Return -1 on error or one of the IapStatusCodes if sucsessful.
 */
int8  loadSettings(void* data, uint32 size);

/**
 * @}
 */
