#include <button.h>

CircularBuffer buttonBuffer;

typedef struct {
    uint8 id;
    uint8 port;
    uint8 pin;
    uint8 unset;
    uint8 type;
} Button;

Button	buttons[20];
ButtonValue val[20];
uint8	buttonCount = 0;

uint32 maxunset;

/** reads the button values
 */
void valueButton(void);

/** stores the values and counts the matches.
 *@param i is the number of the row of buttons.
 *@param var is the value of the button.
 */
void putVal(uint8 i, uint8 pressed);

uint8 initializeButtons(uint32 khz, uint32 sampleInterval, uint32 timeoutInterval)
{
    if (initializeCb(&buttonBuffer,20,sizeof(ButtonValue)) == -1)
        return -1;
    
	if (initializeTimer2(khz, sampleInterval) == -1)
        return -1;
    
    maxunset = (uint32)(timeoutInterval/sampleInterval);
    
    connectFunctionTimer2(valueButton);
    startTimer2();

    return 0;
}

void initializeButton(uint8 id, uint8 port, uint8 pin,ButtonType type)
{
    setGpioDirection(port, pin, GpioDirectionInput );    //direction 0=input
    if(type == ButtonTypeLowActive)
      setPinMode(port, pin, PinModePullUp);
    if(type == ButtonTypeHighActive)
       setPinMode(port, pin, PinModePullDown);
    
    buttons[buttonCount].id     = id;
    buttons[buttonCount].port   = port;
    buttons[buttonCount].pin    = pin;
    buttons[buttonCount].type   = type;
    buttons[buttonCount].unset  = 0;
    
    val[buttonCount].count = 0;
    val[buttonCount].id = id;

    buttonCount++;
}

void valueButton(void){
	uint8 i;
    
	for(i = 0; i < buttonCount; i++)
    {
        if(buttons[i].type == ButtonTypeLowActive)
        {
            readGpio(buttons[i].port,buttons[i].pin) ? putVal(i,0): putVal(i,1);
        }
        else if(buttons[i].type == ButtonTypeLowActive)
        {
            readGpio(buttons[i].port,buttons[i].pin) ? putVal(i,1): putVal(i,0);
        }
	}
}

void putVal(uint8 i,uint8 pressed)
{
    
	if(pressed && (buttons[i].unset != 0))
    {
		val[i].count = val[i].count +1;
        buttons[i].unset = 0;
	}
	else
    {
		if(!pressed && buttons[i].unset <= maxunset) 
        {
            buttons[i].unset = buttons[i].unset +1;
        }
		if((buttons[i].unset > maxunset) && (val[i].count != 0) )
        {
			putCb(&buttonBuffer,(void*)(&val[i]));
			val[i].count = 0;
			buttons[i].unset =0;
		}
	}
}

int8 getButtonPress(ButtonValue *value)
{
    return getCb(&buttonBuffer, (void*)value);
}
