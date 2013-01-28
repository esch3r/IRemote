/**
 * @file rfm12.h
 * @author Alexander Rössler
 * @brief RFM12 Library
 * @date 28-01-2013
 */
#pragma once

#include <sspi.h>

int8 initializeRfm12(void);

void prepareSendingRfm12(void);
void prepareReceivingRfm12(void);
