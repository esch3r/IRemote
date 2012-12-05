/**
 * @file capcom.h
 * @author Alexander Rössler
 * @brief Capture Compare Library
 * @date 16-10-2012
 */

#ifndef _CAPCOM_H_
#define _CAPCOM_H_

#include <types.h>

int8 initializeCapCom0(uint32 clk, void (* func)(void));

#endif
