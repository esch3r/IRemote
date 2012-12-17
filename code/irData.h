/**
 * @file irData.h
 * @author Alexander Rössler
 * @brief Contains anything having to do with the data handling of IRemote
 * @date 13-12-2012
 */

#pragma once

#define IR_MAX_TRANSITIONS 100
#define IR_MAX_TOLERANCE   0.1

#include <stdio.h>
#include <stdlib.h>
#include <types.h>

typedef struct {
    uint16  data[IR_MAX_TRANSITIONS];
    uint8   length;
    uint16  id;
} IrCommand;

/** Creates an empty IR-Command
 *  @return Pointer to the new created command, return NULL on failure.
 */
IrCommand* createIrCommand(void);
/** Frees the memory of an IR-Command.
 *  @param command Pointer to the command that should be freed.
 */
void freeIrCommand(IrCommand *command);
/** Compares 2 IR-Commands, it is not necessare that the commands are 100% equal (see MAX_TOLERANCE).
 *  @param command1 Command 1
 *  @param command2 Command 2
 *  @return 1 = euqal, 0 unequal
 */
int8 compareIrCommands(IrCommand *command1, IrCommand *command2);
