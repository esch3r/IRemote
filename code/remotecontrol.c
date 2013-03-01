#include "remotecontrol.h"

volatile RemoteControl_Medium currentMedium;

volatile uint8   frameReceived = 0;
volatile uint8   firstCapture = 1;
volatile uint8   currentPosition = 0;
volatile uint8   commandRunning = 0;
CircularBuffer   commandBuffer;
volatile uint16  commandBufferData[REMOTE_COMMAND_MAX_TRANSITIONS];

volatile uint8   repeatCount;
volatile uint8   currentRepeatCount = 0;

volatile uint32 receiveTimeout;
volatile uint32 sendTimeout;

volatile uint32 irReceiveTimeout = 20000;
volatile uint32 irSendTimeout = 50000;
volatile uint8  irRepeatCount = 4;

volatile uint32 radio433MhzReceiveTimeout = 9000;
volatile uint32 radio433MhzSendTimeout = 10000;
volatile uint8  radio433MhzRepeatCount = 5;

volatile uint32 radio868MhzReceiveTimeout = 9000;
volatile uint32 radio868MhzSendTimeout = 10000;
volatile uint8  radio868MhzRepeatCount = 5;

RemoteCommand *tmpCommand = NULL;


int8 saveFrame(void);
void captureFunction(void);
void runFunction(void);

int8 RemoteControl_initialize(void)
{
    if (Cb_initialize(&commandBuffer, REMOTE_COMMAND_MAX_TRANSITIONS, sizeof(uint16), (void*)(&commandBufferData)) == -1)
        return -1;
    
    Gpio_setDirection(1, 18, GpioDirectionOutput);  // PWM
    setPinMode(1,18, PinModePullDown);
    
    if (Pwm_initialize(38000, 0.5, IrPwm) == -1)
        return -1;
        
    Gpio_setDirection(IrCapturePort, IrCapturePin, GpioDirectionInput); // TSOP input pin
    
    GpioPair selPair;
    GpioPair dataPair;
    
    // Initialize 433MHz module
    selPair.port = 0;
    selPair.pin = 25;
    dataPair.port = 2;
    dataPair.pin = 2;
    Rfm12_initialize(Rfm12_0, Ssp1, selPair, dataPair);
    //Rfm12_prepareOokReceiving(Rfm12_0, Rfm12_FrequencyBand433Mhz, 433.92, 4200);
    
    // Initialize 868MHz module
    selPair.port = 0;
    selPair.pin = 26;
    dataPair.port = 0;
    dataPair.pin = 22;
    Rfm12_initialize(Rfm12_1, Ssp1, selPair, dataPair);

    if (Timer_initialize(Timer3, 1000,1E9) == -1)
        return -1;
    
    return 0;
}

void RemoteControl_outputCommand(RemoteCommand *command)
{
    if (frameReceived == 1)
    {
        uint16 commandSize = sizeof(RemoteCommand);
        uint16 i;
        
        printfData("*DATA");
        for (i = 0; i < commandSize; i++)
        {
            printfData("%02x",(uint8)(((char*)command)[i]));
        }
        printfData("\r");
    }
}

RemoteCommand* RemoteControl_command(void)
{
    if (frameReceived == 1)
        return tmpCommand;
    else
        return NULL;
}

void RemoteControl_startCapture(RemoteControl_Medium medium)
{
    if (tmpCommand != NULL)
        RemoteCommand_free(tmpCommand);
    
    tmpCommand = RemoteCommand_create();
    frameReceived = 0;
    firstCapture = 1;
    currentPosition = 0;
    
    currentMedium = medium;
    
    if (medium == RemoteControl_Medium_Ir)
    {
        receiveTimeout = irReceiveTimeout;
        
        Gpio_enableInterrupt(IrCapturePort, 
                             IrCapturePin, 
                             GpioInterruptFallingAndRisingEdge, 
                             &captureFunction);
    }
    else if (medium == RemoteControl_Medium_433Mhz)
    {
        receiveTimeout = radio433MhzReceiveTimeout;
        
        Rfm12_prepareOokReceiving(Rfm12_0, Rfm12_FrequencyBand433Mhz, 433.92, 1000);
        Timer_delayMs(100);
        Gpio_enableInterrupt(Radio433MhzCapturePort, 
                             Radio433MhzCapturePin, 
                             GpioInterruptFallingAndRisingEdge, 
                             &captureFunction);
    }
    else if (medium == RemoteControl_Medium_868Mhz)
    {
        receiveTimeout = radio868MhzReceiveTimeout;
        
        Rfm12_prepareOokReceiving(Rfm12_1, Rfm12_FrequencyBand868Mhz, 868, 4200);
        Gpio_enableInterrupt(Radio868MhzCapturePort, 
                             Radio868MhzCapturePin, 
                             GpioInterruptFallingAndRisingEdge, 
                             &captureFunction);
    }
    
    Timer_connectFunction(Timer3, NULL);
    Timer_setIntervalMs(Timer3, 1E6);
    Timer_reset(Timer3);
}

void RemoteControl_stopCapture(void)
{
    Timer_stop(Timer3);
    
    if (currentMedium == RemoteControl_Medium_Ir)
    {
        Gpio_disableInterrupt(IrCapturePort, IrCapturePin);
    }
    else if (currentMedium == RemoteControl_Medium_433Mhz)
    {
        Rfm12_ookClear(Rfm12_0);    // workaround
        Gpio_disableInterrupt(Radio433MhzCapturePort, Radio433MhzCapturePin);
    }
    else if (currentMedium == RemoteControl_Medium_868Mhz)
    {
        Rfm12_ookClear(Rfm12_1);    // workaround
        Gpio_disableInterrupt(Radio868MhzCapturePort, Radio868MhzCapturePin);
    }
    
    Led_blink2(Led3);
}

void captureFunction(void)
{
    static uint16 timeDiff;
    
    timeDiff = (uint16) Timer_counterValue(Timer3);

    if (!firstCapture)
    {
        if (timeDiff >= receiveTimeout)     // Detected a timeout => frameReceived
        {
           /* if ((saveFrame() == -1) || (frameReceived != 1))
            {
                firstCapture = 1;
            }
            else
            {
                RemoteControl_stopCapture();
                return;
            }*/
           
           if (currentPosition > 0)
           {
               tmpCommand->length = currentPosition;
               tmpCommand->medium = currentMedium;
               frameReceived = true;
               RemoteControl_stopCapture();
               return;
           }
           else
           {
               firstCapture = 1;
           }
        }
        else if (timeDiff > 200)
        {
            tmpCommand->data[currentPosition] = timeDiff;
            
            currentPosition++;
            if (currentPosition == REMOTE_COMMAND_MAX_TRANSITIONS)
            {
                currentPosition = 0;
            }
            //Cb_put(&commandBuffer, (void*)(&timeDiff));
        }
    }
    else
    {
        firstCapture = 0;
    }
    
    Timer_reset(Timer3);                      // Reset the timer
    
    Led_toggle(Led2);   // visual feedback
}

int8 saveFrame(void)
{
    uint16 item;
    uint8 i = 0;
        
    while (Cb_get(&commandBuffer,&item) == 0)
    {
        if (item == 0)  //remove the 0 from starting
            continue;
        tmpCommand->data[i] = item;
        i++;
    }
    
    if (i > 0)
    {
        tmpCommand->length = i;
        tmpCommand->medium = currentMedium;
        
        frameReceived = 1;
        return 0;
    }
    
    return -1;
}

void RemoteControl_runCommand(RemoteCommand* command)
{
    tmpCommand = command;
    currentMedium = command->medium;
    currentPosition = 0;
    commandRunning = 1;     // Set the variable which indicates that a command is running
    
    if (currentMedium == RemoteControl_Medium_Ir)
    {
        sendTimeout = irSendTimeout;
        repeatCount = irRepeatCount;
    }
    else if (currentMedium == RemoteControl_Medium_433Mhz)
    {
        sendTimeout = radio433MhzSendTimeout;
        repeatCount = radio433MhzRepeatCount;
        
        Rfm12_prepareOokSending(Rfm12_0, Rfm12_FrequencyBand433Mhz, 433.92, 4200);
    }
    else if (currentMedium == RemoteControl_Medium_868Mhz)
    {
        sendTimeout = radio868MhzSendTimeout;
        repeatCount = radio868MhzRepeatCount;
        
        Rfm12_prepareOokSending(Rfm12_1, Rfm12_FrequencyBand868Mhz, 868, 4200);
    }
    
    Timer_connectFunction(Timer3, &runFunction);
    Timer_setIntervalUs(Timer3, 10);
    Timer_start(Timer3);
}

void RemoteControl_stopCommand(void )
{
    Timer_stop(Timer3);
    
    if (currentMedium == RemoteControl_Medium_Ir)
    {
        Pwm_stop(IrPwm);
    }
    else if (currentMedium == RemoteControl_Medium_433Mhz)
    {
        Rfm12_ookClear(Rfm12_0);
    }
    else if (currentMedium == RemoteControl_Medium_868Mhz)
    {
        Rfm12_ookClear(Rfm12_1);
    }
    
    currentRepeatCount++;
    
    if (currentRepeatCount == repeatCount)  // Repeat the command a few times
    {
        commandRunning = 0;
        currentRepeatCount = 0;
    }
    else
    {
        currentPosition = 0;
        Timer_setIntervalUs(Timer3, sendTimeout);    // Timeout between commands
        Timer_start(Timer3);
    }
}

void runFunction(void )
{
    if (currentMedium == RemoteControl_Medium_Ir)
    {
        Pwm_toggle(IrPwm);
    }
    else if (currentMedium == RemoteControl_Medium_433Mhz)
    {
        Rfm12_ookToggle(Rfm12_0);
    }
    else if (currentMedium == RemoteControl_Medium_868Mhz)
    {
        Rfm12_ookToggle(Rfm12_1);
    }
    
    Led_toggle(Led2);
    
    if (currentPosition < tmpCommand->length)
    {
        Timer_setIntervalUs(Timer3, tmpCommand->data[currentPosition]);

        currentPosition++;
    }
    else
    {
        RemoteControl_stopCommand();
        Led_clear(Led2);
    }
}

uint8 RemoteControl_isCommandRunning(void )
{
    return commandRunning;
}

void RemoteControl_setReceiveTimeout(RemoteControl_Medium medium, uint32 timeout)
{
    if (medium == RemoteControl_Medium_Ir)
    {
        irReceiveTimeout = timeout;
    }
    else if (medium == RemoteControl_Medium_433Mhz)
    {
        radio433MhzReceiveTimeout = timeout;
    }
    else if (medium == RemoteControl_Medium_868Mhz)
    {
        radio868MhzReceiveTimeout = timeout;
    }
}

void RemoteControl_setSendTimeout(RemoteControl_Medium medium, uint32 timeout)
{
    if (medium == RemoteControl_Medium_Ir)
    {
        irSendTimeout = timeout;
    }
    else if (medium == RemoteControl_Medium_433Mhz)
    {
        radio433MhzSendTimeout = timeout;
    }
    else if (medium == RemoteControl_Medium_868Mhz)
    {
        radio868MhzSendTimeout = timeout;
    }
}

void RemoteControl_setRepeatCount(RemoteControl_Medium medium, uint8 count)
{
    if (medium == RemoteControl_Medium_Ir)
    {
        irRepeatCount = count;
    }
    else if (medium == RemoteControl_Medium_433Mhz)
    {
        radio433MhzRepeatCount = count;
    }
    else if (medium == RemoteControl_Medium_868Mhz)
    {
        radio868MhzRepeatCount = count;
    }
}
