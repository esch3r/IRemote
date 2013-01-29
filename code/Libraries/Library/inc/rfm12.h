/**
 * @file rfm12.h
 * @author Alexander Rössler
 * @brief RFM12 Library
 * @date 28-01-2013
 */
/* Device group ----------------------------------------------------------- */
/** @addtogroup Device
 * @{
 */

/** @defgroup RFM12 RFM12 Library
 * @ingroup Device
 * @{
 */
#pragma once

#include <ssp.h>

int8 initializeRfm12(void);

void prepareSendingRfm12(void);
void prepareReceivingRfm12(void);

/**
 * @}
 */

/**
 * @}
 */
