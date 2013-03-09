#include "remotecommand.h"

uint32 commandId = 0u;

void RemoteCommand_initialize(RemoteCommand *command)
{
    command->version = 0u;
    command->id = commandId;
    command->medium = 0u;
    command->frequency = 0u;
    command->length = 0u;
    
    commandId++;
}

bool RemoteCommand_compare(RemoteCommand *command1, RemoteCommand *command2)
{
    uint8 i;
    
    if (command1->length != command2->length)   // different lenght => different command
        return false;
    
    if (command1->frequency != command2->frequency)
        return false;
    
    
    for (i = 0; i < command1->length; i++)
    {
        if (   (command2->data[i] < (command1->data[i]*(1-REMOTE_COMMAND_MAX_TOLERANCE))) 
            || (command2->data[i] > (command1->data[i]*(1+REMOTE_COMMAND_MAX_TOLERANCE))))     //Command is too different
            return false;
    }
    
    return true;   // Command is the same
}
