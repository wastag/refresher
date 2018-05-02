//////////////////////////////////////////////////////////////////////////
/// \file lllrc5.c
///
/// \brief RC5 decoder low level implementation
///
///    All rights released
///
/// \author Jacek Wastag    jwastag@gmail.com
///
//////////////////////////////////////////////////////////////////////////

#include "lllio.h"
#include "lllrc5.h"
#include "lllrc5data.h"
#include "hardwareconfig.h"
#include "debugio.h"
#include <avr/interrupt.h>
#include <avr/io.h>

typedef struct
{
	RC5Frames_t frames; 
	uint8_t bitsCntr;
	uint16_t halfBitTime;
} RC5RxBuffer_t;

volatile static RC5RxBuffer_t rc5RxBuffer = 
{
	{
		0x0000, // uint16_t positiveFrame
		0x0000, // uint16_t negativeFrame
	},
	0, // uint8_t bitsCntr
	0, // uint16_t halfBitTime
};

static void __rc5dummyCallback(void)
{
	return;
}

static pFunction_t rc5dataCompleteCallback = __rc5dummyCallback;

void lllrc5GetDataFrames(RC5Frames_t * rc5_frames)
{
	*rc5_frames = rc5RxBuffer.frames;
}

//////////////////////////////////////////////////////////////////////////
/// \brief The local functions
//////////////////////////////////////////////////////////////////////////
static void __halfBitMaxPeriodReached(void);
static void __sampleRC5data(void);
static uint8_t __extIntStartMeasure(void);
static uint8_t __extIntStartSample(void);

static pFunction_t compareHandler = &__halfBitMaxPeriodReached;

//////////////////////////////////////////////////////////////////////////
/// \brief External interrupt states table
//////////////////////////////////////////////////////////////////////////
static ExtIntState_t extIntSM[] = {
	INIT_EXT_INT_STATE(RC5_EXT_INT_START_MEASURE, 	__extIntStartMeasure),
	INIT_EXT_INT_STATE(RC5_EXT_INT_START_SAMPLE, 	__extIntStartSample),
};

//////////////////////////////////////////////////////////////////////////
/// \brief External interrupt - current state
//////////////////////////////////////////////////////////////////////////
static RC5ExtIntStateID_t stateID = RC5_EXT_INT_START_MEASURE;

static void __extInt0SM(void)
{
	if(stateID == extIntSM[stateID].id)
	{
		stateID = (*extIntSM[stateID].servicePtr)();
	}
	if(stateID >= RC5_EXT_INT_STATES_NR)
	{
		stateID = RC5_EXT_INT_START_MEASURE;
	}
}

//////////////////////////////////////////////////////////////////////////
/// \brief The function initializes the RC5 decoder
//////////////////////////////////////////////////////////////////////////
void lllrc5InitModule(void)
{
	rc5RxBuffer.frames.positiveFrame = 0x0000;
	rc5RxBuffer.frames.negativeFrame = 0x0000;
	rc5RxBuffer.bitsCntr = 0;
	rc5RxBuffer.halfBitTime = 0;
	compareHandler = &__halfBitMaxPeriodReached;
	stateID = RC5_EXT_INT_START_MEASURE;
	
	enableTimerCTCModeWithInt();
	rc5EnableFallingEdgeInt();
	enableExternalInterrupt();
}

void lllrc5dataCompleteCallbackRegister(pFunction_t callback)
{
	rc5dataCompleteCallback = callback;
}

//////////////////////////////////////////////////////////////////////////
/// \brief The function initializes the RC5 decoder
//////////////////////////////////////////////////////////////////////////
void lllrc5DisableDecoder(void)
{
	stopTimer();
	rc5RxBuffer.bitsCntr=0;
	disableExternalInterrupt();
}

//////////////////////////////////////////////////////////////////////////
/// \brief The handler for the external interrupt
//////////////////////////////////////////////////////////////////////////
static uint8_t __extIntStartMeasure(void)
{
	runTimer();
	setSamplePeriod(2*RC5_HALF_BIT_PERIOD_US_MAX);
	compareHandler = &__halfBitMaxPeriodReached;
	rc5RxBuffer.frames.negativeFrame = 0;
	rc5RxBuffer.frames.positiveFrame = 0;

	if(0 != ((1<<RC5_PIN) & RC5_STATE))
	{
		stopTimer();
		return RC5_EXT_INT_START_MEASURE;
	}
	return RC5_EXT_INT_START_SAMPLE;
}

static uint8_t __extIntStartSample(void)
{
	haltTimer();
	if(0 != ((1<<RC5_PIN) & RC5_STATE) )//|| (2*RC5_HALF_BIT_PERIOD_US_MIN) > rc5TimerGetUS())
	{
		stopTimer();
	}
	else
	{
		rc5RxBuffer.halfBitTime = rc5TimerGet();
		cleanTimer();
		rc5RxBuffer.halfBitTime >>= 1;
		setSamplePeriod((rc5RxBuffer.halfBitTime>>1));
		compareHandler = &__sampleRC5data;
		runTimer();
		disableExternalInterrupt();
	}
	return RC5_EXT_INT_START_MEASURE;
}


static void __halfBitMaxPeriodReached(void)
{
	stopTimer();
	stateID = RC5_EXT_INT_START_MEASURE;
	enableExternalInterrupt();
}

static void __sampleRC5data(void)
{
	static volatile uint8_t isPositiveFrame = 0;

	setSamplePeriod(rc5RxBuffer.halfBitTime);
	if(isPositiveFrame)
	{
		isPositiveFrame = 0;
		rc5RxBuffer.frames.positiveFrame |= (((1<<RC5_PIN) & RC5_STATE)>>RC5_PIN);
	}
	else
	{
		isPositiveFrame = 1;
		rc5RxBuffer.frames.negativeFrame |= (((1<<RC5_PIN) & RC5_STATE)>>RC5_PIN);
	}
	rc5RxBuffer.bitsCntr++;
	
	if(RC5_FRAME_LENGTH > rc5RxBuffer.bitsCntr)
	{
		if(isPositiveFrame){
			rc5RxBuffer.frames.positiveFrame <<= 1;
		}
		else
		{
			rc5RxBuffer.frames.negativeFrame <<= 1;
		}
	}
	else
	{
		stopTimer();
		enableExternalInterrupt();
		rc5RxBuffer.bitsCntr = 0;
		isPositiveFrame = 0;
		(*rc5dataCompleteCallback)();
	}
}

//////////////////////////////////////////////////////////////////////////
/// \brief The external interrupt handler
//////////////////////////////////////////////////////////////////////////
ISR(INT6_vect)
{
#if (MEASURE_IRQ_TIME == INT0_ISR_NR)
	isrMeasurePinHigh();
#endif
	__extInt0SM();
#if (MEASURE_IRQ_TIME == INT0_ISR_NR)
	isrMeasurePinLow();
#endif
}

//////////////////////////////////////////////////////////////////////////
/// \brief The timer's clear on compare interrupt handler
//////////////////////////////////////////////////////////////////////////
ISR(TIMER1_COMPA_vect)
{
#if (MEASURE_IRQ_TIME == TIMER1_COMPA_ISR_NR)
	isrMeasurePinLow();
#endif

	(*compareHandler)();

#if (MEASURE_IRQ_TIME == TIMER1_COMPA_ISR_NR)
	isrMeasurePinHigh();
#endif
}