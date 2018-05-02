/*
 * lllio.h
 *
 * Created: 2013-10-15 18:08:45
 *  Author: Administrator
 */ 


#ifndef LLLIO_H_
#define LLLIO_H_

#include "typedefs.h"

typedef enum
{
	GREEN_LED,
	RED_LED,	
} eLedId_t;
typedef uint8_t LedId_t;

typedef enum
{
	LED_OFF,
	LED_ON,	
} eLedAction_t;
typedef uint8_t LedAction_t;

typedef enum
{
	BUTTON_RELEASED,
	BUTTON_PRESSED,
	BUTTON_UNINIT,
} eButton1State_t;
typedef uint8_t Button1State_t;

void lllioInitLeds(void);
void lllioInitButtons(void);
Button1State_t lllioGetButton1State(void);
Button1State_t lllioGetButton2State(void);
void lllioButtonsStateControl(void);
void lllioToggleGreenLed(void);
void lllioToggleRedLed(void);
int16_t lllioSetLEDState(LedId_t id, LedAction_t action);
void lllsysInitOtherPeripherals(void);
void lllioRefresherOff(void);
void lllioRefresherOn(void);
void lllioInitRefresher(void);

#endif /* LLLIO_H_ */