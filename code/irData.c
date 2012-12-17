#include "irData.h"

IrCommand *createIrCommand(void)
{
    return (IrCommand *)calloc(sizeof(IrCommand),1);
}

void freeIrCommand(IrCommand *command)
{
    free(command);
}

int8 compareIrCommands(IrCommand *command1, IrCommand *command2)
{
    uint8 i;
    
    if (command1->length != command2->length)   // different lenght => different command
        return 0;
    
    for (i = 0; i < command1->length; i++)
    {
        if (   (command2->data[i] < (command1->data[i]*(1-IR_MAX_TOLERANCE))) 
            || (command2->data[i] > (command1->data[i]*(1+IR_MAX_TOLERANCE))))     //Command is too different
            return 0;
    }
    
    return 1;   // Command is the same
}
