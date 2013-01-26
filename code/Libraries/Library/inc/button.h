#ifndef _BUTTONLIB_H_
#define _BUTTONLIB_H_

#include <types.h>
#include <timer.h>

#define maxunset 	4

/** Initizes the Button Pin und Ports.
 *@param Mhz Is the frequency in wich values are checked.
 *@param ID  Stands for the name of the Buttons(B1,B2,B3,..).
 *@param Port Is the port of the button.
 *@param Pin  Is the pin of the button.
 *@param Type set "0" for low_active, or" 1" for high_active
 */
uint8 initializeButton(uint8 khz,uint8 ID, uint8 Port, uint8 Pin,uint8 Type);

/** reads the button values
 */
void valueButton(void);

/** stores the values and counts the matches.
 *@param i is the number of the row of buttons.
 *@param var is the value of the button.
 */
int8 putVal(uint8 i,uint8 var);

#endif

/** ButtonLib.h */
