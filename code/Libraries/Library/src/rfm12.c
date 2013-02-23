#include "rfm12.h"

//#define RFM12_WRITEDATA             writeDataSSP0
#define RFXX_WRT_CMD(x)               putcharSsp1(0, x)
//#define RFM12_GETCHAR              // getcharSSP0
#define RFM12_INIT                  initializeSsp1

Rfm12ReceiverSenderMode receiverSenderMode;

int8 initializeRfm12(uint8 id, GpioPair selPair, GpioPair dataPair)
{
    //init ssp
    initializeSsp1(2.5E6, 
               SspDataSize16Bit, 
               SspFrameFormatSPI, 
               SspMasterMode, 
               SspLoopbackDisabled,
               SspSlaveOutputEnabled,
               SspLowClockOutPolarity,
               SspFirstClockOutPhase
              );
    initializeSelSsp1(id, selPair.port, selPair.pin);
    //initializeSelSsp1(1, 0, 26);
    
    setGpioDirection(dataPair.port, dataPair.pin, GpioDirectionInput );   // nFS pin must be set
    setPinMode(dataPair.port, dataPair.pin, PinModeNoPullUpDown);
    
    receiverSenderMode = Rfm12NoMode;
    
    return 0;
}

void prepareOokSendingRfm12(void)
{
    /*RFXX_WRT_CMD(0x80D7);//EL,EF,12.0pF
    RFXX_WRT_CMD(0x8239);//!er,!ebb,ET,ES,EX,!eb,!ew,DC
    RFXX_WRT_CMD(0xA640);//A140=430.8MHz
    RFXX_WRT_CMD(0xC647);//4.8kbps
    RFXX_WRT_CMD(0x94A0);//VDI,FAST,134kHz,0dBm,-103dBm
    RFXX_WRT_CMD(0xC2AC);//AL,!ml,DIG,DQD4
    RFXX_WRT_CMD(0xCA81);//FIFO8,SYNC,!ff,DR
    RFXX_WRT_CMD(0xC483);//@PWR,NO RSTRIC,!st,!fi,OE,EN
    RFXX_WRT_CMD(0x9850);//!mp,9810=30kHz,MAX OUT
    RFXX_WRT_CMD(0xE000);//NOT USE
    RFXX_WRT_CMD(0xC800);//NOT USE
    RFXX_WRT_CMD(0xC400);//1.66MHz,2.2V*/
    
    setRfm12BaseConfig(                 Rfm12DisableTxFifo, 
                                        Rfm12DisableRxFifo, 
                                        Rfm12_433MhzBand, 
                                        Rfm12_12_0pFCrystalLoadCapacitance
                      );
    setRfm12PowerManagement(            Rfm12DisableReceiver,
                                        Rfm12DisableBaseband,
                                        Rfm12DisableTransmitter,
                                        Rfm12DisableSynthesizer,
                                        Rfm12EnableOscillator,
                                        Rfm12DisableBrownoutDetection,
                                        Rfm12DisableWakeupTimer,
                                        Rfm12EnableClockOutput
                           );
    setRfm12Frequency(                  Rfm12_433MhzBand,
                                        433.92
                     );
    setRfm12DataRate(                   4800
    );
    setRfm12ReceiverControl(            Rfm12VdiPin,
                                        Rfm12FastValidDataIndicatorResponse,
                                        Rfm12_134kHzReceiverBasebandBandwidth,
                                        Rfm12_6dBLnaGain,
                                        Rfm12_97dBRssiDetectorThreshold
                           );
    setRfm12DataFilter(                 Rfm12DisableAutoLock,
                                        Rfm12SlowManualLock,
                                        Rfm12DigitalFilter,
                                        Rfm12AlwayOnDqdThreshold,
                                        Rfm12EnableOokModulation
                      );
    setRfm12FifoAndResetMode(           Rfm12ByteFifoInterruptLevel,
                                        Rfm12_2ByteSyncPatternLength,
                                        Rfm12SynchronPatternFillFifo,
                                        Rfm12ClearAndLockFifo,
                                        Rfm12DisableSensitiveReset
                            );
     setRfm12AutomaticFrequencyControl( Rfm12StartupAutoMode,
                                        Rfm12SmallRangeLimit,
                                        Rfm12DisableStrobeEdge,
                                        Rfm12DisableFineMode,
                                        Rfm12EnableAfcOffset,
                                        Rfm12EnableAfc
                                     );
    setRfm12ClockGenerator(             Rfm12WeakClockOutputBuffer,
                                        Rfm12_1msOscillatorStartup,
                                        Rfm12DisablePhaseDetectorDelay,
                                        Rfm12DisablePllDithering,
                                        Rfm12PllBandwith2
                          );
    setRfm12WakeUpTimer(                0,                          // disable wakeuptimer
                                        0
                       );   
    setRfm12LowDutyCycle(               0, 
                                        Rfm12DisableCyclicWakeup    // disable low duty cycle
                        ); 
    setRfm12LowBatteryDetectorAndClockDivider(Rfm12_1_6MHzClock,
                                              225);
}

void prepareOokReceivingRfm12(Rfm12FrequencyBand frequencyBand, float frequency, uint32 dataRate)
{
  /*  RFXX_WRT_CMD(0x80D7);//EL,EF,11.5pF
    RFXX_WRT_CMD(0x82D9);//!er,!ebb,ET,ES,EX,!eb,!ew,DC
    RFXX_WRT_CMD(0xA640);//434MHz
    RFXX_WRT_CMD(0xC647);//4.8kbps
    RFXX_WRT_CMD(0x94A0);//VDI,FAST,134kHz,0dBm,-103dBm
    RFXX_WRT_CMD(0xC2AC);//AL,!ml,DIG,DQD4
    RFXX_WRT_CMD(0xCA81);//FIFO8,SYNC,!ff,DR
    RFXX_WRT_CMD(0xC483);//@PWR,NO RSTRIC,!st,!fi,OE,EN
    RFXX_WRT_CMD(0x9850);//!mp,9810=30kHz,MAX OUT
    RFXX_WRT_CMD(0xE000);//NOT USE
    RFXX_WRT_CMD(0xC800);//NOT USE
    RFXX_WRT_CMD(0xC400);//1.66MHz,2.2V*/
    
    setRfm12BaseConfig(                 Rfm12DisableTxFifo, 
                                        Rfm12DisableRxFifo, 
                                        Rfm12_868MhzBand, 
                                        Rfm12_12_0pFCrystalLoadCapacitance
                      );
    setRfm12PowerManagement(            Rfm12EnableReceiver,
                                        Rfm12EnableBaseband,
                                        Rfm12DisableTransmitter,
                                        Rfm12DisableSynthesizer,
                                        Rfm12DisableOscillator,
                                        Rfm12DisableBrownoutDetection,
                                        Rfm12DisableWakeupTimer,
                                        Rfm12DisableClockOutput
                           );
    setRfm12Frequency(                  frequencyBand, 
                                        frequency
                     );
    setRfm12DataRate(                   dataRate
    );
    setRfm12ReceiverControl(            Rfm12VdiPin,
                                        Rfm12FastValidDataIndicatorResponse,
                                        Rfm12_200kHzReceiverBasebandBandwidth,
                                        Rfm12_6dBLnaGain,
                                        Rfm12_97dBRssiDetectorThreshold
                           );
    setRfm12DataFilter(                 Rfm12DisableAutoLock,
                                        Rfm12SlowManualLock,
                                        Rfm12DigitalFilter,
                                        Rfm12AlwayOnDqdThreshold,
                                        Rfm12EnableOokModulation
                      );
    setRfm12FifoAndResetMode(           Rfm12ByteFifoInterruptLevel,
                                        Rfm12_2ByteSyncPatternLength,
                                        Rfm12SynchronPatternFillFifo,
                                        Rfm12ClearAndLockFifo,
                                        Rfm12DisableSensitiveReset
                            );
    setRfm12AutomaticFrequencyControl(  Rfm12StartupAutoMode,
                                        Rfm12SmallRangeLimit,
                                        Rfm12DisableStrobeEdge,
                                        Rfm12DisableFineMode,
                                        Rfm12EnableAfcOffset,
                                        Rfm12EnableAfc
                                     );
    setRfm12ClockGenerator(             Rfm12WeakClockOutputBuffer,
                                        Rfm12_1msOscillatorStartup,
                                        Rfm12DisablePhaseDetectorDelay,
                                        Rfm12DisablePllDithering,
                                        Rfm12PllBandwith2
                          );
    setRfm12WakeUpTimer(                0,
                                        0
                       );   // disable wakeuptimer
    setRfm12LowDutyCycle(               0, 
                                        Rfm12DisableCyclicWakeup
                        ); // disable low duty cycle
    setRfm12LowBatteryDetectorAndClockDivider(Rfm12_1MHzClock,
                                              225);
}

void setRfm12BaseConfig(Rfm12TxFifoEnable txFifoEnable, 
                        Rfm12RxFifoEnable rxFifoEnable, 
                        Rfm12FrequencyBand frequencyBand, 
                        Rfm12CrystalLoadCapacitance loadCapacitance)
{
    uint16 data;
    data = 0x8000 | (txFifoEnable << 7) 
                  | (rxFifoEnable << 6) 
                  | (frequencyBand << 4) 
                  | (loadCapacitance << 0);
    RFXX_WRT_CMD(data);
}

void setRfm12PowerManagement(Rfm12ReceiverEnable receiverEnable,
                             Rfm12BasebandEnbale basebandEnable,
                             Rfm12TransmitterEnable transmitterEnable,
                             Rfm12SynthesizerEnable synthesizerEnable,
                             Rfm12OscillatorEnable xtalEnable,
                             Rfm12BrownoutDetectionEnable brownoutEnable,
                             Rfm12WakeupTimerEnable wakeupEnable,
                             Rfm12ClockOutputEnable clockEnable)
{
    uint16 data;
    data = 0x8200 | (receiverEnable << 7) 
                  | (basebandEnable << 6) 
                  | (transmitterEnable << 5)
                  | (synthesizerEnable << 4)
                  | (xtalEnable << 3)
                  | (brownoutEnable << 2)
                  | (wakeupEnable << 1)
                  | (clockEnable << 0);
    RFXX_WRT_CMD(data);
    
    if (receiverEnable == Rfm12EnableReceiver)
    {
        receiverSenderMode = Rfm12ReceiverMode;
    }
    else if (transmitterEnable == Rfm12EnableTransmitter)
    {
        receiverSenderMode = Rfm12SenderMode;
    }
    else
    {
        receiverSenderMode = Rfm12NoMode;
    }
}

void setRfm12ClockGenerator(Rfm12ClockOutputBuffer outputBuffer,
                            Rfm12OscillatorLowPowerMode lowPowerXtal,
                            Rfm12PhaseDetectorDelay phaseDelay,
                            Rfm12PllDitheringEnable ditheringEnable,
                            Rfm12PllBandwith bandwith)
{
    uint16 data;
    data = 0xCC00 | (outputBuffer << 5)
                  | (lowPowerXtal << 4)
                  | (phaseDelay << 3)
                  | (ditheringEnable << 2)
                  | (bandwith << 0);
    RFXX_WRT_CMD(data);
}

void setRfm12LowBatteryDetectorAndClockDivider(Rfm12Clock clock,
                                               uint16 thresholdVoltage)
{
    uint16 data;
    uint8 voltageData;
    
    voltageData = (uint8)((thresholdVoltage-225)/10);
    
    data = 0xC000 | (clock << 5)
                | (voltageData << 0);
    RFXX_WRT_CMD(data);
}

void setRfm12Frequency(Rfm12FrequencyBand frequencyBand, float frequency)
{
    uint16 data;
    uint16 frequencyData;
    
    if (frequencyBand == Rfm12_315MhzBand)
    {
        frequencyData = (frequency - 310.0) * 400.0;
    }
    else if (frequencyBand == Rfm12_433MhzBand)
    {
        frequencyData = (frequency - 430.0) * 400.0;
    }
    else if (frequencyBand == Rfm12_868MhzBand)
    {
        frequencyData = (frequency - 860.0) * 200.0;
    }
    else if (frequencyBand == Rfm12_915MhzBand)
    {
        frequencyData = (frequency - 900.0) * 133.3;
    }
    
    data = 0xA000 | (frequencyData << 0);
    RFXX_WRT_CMD(data);
}

void setRfm12DataRate(uint32 baudrate)
{
    uint16 data;
    uint8  clockSelect;
    uint16 rate;
    
    if (baudrate < 2694)
    {
        clockSelect = 1;
        rate = (uint16)((10E6/(29*8))/(baudrate));
    }
    else
    {
        clockSelect = 0;
        rate = (uint16)((10E6/29)/(baudrate));
    }
    
    data = 0xC600 | (clockSelect << 7)
                  | (rate << 0);
    RFXX_WRT_CMD(data);
}

void setRfm12ReceiverControl(Rfm12IntVdiPin pinIntVdi,
                             Rfm12ValidDataIndicatorResponse validDataIndicatorResponse,
                             Rfm12ReceiverBasebandBandwidth receiverBasebandBandwidth,
                             Rfm12LnaGainSelect lnaGainSelect,
                             Rfm12RssiDetectorThreshold rssiDetectorThreshold)
{
    uint16 data;
    data = 0x9000 | (pinIntVdi << 10)
                  | (validDataIndicatorResponse << 8)
                  | (receiverBasebandBandwidth << 5)
                  | (lnaGainSelect << 3)
                  | (rssiDetectorThreshold << 0);
    RFXX_WRT_CMD(data);
}

void setRfm12SynchronPattern(uint8 pattern)
{
    uint16 data;
    data = 0xCE00 | pattern;
    RFXX_WRT_CMD(data);
}

void setRfm12DataFilter(Rfm12AutoLock autoLock,
                        Rfm12ManualLock manualLock,
                        Rfm12Filter filter,
                        Rfm12DqdThreshold dqdThreshold,
                        Rfm12OokModulationEnable ookModulationEnable)
{
    uint16 data;
    data = 0xC200 | (autoLock << 7)
                  | (manualLock << 6)
                  | (filter << 4)
                  | (dqdThreshold << 0)
                  | (1 << 5)
                  | (ookModulationEnable << 3);   // this bit is somehow necessary for the OOK demodulation
    RFXX_WRT_CMD(data);
}

void setRfm12FifoAndResetMode(Rfm12FifoInterruptLevel fifoInterruptLevel,
                              Rfm12SyncPatternLength syncPatternLength,
                              Rfm12FifoFillCondition alwayFill,
                              Rfm12FifoFill fifoFill,
                              Rfm12SensitiveResetEnable sensResetEnable)
{
    uint16 data;
    data = 0xCA00 | (fifoInterruptLevel << 4)
                  | (syncPatternLength << 3)
                  | (alwayFill << 2)
                  | (fifoFill << 1)
                  | (sensResetEnable << 0);
    RFXX_WRT_CMD(data);
}

void setRfm12AutomaticFrequencyControl(Rfm12AutoMode autoMode,
                                       Rfm12RangeLimit rangeLimit,
                                       Rfm12StrobeEdgeEnable strobeEdgeEnable,
                                       Rfm12FineModeEnable fineModeEnable,
                                       Rfm12AfcOffsetEnable afcOffsetEnable,
                                       Rfm12AfcEnable afcEnable)
{
    uint16 data;
    data = 0xC400 | (autoMode << 6)
                  | (rangeLimit << 4)
                  | (strobeEdgeEnable << 3)
                  | (fineModeEnable << 2)
                  | (afcOffsetEnable << 1)
                  | (afcEnable << 0);
    RFXX_WRT_CMD(data);
}

void setRfm12TxConfiguration(Rfm12ModulationPolarity modulationPolarity,
                             Rfm12FrequencyDeviation frequencyDeviation,
                             Rfm12RelativeOutputPower relativeOutputPower)
{
    uint16 data;
    data = 0x9800 | (modulationPolarity << 8)
                  | (frequencyDeviation << 4)
                  | (relativeOutputPower << 0);
    RFXX_WRT_CMD(data);
}

void setRfm12WakeUpTimer(uint8 radix, uint8 mantissa)
{
    uint16 data;
    data = 0xE000 | (radix << 8) | (mantissa << 0);
    RFXX_WRT_CMD(data);
}

void setRfm12LowDutyCycle(uint8 duration,
                          Rfm12CyclicWakeupEnable cyclicWakeupEnable)
{
    uint16 data;
    data = 0xC800 | (duration << 1)
                  | (cyclicWakeupEnable << 0);
    RFXX_WRT_CMD(data);
}

void actionRfm12SoftwareReset()
{
    uint16 data = 0xFE00;
    RFXX_WRT_CMD(data);
}

uint16 readRfm12Status()
{
    uint16 writeData = 0x000;
    uint16 readData;
    
    readWriteSsp1(1,writeData, &readData);
    
    return readData;
}

char getcharRfm12()
{
    uint16 writeData = 0xB000;
    uint16 readData;
    
    readWriteSsp1(1,writeData, &readData);
    
    return (char)(readData & (0x0F));
}

void putcharRfm12(char byte)
{
    uint16 data;
    data = 0xB800 | (byte << 0);
    RFXX_WRT_CMD(data);
}
