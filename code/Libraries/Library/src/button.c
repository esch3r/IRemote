#include <button.h>
#include <gpio.h>
#include <pincon.h>
#include <circularbuffer.h>

int32 Puffer[20];
int32 *p_Puffer;

CircularBuffer buttonBuffer;


typedef struct {
    uint8 id;
    uint8 port;
    uint8 pin;
    uint8 unset;
    uint8 type;
} Button;

typedef struct {
	uint8 id;
	uint8 count;
} ButtonValue;

Button	buttons[10];
ButtonValue val[20];
uint8	buttonCount = 0;

uint8 initializeButton(uint8 khz,uint8 ID, uint8 Port, uint8 Pin,uint8 Type){

	setGpioDirection(Port, Pin, 0 );	//direction 0=input
	if(Type==0)
	  setPinMode(Port, Pin, PinModePullUp); 
	if(Type==1)
	   setPinMode(Port, Pin, PinModePullDown);
	(buttons[buttonCount]).id   = ID;
	buttons[buttonCount].port   = Port;
	buttons[buttonCount].pin    = Pin;
	buttons[buttonCount].type   =Type;

	buttonCount++;

	initializeTimer2(khz, 100);
	initializeCb(&buttonBuffer,20,sizeof(uint8));


singleShotTimer2(10, valueButton);

return 0;
}

void valueButton(void){
	uint8 i;
	for(i=0;i<=buttonCount;i++){
	if(buttons[i].type==0)
	  readGpio(buttons[i].port,buttons[i].pin) ? putVal(i,0): putVal(i,1);
	if(buttons[i].type==1)
	  readGpio(buttons[i].port,buttons[i].pin) ? putVal(i,1): putVal(i,0);
	}
}

int8 putVal(uint8 i,uint8 var){

	if(var){
		val[i].id=buttons[i].id;
		val[i].count = val[i].count +1;
	}
	else{
		if(buttons[i].unset <= maxunset) buttons[i].unset = buttons[i].unset +1;
		else{
			putCb(&buttonBuffer,&val[i]);
			val[i].count = 0;
			buttons[i].unset =0;
		}
	}
return 0;
}
