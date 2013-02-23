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
    Rfm12EnableTxFifo = 1,
    Rfm12DisableTxFifo = 0
} Rfm12TxFifoEnable;
    
typedef enum {
    Rfm12EnableRxFifo = 1,
    Rfm12DisableRxFifo = 0
} Rfm12RxFifoEnable;

typedef enum {
    Rfm12_315MhzBand = 0b00,
    Rfm12_433MhzBand = 0b01,
    Rfm12_868MhzBand = 0b10,
    Rfm12_915MhzBand = 0b11
} Rfm12FrequencyBand;

typedef enum {
    Rfm12_8_5pFCrystalLoadCapacitance = 0b0000,
    Rfm12_9_0pFCrystalLoadCapacitance = 0b0001,
    Rfm12_9_5pFCrystalLoadCapacitance = 0b0010,
    Rfm12_10_0pFCrystalLoadCapacitance = 0b0011,
    Rfm12_10_5pFCrystalLoadCapacitance = 0b0100,
    Rfm12_11_0pFCrystalLoadCapacitance = 0b0101,
    Rfm12_11_5pFCrystalLoadCapacitance = 0b0110,
    Rfm12_12_0pFCrystalLoadCapacitance = 0b0111,
    Rfm12_12_5pFCrystalLoadCapacitance = 0b1000,
    Rfm12_13_0pFCrystalLoadCapacitance = 0b1001,
    Rfm12_13_5pFCrystalLoadCapacitance = 0b1010,
    Rfm12_14_0pFCrystalLoadCapacitance = 0b1011,
    Rfm12_14_5pFCrystalLoadCapacitance = 0b1100,
    Rfm12_15_0pFCrystalLoadCapacitance = 0b1101,
    Rfm12_15_5pFCrystalLoadCapacitance = 0b1110,
    Rfm12_16_0pFCrystalLoadCapacitance = 0b1111
} Rfm12CrystalLoadCapacitance;

typedef enum {
    Rfm12EnableReceiver = 1,
    Rfm12DisableReceiver = 0
} Rfm12ReceiverEnable;

typedef enum {
    Rfm12EnableBaseband = 1,
    Rfm12DisableBaseband = 0
} Rfm12BasebandEnbale;

typedef enum {
    Rfm12EnableTransmitter = 1,
    Rfm12DisableTransmitter = 0
} Rfm12TransmitterEnable;

typedef enum {
    Rfm12EnableSynthesizer = 1,
    Rfm12DisableSynthesizer = 0
} Rfm12SynthesizerEnable;

typedef enum {
    Rfm12EnableOscillator = 1,
    Rfm12DisableOscillator = 0
} Rfm12OscillatorEnable;

typedef enum {
    Rfm12EnableBrownoutDetection = 1,
    Rfm12DisableBrownoutDetection = 0
} Rfm12BrownoutDetectionEnable;

typedef enum {
    Rfm12EnableWakeupTimer = 1,
    Rfm12DisableWakeupTimer = 0
} Rfm12WakeupTimerEnable;

typedef enum {
    Rfm12EnableClockOutput = 0,
    Rfm12DisableClockOutput = 1
} Rfm12ClockOutputEnable;

typedef enum {
    Rfm12WeakClockOutputBuffer = 0b00,   /** < 2.5MHz */
    Rfm12MediumClockOutputBuffer = 0b10, /** 3.3MHz */
    Rfm12StrongClockOutputBuffer = 0b11  /** 5 to 10MHz */
} Rfm12ClockOutputBuffer;

typedef enum {
    Rfm12_1msOscillatorStartup = 0,
    Rfm12_2msOscillatorStartup = 1
} Rfm12OscillatorLowPowerMode;

typedef enum {
    Rfm12DisablePhaseDetectorDelay = 0,
    Rfm12EnablePhaseDetectorDelay = 1
} Rfm12PhaseDetectorDelay;

typedef enum {
    Rfm12EnablePllDithering = 0,
    Rfm12DisablePllDithering = 1
} Rfm12PllDitheringEnable;

typedef enum {
    Rfm12PllBandwith1 = 0x10,
    Rfm12PllBandwith2 = 0x11
} Rfm12PllBandwith;

typedef enum {
    Rfm12_1MHzClock = 0b000,
    Rfm12_1_25MHzClock = 0b001,
    Rfm12_1_6MHzClock = 0b010,
    Rfm12_2MHzClock = 0b011,
    Rfm12_2_5MHzClock = 0b100,
    Rfm12_3_3MHzClock = 0b101,
    Rfm12_5MHzClock = 0b110,
    Rfm12_10MHzClock = 0b111
} Rfm12Clock;

typedef enum {
    Rfm12IntPin = 0,
    Rfm12VdiPin = 1
} Rfm12IntVdiPin;

typedef enum {
    Rfm12FastValidDataIndicatorResponse = 0b00,
    Rfm12MediumValidDataIndicatorResponse = 0b01,
    Rfm12SlowValidDataIndicatorResponse = 0b10,
    Rfm12AlwaysOnValidDataIndicatorResponse = 0b11
} Rfm12ValidDataIndicatorResponse;

typedef enum {
    Rfm12_400kHzReceiverBasebandBandwidth = 0b001,
    Rfm12_340kHzReceiverBasebandBandwidth = 0b010,
    Rfm12_270kHzReceiverBasebandBandwidth = 0b011,
    Rfm12_200kHzReceiverBasebandBandwidth = 0b100,
    Rfm12_134kHzReceiverBasebandBandwidth = 0b101,
    Rfm12_67kHzReceiverBasebandBandwidth = 0b110
} Rfm12ReceiverBasebandBandwidth;

typedef enum {
    Rfm12_0dBLnaGain = 0b00,
    Rfm12_6dBLnaGain = 0b01,
    Rfm12_14dBLnaGain = 0b10,
    Rfm12_20dBLnaGain = 0b11
} Rfm12LnaGainSelect;

typedef enum {
    Rfm12_103dBRssiDetectorThreshold = 0b000,
    Rfm12_97dBRssiDetectorThreshold = 0b001,
    Rfm12_91dBRssiDetectorThreshold = 0b010,
    Rfm12_85dBRssiDetectorThreshold = 0b011,
    Rfm12_79dBRssiDetectorThreshold = 0b100,
    Rfm12_73dBRssiDetectorThreshold = 0b101
} Rfm12RssiDetectorThreshold;

typedef enum {
    Rfm12EnableAutoLock = 1,
    Rfm12DisableAutoLock = 0
} Rfm12AutoLock;

typedef enum {
    Rfm12SlowManualLock = 0,
    Rfm12FastManualLock = 1
} Rfm12ManualLock;

typedef enum {
    Rfm12DigitalFilter = 0,
    Rfm12AnalogRcFilter = 1
} Rfm12Filter;

typedef enum {
    Rfm12AlwayOnDqdThreshold = 0b000,
    Rfm12MediumDqdThreshold = 0b100,
    Rfm12MaximumDqdThreshold = 0b111
} Rfm12DqdThreshold;

/** This bit is undocumented, however it is necessary to use OOK modulation/demodulation */
typedef enum {
    Rfm12EnableOokModulation = 0,
    Rfm12DisableOokModulation = 1
} Rfm12OokModulationEnable;

typedef enum {
    Rfm12BitFifoInterruptLevel = 0b0001,
    Rfm12ByteFifoInterruptLevel = 0b1000
} Rfm12FifoInterruptLevel;

typedef enum {
    Rfm12_2ByteSyncPatternLength = 0,
    Rfm12_1ByteSyncPatternLength = 1
} Rfm12SyncPatternLength;

typedef enum {
    Rfm12AlwaysFillFifo = 1,
    Rfm12SynchronPatternFillFifo = 0
} Rfm12FifoFillCondition;

typedef enum {
    Rfm12ClearAndLockFifo = 0,
    Rfm12ReleaseFifo = 1
} Rfm12FifoFill;

typedef enum {
    Rfm12EnableSensitiveReset = 1,
    Rfm12DisableSensitiveReset = 0
} Rfm12SensitiveResetEnable;

typedef enum {
    Rfm12NoAutoMode = 0b00,
    Rfm12StartupAutoMode = 0b01,
    Rfm12VdiLowAutoMode = 0b10,
    Rfm12AlwaysAutoMode = 0b11
} Rfm12AutoMode;

typedef enum {
    Rfm12UnlimitedRangeLimit = 0b00,    /** -64delta f, +62delta f */
    Rfm12WideRangeLimit = 0b01,         /** -16delta f, +15delta f */
    Rfm12MediumRangeLimit = 0b10,       /** -8detla f, +7delta f */
    Rfm12SmallRangeLimit = 0b11         /** -4delta f, +3delta f */
} Rfm12RangeLimit;

typedef enum {
    Rfm12EnableStrobeEdge = 1,
    Rfm12DisableStrobeEdge = 0
} Rfm12StrobeEdgeEnable;

typedef enum {
    Rfm12EnableFineMode = 1,
    Rfm12DisableFineMode = 0
} Rfm12FineModeEnable;

typedef enum {
    Rfm12EnableAfcOffset = 1,
    Rfm12DisableAfcOffset = 0
} Rfm12AfcOffsetEnable;

typedef enum {
    Rfm12EnableAfc = 1,
    Rfm12DisableAfc = 0
} Rfm12AfcEnable;

typedef enum {
    Rfm12NormalModulationPolarity = 0,
    Rfm12InvertedModulationPolarity = 1
} Rfm12ModulationPolarity;

typedef enum {
    Rfm12_15kHzFrequencyDeviation = 0b0000,
    Rfm12_30kHzFrequencyDeviation = 0b0001,
    Rfm12_45kHzFrequencyDeviation = 0b0010,
    Rfm12_60kHzFrequencyDeviation = 0b0011,
    Rfm12_75kHzFrequencyDeviation = 0b0100,
    Rfm12_90kHzFrequencyDeviation = 0b0101,
    Rfm12_105kHzFrequencyDeviation = 0b0110,
    Rfm12_120kHzFrequencyDeviation = 0b0111,
    Rfm12_135kHzFrequencyDeviation = 0b1000,
    Rfm12_150kHzFrequencyDeviation = 0b1001,
    Rfm12_165kHzFrequencyDeviation = 0b1010,
    Rfm12_180kHzFrequencyDeviation = 0b1011,
    Rfm12_195kHzFrequencyDeviation = 0b1100,
    Rfm12_210kHzFrequencyDeviation = 0b1101,
    Rfm12_225kHzFrequencyDeviation = 0b1110,
    Rfm12_240kHzFrequencyDeviation = 0b1111
} Rfm12FrequencyDeviation;

typedef enum {
    Rfm12_0dBRelativeOutputPower = 0b000,
    Rfm12_2_5dBRelativeOutputPower = 0b001,
    Rfm12_5dBRelativeOutputPower = 0b010,
    Rfm12_7_5dBRelativeOutputPower = 0b011,
    Rfm12_10dBRelativeOutputPower = 0b100,
    Rfm12_12_5dBRelativeOutputPower = 0b101,
    Rfm12_15dBRelativeOutputPower = 0b110,
    Rfm12_17_5dBRelativeOutputPower = 0b111
} Rfm12RelativeOutputPower;

typedef enum {
    Rfm12EnableCyclicWakeup = 1,
    Rfm12DisableCyclicWakeup = 0
} Rfm12CyclicWakeupEnable;

typedef enum {
    Rfm12NoMode = 0,
    Rfm12SenderMode = 1,
    Rfm12ReceiverMode = 2
} Rfm12ReceiverSenderMode;

/** Here is the place for the status bits */

int8 initializeRfm12(uint8 id, GpioPair selPair, GpioPair dataPair);

void prepareOokSendingRfm12(void);
void prepareOokReceivingRfm12(Rfm12FrequencyBand frequencyBand, float frequency, uint32 dataRate);

void setRfm12BaseConfig(Rfm12TxFifoEnable txFifoEnable, 
                        Rfm12RxFifoEnable rxFifoEnable, 
                        Rfm12FrequencyBand frequencyBand, 
                        Rfm12CrystalLoadCapacitance loadCapacitance);

void setRfm12PowerManagement(Rfm12ReceiverEnable receiverEnable,
                             Rfm12BasebandEnbale basebandEnable,
                             Rfm12TransmitterEnable transmitterEnable,
                             Rfm12SynthesizerEnable synthesizerEnable,
                             Rfm12OscillatorEnable xtalEnable,
                             Rfm12BrownoutDetectionEnable brownoutEnable,
                             Rfm12WakeupTimerEnable wakeupEnable,
                             Rfm12ClockOutputEnable clockEnable);

void setRfm12ClockGenerator(Rfm12ClockOutputBuffer outputBuffer,
                            Rfm12OscillatorLowPowerMode lowPowerXtal,
                            Rfm12PhaseDetectorDelay phaseDelay,
                            Rfm12PllDitheringEnable ditheringEnable,
                            Rfm12PllBandwith bandwith);

void setRfm12LowBatteryDetectorAndClockDivider(Rfm12Clock clock,
                                               uint16 thresholdVoltage);

void setRfm12Frequency(Rfm12FrequencyBand frequencyBand, 
                       float frequency);

void setRfm12DataRate(uint32 baudrate);

void setRfm12ReceiverControl(Rfm12IntVdiPin pinIntVdi,
                             Rfm12ValidDataIndicatorResponse validDataIndicatorResponse,
                             Rfm12ReceiverBasebandBandwidth receiverBasebandBandwidth,
                             Rfm12LnaGainSelect lnaGainSelect,
                             Rfm12RssiDetectorThreshold rssiDetectorThreshold);
                             
void setRfm12SynchronPattern(uint8 pattern);

void setRfm12DataFilter(Rfm12AutoLock autoLock,
                        Rfm12ManualLock manualLock,
                        Rfm12Filter filter,
                        Rfm12DqdThreshold dqdThreshold,
                        Rfm12OokModulationEnable ookModulationEnable);

void setRfm12FifoAndResetMode(Rfm12FifoInterruptLevel fifoInterruptLevel,
                              Rfm12SyncPatternLength syncPatternLength,
                              Rfm12FifoFillCondition alwayFill,
                              Rfm12FifoFill fifoFill,
                              Rfm12SensitiveResetEnable sensResetEnable);

void setRfm12AutomaticFrequencyControl(Rfm12AutoMode autoMode,
                                       Rfm12RangeLimit rangeLimit,
                                       Rfm12StrobeEdgeEnable strobeEdgeEnable,
                                       Rfm12FineModeEnable fineModeEnable,
                                       Rfm12AfcOffsetEnable afcOffsetEnable,
                                       Rfm12AfcEnable afcEnable);
void setRfm12TxConfiguration(Rfm12ModulationPolarity modulationPolarity,
                             Rfm12FrequencyDeviation frequencyDeviation,
                             Rfm12RelativeOutputPower relativeOutputPower);
                             
void setRfm12WakeUpTimer(uint8 radix, uint8 mantissa);

void setRfm12LowDutyCycle(uint8 duration,
                          Rfm12CyclicWakeupEnable cyclicWakeupEnable);

void actionRfm12SoftwareReset();

uint16 readRfm12Status();

char getcharRfm12();
void putcharRfm12(char byte);


/**
 * @}
 */
