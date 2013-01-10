#include <button.h>
#include <gpio.h>
#include <circularbuffer.h>

int32 Puffer[20];
int32 *p_Puffer;

CircularBuffer buttonBuffer;


typedef struct {
	uint8 id;
	uint8 port;
	uint8 pin;
	uint8 unset;
} Button;

typedef struct {
	uint8 id;
	uint8 count;
} ButtonValue;

Button	buttons[10];
ButtonValue val[20];
int8	buttonCount = 0;

uint8 initializeButton(uint8 Mhz,uint8 ID, uint8 Port, uint8 Pin){

	setGpioDirection(Port, Pin, 0 );	//direction 0=input
	(buttons[buttonCount]).id = ID;
	buttons[buttonCount].port = Port;
	buttons[buttonCount].pin = Pin;

	buttonCount++;

	initializeTimer2(Mhz, 100);
	initializeCb(&buttonBuffer,20,sizeof(uint8));

//singleShotTimer2(uint32 ms, void (* func)(void));
singleShotTimer2(Mhz, valueButton);

return 0;
}

void valueButton(void){
	uint8 i;
	for(i=0;i<=buttonCount;i++){


	readGpio(buttons[i].port,buttons[i].pin) ? putVal(i,1): putVal(i,0);
	}
}

int8 putVal(uint8 i,uint8 var){

	if(var){
		val[i].id=buttons[i].id;
		val[i].count = &val[i].count +1;
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
