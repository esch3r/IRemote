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

uint8 initializeButton(uint8 khz,uint8 id, uint8 port, uint8 pin,uint8 type){

	setGpioDirection(port, pin, 0 );	//direction 0=input
	if(type==0)
	  setPinMode(port, pin, PinModePullUp);
	if(type==1)
	   setPinMode(port, pin, PinModePullDown);
	(buttons[buttonCount]).id   = id;
	buttons[buttonCount].port   = port;
	buttons[buttonCount].pin    = pin;
	buttons[buttonCount].type   = type;

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

	if(var && buttons[i].unset!=0){
        printfData("button pressed\r");   
		val[i].id=buttons[i].id;
		val[i].count = val[i].count +1;
        buttons[i].unset = 0;
	}
	else{
		if(!var && buttons[i].unset <= maxunset) buttons[i].unset = buttons[i].unset +1;
		if(buttons[i].unset > maxunset && val[i].count != 0 ){
			putCb(&buttonBuffer,&val[i]);
			val[i].count = 0;
			buttons[i].unset =0;
		}
	}
return 0;
}
