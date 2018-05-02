/*
 * lllrc5data.h
 *
 * Created: 2013-09-24 19:02:41
 *  Author: Administrator
 */ 

#ifndef LLLRC5DATA_H_
#define LLLRC5DATA_H_

#define RC5_HALF_BIT_PERIOD_US_MIN 		(uint16_t)(F_CPU/1000000UL*640)
#define RC5_HALF_BIT_PERIOD_US_TYP 		(uint16_t)(F_CPU/1000000UL*889)
#define RC5_HALF_BIT_PERIOD_US_MAX		(uint16_t)(F_CPU/1000000UL*1140)

#define SAMPLE_PERIOD_US				(uint16_t)(F_CPU/1000000UL*889)
#define FIRST_SAMPLE_PERIOD_US			(uint16_t)(F_CPU/1000000UL*1300)

#define RC5_FRAME_LENGTH				25


#define enableTimerCTCModeWithInt()	    TCCR1B |= (1<<WGM12); TIMSK1 |= (1<<OCIE1A)	////	CTC Mode
#define setSamplePeriod(T)              OCR1A = (T)
#define setSamplePeriodUS(T)            OCR1A = ((T) * (F_CPU/1000000UL))
#define setMeasuredHalfBitPeriod()		OCR1A = TCNT1

#define rc5EnableFallingEdgeInt()       EICRB |= (1<<ISC61)
#define rc5EnableBothEdgesInt()			EICRB |= (1<<ISC60)
#define enableExternalInterrupt()       cli(); EIMSK |= (1<<INT6); EIFR = (1<<INTF6); sei()
#define disableExternalInterrupt()      EIMSK &= ~(1<<INT6)
#define rc5TimerGetUS()					(TCNT1>>2)
#define rc5TimerGet()					TCNT1
#define cleanTimer()					TCNT1 = 0x0000

#define runTimer()                      TCCR1B|=(1<<CS10) //run timer 1; prescaler 1;
#define haltTimer()                     TCCR1B&=~(1<<CS10) // halt timer
#define stopTimer()                     TCCR1B&=~(1<<CS10); TCNT1=0x0000 // stop and clear timer


typedef enum {
	RC5_EXT_INT_START_MEASURE,
	RC5_EXT_INT_START_SAMPLE,
	
	RC5_EXT_INT_STATES_NR,
} eRC5ExtIntStateID_t;
typedef uint8_t RC5ExtIntStateID_t;

typedef uint8_t (*ExtIntServicePtr_t)(void);

typedef struct {
	RC5ExtIntStateID_t id;
	ExtIntServicePtr_t servicePtr;
} ExtIntState_t;

#define INIT_EXT_INT_STATE(ID, SERVICE_PTR)		{ID, SERVICE_PTR}



#endif /* LLLRC5DATA_H_ */