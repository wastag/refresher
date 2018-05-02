//////////////////////////////////////////////////////////////////////////
/// \file lllio.c
///
/// \brief Basic low level I/O functionality implementation
///
///  All rights released
///
/// \author Jacek Wastag    jwastag@gmail.com
//////////////////////////////////////////////////////////////////////////

#include "typedefs.h"
#include "lllio.h"
#include "hardwareconfig.h"
#include <avr/io.h>

#define LLLIO_BUTTONS_BUFF_MASK		0x1F

static struct
{
	uint8_t button1Buffer;
	uint8_t button2Buffer;
	uint8_t isButton1Pressed		:1;
	uint8_t isButton2Pressed		:1;
	uint8_t areButtonsInitialized	:1;
	uint8_t							:5;
} buttonsCtrl = 
{
	0xFF,
	0xFF,
	FALSE,
	FALSE,
	TRUE,
};

//////////////////////////////////////////////////////////////////////////
/// \brief Initializes buttons handling
//////////////////////////////////////////////////////////////////////////
void lllioInitButtons(void)
{
	BUTTON_1_DIR &= ~(1<<BUTTON_1_PIN);
	BUTTON_1_PORT |= (1<<BUTTON_1_PIN);
	
	BUTTON_2_DIR &= ~(1<<BUTTON_2_PIN);
	BUTTON_2_PORT |= (1<<BUTTON_2_PIN);
	
	buttonsCtrl.areButtonsInitialized = TRUE;
}

//////////////////////////////////////////////////////////////////////////
/// \brief Returns status of the button 1
//////////////////////////////////////////////////////////////////////////
Button1State_t lllioGetButton1State(void)
{
	if(FALSE == buttonsCtrl.areButtonsInitialized)
	{
		return BUTTON_UNINIT;
	}

	if(TRUE == buttonsCtrl.isButton1Pressed)
	{
		return BUTTON_PRESSED;
	}
	return BUTTON_RELEASED;
}

//////////////////////////////////////////////////////////////////////////
/// \brief Returns status of the button 2
//////////////////////////////////////////////////////////////////////////
Button1State_t lllioGetButton2State(void)
{
	if(FALSE == buttonsCtrl.areButtonsInitialized)
	{
		return BUTTON_UNINIT;
	}

	if(TRUE == buttonsCtrl.isButton2Pressed)
	{
		return BUTTON_PRESSED;
	}
	return BUTTON_RELEASED;
}

//////////////////////////////////////////////////////////////////////////
/// \brief Filters buttons state
//////////////////////////////////////////////////////////////////////////
void lllioButtonsStateControl(void)
{
	if(FALSE == buttonsCtrl.areButtonsInitialized)
	{
		return;
	}

	buttonsCtrl.button1Buffer <<= 1;

	if(0 != (BUTTON_1_STATE & (1<<BUTTON_1_PIN)))
	{
		buttonsCtrl.button1Buffer |= 0x01;
	}
	
	buttonsCtrl.button2Buffer <<= 1;

	if(0 != (BUTTON_2_STATE & (1<<BUTTON_2_PIN)))
	{
		buttonsCtrl.button2Buffer |= 0x01;
	}

	if (TRUE == buttonsCtrl.isButton1Pressed)
	{
		if(LLLIO_BUTTONS_BUFF_MASK == (LLLIO_BUTTONS_BUFF_MASK & buttonsCtrl.button1Buffer))
		{
			buttonsCtrl.isButton1Pressed = FALSE;
		}
	}
	else
	{
		if(0 == (LLLIO_BUTTONS_BUFF_MASK & buttonsCtrl.button1Buffer))
		{
			buttonsCtrl.isButton1Pressed = TRUE;
		}
	}
	
	if (TRUE == buttonsCtrl.isButton2Pressed)
	{
		if(LLLIO_BUTTONS_BUFF_MASK == (LLLIO_BUTTONS_BUFF_MASK & buttonsCtrl.button2Buffer))
		{
			buttonsCtrl.isButton2Pressed = FALSE;			
		}
	}
	else
	{
		if(0 == (LLLIO_BUTTONS_BUFF_MASK & buttonsCtrl.button2Buffer))
		{
			buttonsCtrl.isButton2Pressed = TRUE;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/// \brief Initialize LEDs
//////////////////////////////////////////////////////////////////////////
void lllioInitLeds(void)
{
	lllioSetLEDState(GREEN_LED, LED_OFF);
	lllioSetLEDState(RED_LED, LED_OFF);
	RED_LED_DIR |= (1<<RED_LED_PIN);
	GREEN_LED_DIR |= (1<<GREEN_LED_PIN);
}

//////////////////////////////////////////////////////////////////////////
/// \brief Toggles red's LED state
//////////////////////////////////////////////////////////////////////////
void lllioToggleRedLed(void)
{
	RED_LED_PORT ^= (1<<RED_LED_PIN);
}

//////////////////////////////////////////////////////////////////////////
/// \brief Toggles green's LED state
//////////////////////////////////////////////////////////////////////////
void lllioToggleGreenLed(void)
{
	GREEN_LED_PORT ^= (1<<GREEN_LED_PIN);
}

//////////////////////////////////////////////////////////////////////////
/// \brief Sets a new LED state
///
///	\param id - GREEN_LED, \n
///				RED_LED, \n
/// \param action - LED_OFF, \n
///					LED_ON,
/// \return 0 - if OK, -1 - if wrong LED's id passed to the function
//////////////////////////////////////////////////////////////////////////
int16_t lllioSetLEDState(LedId_t id, LedAction_t action)
{
	if(LED_OFF == action)
	{
		switch(id)
		{
			case GREEN_LED:
				GREEN_LED_PORT |= (1<<GREEN_LED_PIN);
				break;
			case RED_LED:
				RED_LED_PORT |= (1<<RED_LED_PIN);
				break;
			default:
				return -1;
		}	
	}
	else
	{
		switch(id)
		{
			case GREEN_LED:
				GREEN_LED_PORT &= ~(1<<GREEN_LED_PIN);
				break;
			case RED_LED:
				RED_LED_PORT &= ~(1<<RED_LED_PIN);
				break;
			default:
				return -1;
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/// \brief 
//////////////////////////////////////////////////////////////////////////
void lllioRefresherOn(void)
{
	REFRESHER_PORT |= (1<<REFRESHER_PIN);
}

//////////////////////////////////////////////////////////////////////////
/// \brief
//////////////////////////////////////////////////////////////////////////
void lllioRefresherOff(void)
{
	REFRESHER_PORT &= ~(1<<REFRESHER_PIN);
}

//////////////////////////////////////////////////////////////////////////
/// \brief
//////////////////////////////////////////////////////////////////////////
void lllioInitRefresher(void)
{
	REFRESHER_PORT &= ~(1<<REFRESHER_PIN);
	REFRESHER_DIR |= (1<<REFRESHER_PIN);
}