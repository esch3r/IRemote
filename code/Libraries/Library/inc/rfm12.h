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
    Rfm12EnableTxFifo = 0,
    Rfm12DisableTxFifo = 1
} Rfm12TxFifoEnable;
    
typedef enum {
    Rfm12EnableRxFifo = 0,
    Rfm12DisableRxFifo = 1
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
    Rfm12EnableXtal = 1,
    Rfm12DisableXtal = 0
} Rfm12XtalEnable;

typedef enum {
    Rfm12EnableBrownout = 1,
    Rfm12DisableBrownout = 0
} Rfm12BrownoutEnable;

typedef enum {
    Rfm12EnableWakeup = 1,
    Rfm12DisableWakeup = 0
} Rfm12WakeupEnable;

typedef enum {
    Rfm12EnableClock = 0,
    Rfm12DisableClock = 1
} Rfm12ClockEnable;

typedef enum {
    Rfm12WeakOutputBuffer = 0b00,   /** < 2.5MHz */
    Rfm12MediumOutputBuffer = 0b10, /** 3.3MHz */
    Rfm12StrongOutputBuffer = 0b11  /** 5 to 10MHz */
} Rfm12OutputBuffer;

typedef enum {
    Rfm12_1msXtal = 0,
    Rfm12_2msXtal = 1
} Rfm12LowPowerXtal;

typedef enum {
    Rfm12DisablePhaseDelay = 0,
    Rfm12EnablePhaseDelay = 1
} Rfm12PhaseDelay;

typedef enum {
    Rfm12EnableDithering = 0,
    Rfm12DisableDithering = 1
} Rfm12DitheringEnable;

typedef enum {
    Rfm12Bandwith1 = 0x10,
    Rfm12Bandwith2 = 0x11
} Rfm12Bandwith;

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
    Rfm12PinInt = 0,
    Rfm12PinVdi = 1
} Rfm12PinIntVdi;

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

typedef enum {
    Rfm12BitInterruptLevel = 0b0001,
    Rfm12ByteInterruptLevel = 0b1000
} Rfm12FifoInterruptLevel;

typedef enum {
    Rfm12_2ByteSyncPatternLength = 0,
    Rfm12_1ByteSyncPatternLength = 1
} Rfm12SyncPatternLength;

typedef enum {
    Rfm12FillInstant = 1,
    Rfm12FillPattern = 0
} Rfm12AlwayFill;

typedef enum {
    Rfm12EmptyAndLockFifo = 0,
    Rfm12ReleaseFifo = 1
} Rfm12FifoFill;

typedef enum {
    Rfm12EnableSensReset = 1,
    Rfm12DisableSensReset = 0
} Rfm12SensResetEnable;

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

void prepareSendingRfm12(void);
void prepareReceivingRfm12(void);

void setRfm12BaseConfig(Rfm12TxFifoEnable txFifoEnable, 
                        Rfm12RxFifoEnable rxFifoEnable, 
                        Rfm12FrequencyBand frequencyBand, 
                        Rfm12CrystalLoadCapacitance loadCapacitance);

void setRfm12PowerManagement(Rfm12ReceiverEnable receiverEnable,
                             Rfm12BasebandEnbale basebandEnable,
                             Rfm12TransmitterEnable transmitterEnable,
                             Rfm12SynthesizerEnable synthesizerEnable,
                             Rfm12XtalEnable xtalEnable,
                             Rfm12BrownoutEnable brownoutEnable,
                             Rfm12WakeupEnable wakeupEnable,
                             Rfm12ClockEnable clockEnable);

void setRfm12ClockGenerator(Rfm12OutputBuffer outputBuffer,
                            Rfm12LowPowerXtal lowPowerXtal,
                            Rfm12PhaseDelay phaseDelay,
                            Rfm12DitheringEnable ditheringEnable,
                            Rfm12Bandwith bandwith);

void setRfm12LowBatteryDetectorAndClockDivider(Rfm12Clock clock,
                                               uint16 thresholdVoltage);

void setRfm12Frequency(Rfm12FrequencyBand frequencyBand, 
                       float frequency);

void setRfm12DataRate(uint32 baudrate);

void setRfm12ReceiverControl(Rfm12PinIntVdi pinIntVdi,
                             Rfm12ValidDataIndicatorResponse validDataIndicatorResponse,
                             Rfm12ReceiverBasebandBandwidth receiverBasebandBandwidth,
                             Rfm12LnaGainSelect lnaGainSelect,
                             Rfm12RssiDetectorThreshold rssiDetectorThreshold);
                             
void setRfm12SynchronPattern(uint8 pattern);

void setRfm12DataFilter(Rfm12AutoLock autoLock,
                        Rfm12ManualLock manualLock,
                        Rfm12Filter filter,
                        Rfm12DqdThreshold dqdThreshold);

void setRfm12FifoAndResetMode(Rfm12FifoInterruptLevel fifoInterruptLevel,
                              Rfm12SyncPatternLength syncPatternLength,
                              Rfm12AlwayFill alwayFill,
                              Rfm12FifoFill fifoFill,
                              Rfm12SensResetEnable sensResetEnable);

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
