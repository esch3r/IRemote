 #ifndef _BUTTONLIB_H_
#define _BUTTONLIB_H_

#include <types.h>
#include <timer.h>

uint8 initializeButton(uint8 Mhz,uint8 ID, uint8 Port, uint8 Pin);
void valueButton(void);
int8 putVal(uint8 i,uint8 var);

#endif

/** ButtonLib.h */
