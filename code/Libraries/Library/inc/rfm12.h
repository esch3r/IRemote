/**
 * @file rfm12.h
 * @author Alexander Rössler
 * @brief RFM12 Library
 * @date 28-01-2013
 */
/** @defgroup RFM12 RFM12 Library
 * @ingroup Device
 * @{
 */
#pragma once

#include <ssp.h>
#include <gpio.h>

typedef enum {
    Rfm12EnableTxFifo = 0,
    Rfm12DisableTxFifo = 1
} Rfm12TxFifoEnable;
    
typedef enum {
    Rfm12EnableRxFifo = 0,
    Rfm12DisableRxFifo = 1
} Rfm12RxFifoEnable;

typedef enum {
    Rfm12_315MhzBand = 0b00,
    Rfm12_433MhzBand = 0b01,
    Rfm12_868MhzBand = 0b10,
    Rfm12_915MhzBand = 0b11
} Rfm12FrequencyBand;

typedef enum {
    Rfm12_85pFCrystalLoadCapacitance = 0b0000
} Rfm12CrystalLoadCapacitance;

int8 initializeRfm12(uint8 id, GpioPair selPair, GpioPair dataPair);

void prepareSendingRfm12(void);
void prepareReceivingRfm12(void);

void setRfm12BaseConfig(Rfm12TxFifoEnable txFifoEnable, 
                        Rfm12RxFifoEnable rxFifoEnable, 
                        Rfm12FrequencyBand frequencyBand, 
                        Rfm12CrystalLoadCapacitance loadCapacitance);

/**
 * @}
 */
