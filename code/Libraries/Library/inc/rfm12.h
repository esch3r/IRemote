/**
 * @file rfm12.h
 * @author Alexander Rössler
 * @brief RFM12 Library
 * @date 28-01-2013
 */
/** @defgroup RFM12 RFM12 Library
 * @ingroup Device
 * @{
 */
#pragma once

#include <ssp.h>
#include <gpio.h>
#include <pincon.h>
#include <sys/types.h>

typedef enum {
    Rfm12TxFifoEnabled = 1,
    Rfm12TxFifoDisabled = 0
} Rfm12TxFifo;
    
typedef enum {
    Rfm12RxFifoEnabled = 1,
    Rfm12RxFifoDisabled = 0
} Rfm12RxFifo;

typedef enum {
    Rfm12FrequencyBand315Mhz = 0b00,
    Rfm12FrequencyBand433Mhz = 0b01,
    Rfm12FrequencyBand868Mhz = 0b10,
    Rfm12FrequencyBand915Mhz = 0b11
} Rfm12FrequencyBand;

typedef enum {
    Rfm12CrystalLoadCapacitance8_5pF = 0b0000,
    Rfm12CrystalLoadCapacitance9_0pF = 0b0001,
    Rfm12CrystalLoadCapacitance9_5pF = 0b0010,
    Rfm12CrystalLoadCapacitance10_0pF = 0b0011,
    Rfm12CrystalLoadCapacitance10_5pF = 0b0100,
    Rfm12CrystalLoadCapacitance11_0pF = 0b0101,
    Rfm12CrystalLoadCapacitance11_5pF = 0b0110,
    Rfm12CrystalLoadCapacitance12_0pF = 0b0111,
    Rfm12CrystalLoadCapacitance12_5pF = 0b1000,
    Rfm12CrystalLoadCapacitance13_0pF = 0b1001,
    Rfm12CrystalLoadCapacitance13_5pF = 0b1010,
    Rfm12CrystalLoadCapacitance14_0pF = 0b1011,
    Rfm12CrystalLoadCapacitance14_5pF = 0b1100,
    Rfm12CrystalLoadCapacitance15_0pF = 0b1101,
    Rfm12CrystalLoadCapacitance15_5pF = 0b1110,
    Rfm12CrystalLoadCapacitance16_0pF = 0b1111
} Rfm12CrystalLoadCapacitance;

typedef enum {
    Rfm12ReceiverEnabled = 1,
    Rfm12ReceiverDisabled = 0
} Rfm12Receiver;

typedef enum {
    Rfm12BasebandEnabled = 1,
    Rfm12BasebandDisabled = 0
} Rfm12Baseband;

typedef enum {
    Rfm12TransmitterEnabled = 1,
    Rfm12TransmitterDisabled = 0
} Rfm12Transmitter;

typedef enum {
    Rfm12SynthesizerEnabled = 1,
    Rfm12SynthesizerDisabled = 0
} Rfm12Synthesizer;

typedef enum {
    Rfm12OscillatorEnabled = 1,
    Rfm12OscillatorDisabled = 0
} Rfm12Oscillator;

typedef enum {
    Rfm12BrownoutDetectionEnabled = 1,
    Rfm12BrownoutDetectionDisabled = 0
} Rfm12BrownoutDetection;

typedef enum {
    Rfm12WakeupTimerEnabled = 1,
    Rfm12WakeupTimerDisabled = 0
} Rfm12WakeupTimer;

typedef enum {
    Rfm12ClockOutputEnabled = 0,
    Rfm12ClockOutputDisabled = 1
} Rfm12ClockOutput;

typedef enum {
    Rfm12ClockOutputBufferWeak = 0b00,   /** < 2.5MHz */
    Rfm12ClockOutputBufferMedium = 0b10, /** 3.3MHz */
    Rfm12ClockOutputBufferStrong = 0b11  /** 5 to 10MHz */
} Rfm12ClockOutputBuffer;

typedef enum {
    Rfm12OsciallatorLowPowerMode1msStartup = 0,
    Rfm12OsciallatorLowPowerMode2msStartup = 1
} Rfm12OscillatorLowPowerMode;

typedef enum {
    Rfm12PhaseDetectorDelayDisabled = 0,
    Rfm12PhaseDetectorDelayEnabled = 1
} Rfm12PhaseDetectorDelay;

typedef enum {
    Rfm12PllDitheringEnabled = 0,
    Rfm12PllDitheringDisabled = 1
} Rfm12PllDithering;

typedef enum {
    Rfm12PllBandwith1 = 0x10,
    Rfm12PllBandwith2 = 0x11
} Rfm12PllBandwith;

typedef enum {
    Rfm12Clock1MHz = 0b000,
    Rfm12Clock1_25MHz = 0b001,
    Rfm12Clock1_6MHz = 0b010,
    Rfm12Clock2MHz = 0b011,
    Rfm12Clock2_5MHz = 0b100,
    Rfm12Clock3_3MHz = 0b101,
    Rfm12Clock5MHz = 0b110,
    Rfm12Clock10MHz = 0b111
} Rfm12Clock;

typedef enum {
    Rfm12DataPinInt = 0,
    Rfm12DataPinVdi = 1
} Rfm12DataPin;

typedef enum {
    Rfm12ValidDataIndicatorResponseFast = 0b00,
    Rfm12ValidDataIndicatorResponseMedium = 0b01,
    Rfm12ValidDataIndicatorResponseSlow = 0b10,
    Rfm12ValidDataIndicatorResponseAlwaysOn = 0b11
} Rfm12ValidDataIndicatorResponse;

typedef enum {
    Rfm12ReceiverBasebandBandwidth400kHz = 0b001,
    Rfm12ReceiverBasebandBandwidth340kHz = 0b010,
    Rfm12ReceiverBasebandBandwidth270kHz = 0b011,
    Rfm12ReceiverBasebandBandwidth200kHz = 0b100,
    Rfm12ReceiverBasebandBandwidth134kHz = 0b101,
    Rfm12ReceiverBasebandBandwidth67kHz = 0b110
} Rfm12ReceiverBasebandBandwidth;

typedef enum {
    Rfm12LnaGain0dB = 0b00,
    Rfm12LnaGain6dB = 0b01,
    Rfm12LnaGain14dB = 0b10,
    Rfm12LnaGain20dB = 0b11
} Rfm12LnaGain;

typedef enum {
    Rfm12RssiDetectorThreshold103dB = 0b000,
    Rfm12RssiDetectorThreshold97dB = 0b001,
    Rfm12RssiDetectorThreshold91dB = 0b010,
    Rfm12RssiDetectorThreshold85dB = 0b011,
    Rfm12RssiDetectorThreshold79dB = 0b100,
    Rfm12RssiDetectorThreshold73dB = 0b101
} Rfm12RssiDetectorThreshold;

typedef enum {
    Rfm12AutoLockEnabled = 1,
    Rfm12AutoLockDisabled = 0
} Rfm12AutoLock;

typedef enum {
    Rfm12ManualLockSlow = 0,
    Rfm12ManualLockFast = 1
} Rfm12ManualLock;

typedef enum {
    Rfm12DigitalFilter = 0,
    Rfm12AnalogRcFilter = 1
} Rfm12Filter;

typedef enum {
    Rfm12DqdThresholdAlwayOn = 0b000,
    Rfm12DqdThresholdMedium = 0b100,
    Rfm12DqdThresholdMaximum = 0b111
} Rfm12DqdThreshold;

/** This bit is undocumented, however it is necessary to use OOK modulation/demodulation */
typedef enum {
    Rfm12OokModulationEnabled = 0,
    Rfm12OokModulationDisabled = 1
} Rfm12OokModulation;

typedef enum {
    Rfm12FifoInterruptLevelBit = 0b0001,
    Rfm12FifoInterruptLevelByte = 0b1000
} Rfm12FifoInterruptLevel;

typedef enum {
    Rfm12SyncPatternLength2Byte = 0,
    Rfm12SyncPatternLength1Byte = 1
} Rfm12SyncPatternLength;

typedef enum {
    Rfm12FifoFillConditionAlways = 1,
    Rfm12FifoFillConditionSynchronPattern = 0
} Rfm12FifoFillCondition;

typedef enum {
    Rfm12FifoFillModeClearAndLock = 0,
    Rfm12FifoFillModeRelease = 1
} Rfm12FifoFillMode;

typedef enum {
    Rfm12SensitiveResetEnabled = 1,
    Rfm12SensitiveResetDisabled = 0
} Rfm12SensitiveReset;

typedef enum {
    Rfm12AfcAutoModeOff = 0b00,
    Rfm12AfcAutoModeStartup = 0b01,
    Rfm12AfcAutoModeVdiLow = 0b10,
    Rfm12AfcAutoModeAlways = 0b11
} Rfm12AfcAutoMode;

typedef enum {
    Rfm12AfcRangeLimitUnlimited = 0b00,    /** -64delta f, +62delta f */
    Rfm12AfcRangeLimitWide = 0b01,         /** -16delta f, +15delta f */
    Rfm12AfcRangeLimitMedium = 0b10,       /** -8detla f, +7delta f */
    Rfm12AfcRangeLimitSmall = 0b11         /** -4delta f, +3delta f */
} Rfm12AfcRangeLimit;

typedef enum {
    Rfm12AfcStrobeEdgeEnabled = 1,
    Rfm12AfcStrobeEdgeDisabled = 0
} Rfm12AfcStrobeEdge;

typedef enum {
    Rfm12AfcFineModeEnabled = 1,
    Rfm12AfcFineModeDisabled = 0
} Rfm12AfcFineMode;

typedef enum {
    Rfm12AfcOffsetEnabled = 1,
    Rfm12AfcOffsetDisabled = 0
} Rfm12AfcOffset;

typedef enum {
    Rfm12AfcEnabled = 1,
    Rfm12AfcDisabled = 0
} Rfm12Afc;

typedef enum {
    Rfm12ModulationPolarityNormal = 0,
    Rfm12ModulationPolarityInverted = 1
} Rfm12ModulationPolarity;

typedef enum {
    Rfm12FrequencyDeviation15kHz = 0b0000,
    Rfm12FrequencyDeviation30kHz = 0b0001,
    Rfm12FrequencyDeviation45kHz = 0b0010,
    Rfm12FrequencyDeviation60kHz = 0b0011,
    Rfm12FrequencyDeviation75kHz = 0b0100,
    Rfm12FrequencyDeviation90kHz = 0b0101,
    Rfm12FrequencyDeviation105kHz = 0b0110,
    Rfm12FrequencyDeviation120kHz = 0b0111,
    Rfm12FrequencyDeviation135kHz = 0b1000,
    Rfm12FrequencyDeviation150kHz = 0b1001,
    Rfm12FrequencyDeviation165kHz = 0b1010,
    Rfm12FrequencyDeviation180kHz = 0b1011,
    Rfm12FrequencyDeviation195kHz = 0b1100,
    Rfm12FrequencyDeviation210kHz = 0b1101,
    Rfm12FrequencyDeviation225kHz = 0b1110,
    Rfm12FrequencyDeviation240kHz = 0b1111
} Rfm12FrequencyDeviation;

typedef enum {
    Rfm12RelativeOutputPower0dB = 0b000,
    Rfm12RelativeOutputPower2_5dB = 0b001,
    Rfm12RelativeOutputPower5dB = 0b010,
    Rfm12RelativeOutputPower7_5dB = 0b011,
    Rfm12RelativeOutputPower10dB = 0b100,
    Rfm12RelativeOutputPower12_5dB = 0b101,
    Rfm12RelativeOutputPower15dB = 0b110,
    Rfm12RelativeOutputPower17_5dB = 0b111
} Rfm12RelativeOutputPower;

typedef enum {
    Rfm12CyclicWakeupEnabled = 1,
    Rfm12CyclicWakeupDisabled = 0
} Rfm12CyclicWakeup;

typedef enum {
    Rfm12ModeNone = 0,
    Rfm12ModeSender = 1,
    Rfm12ModeReceiver = 2
} Rfm12Mode;

/** Here is the place for the status bits */

int8 initializeRfm12(uint8 id, GpioPair selPair, GpioPair dataPair);

void prepareOokSendingRfm12(void);
void prepareOokReceivingRfm12(Rfm12FrequencyBand frequencyBand, float frequency, uint32 dataRate);

void setRfm12BaseConfig(Rfm12TxFifo txFifoEnable, 
                        Rfm12RxFifo rxFifoEnable, 
                        Rfm12FrequencyBand frequencyBand, 
                        Rfm12CrystalLoadCapacitance loadCapacitance);

void setRfm12PowerManagement(Rfm12Receiver receiverEnable,
                             Rfm12Baseband basebandEnable,
                             Rfm12Transmitter transmitterEnable,
                             Rfm12Synthesizer synthesizerEnable,
                             Rfm12Oscillator xtalEnable,
                             Rfm12BrownoutDetection brownoutEnable,
                             Rfm12WakeupTimer wakeupEnable,
                             Rfm12ClockOutput clockEnable);

void setRfm12ClockGenerator(Rfm12ClockOutputBuffer outputBuffer,
                            Rfm12OscillatorLowPowerMode lowPowerXtal,
                            Rfm12PhaseDetectorDelay phaseDelay,
                            Rfm12PllDithering ditheringEnable,
                            Rfm12PllBandwith bandwith);

void setRfm12LowBatteryDetectorAndClockDivider(Rfm12Clock clock,
                                               uint16 thresholdVoltage);

void setRfm12Frequency(Rfm12FrequencyBand frequencyBand, 
                       float frequency);

void setRfm12DataRate(uint32 baudrate);

void setRfm12ReceiverControl(Rfm12DataPin pinIntVdi,
                             Rfm12ValidDataIndicatorResponse validDataIndicatorResponse,
                             Rfm12ReceiverBasebandBandwidth receiverBasebandBandwidth,
                             Rfm12LnaGain lnaGainSelect,
                             Rfm12RssiDetectorThreshold rssiDetectorThreshold);
                             
void setRfm12SynchronPattern(uint8 pattern);

void setRfm12DataFilter(Rfm12AutoLock autoLock,
                        Rfm12ManualLock manualLock,
                        Rfm12Filter filter,
                        Rfm12DqdThreshold dqdThreshold,
                        Rfm12OokModulation ookModulationEnable);

void setRfm12FifoAndResetMode(Rfm12FifoInterruptLevel fifoInterruptLevel,
                              Rfm12SyncPatternLength syncPatternLength,
                              Rfm12FifoFillCondition alwayFill,
                              Rfm12FifoFillMode fifoFill,
                              Rfm12SensitiveReset sensResetEnable);

void setRfm12AutomaticFrequencyControl(Rfm12AfcAutoMode autoMode,
                                       Rfm12AfcRangeLimit rangeLimit,
                                       Rfm12AfcStrobeEdge strobeEdgeEnable,
                                       Rfm12AfcFineMode fineModeEnable,
                                       Rfm12AfcOffset afcOffsetEnable,
                                       Rfm12Afc afcEnable);
void setRfm12TxConfiguration(Rfm12ModulationPolarity modulationPolarity,
                             Rfm12FrequencyDeviation frequencyDeviation,
                             Rfm12RelativeOutputPower relativeOutputPower);
                             
void setRfm12WakeUpTimer(uint8 radix, uint8 mantissa);

void setRfm12LowDutyCycle(uint8 duration,
                          Rfm12CyclicWakeup cyclicWakeupEnable);

void actionRfm12SoftwareReset();

uint16 readRfm12Status();

char getcharRfm12();
void putcharRfm12(char byte);


/**
 * @}
 */
