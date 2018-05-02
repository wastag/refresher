//////////////////////////////////////////////////////////////////////////
/// \file lllsys.c
///
/// \brief Methods for basic system control and configuration
///
///  All rights released
///
/// \author Jacek Wastag    jwastag@gmail.com
//////////////////////////////////////////////////////////////////////////

#include "typedefs.h"
#include "lllsys.h"
#include "debugio.h"
#include "hardwareconfig.h"
#include "lllio.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include "qpn.h"

static SysTimersCtrlTable_T * lllsys_timers_ctrl_tab_ptr = NULL_PTR;

//////////////////////////////////////////////////////////////////////////
/// \brief A dummy wake up callback
//////////////////////////////////////////////////////////////////////////
static void dummyWakeUpCallback(void){return;}

//////////////////////////////////////////////////////////////////////////
/// \brief The pointer to a wake up handler
//////////////////////////////////////////////////////////////////////////
static pFunction_t wakeUpCallback = dummyWakeUpCallback;

//////////////////////////////////////////////////////////////////////////
/// \brief     Registers a function to be called when during wake up
///
/// \param[in] pointer to the function to be called
/// \return    Nothing
//////////////////////////////////////////////////////////////////////////
void lllsysRegisterWakeUpCallback(pFunction_t __wakeUpCallback)
{
	wakeUpCallback = __wakeUpCallback;
}

void lllsysRegisterTimersCtrlTable(SysTimersCtrlTable_T * p_timers_ctrl_tab)
{
  lllsys_timers_ctrl_tab_ptr = p_timers_ctrl_tab;
}

//////////////////////////////////////////////////////////////////////////
/// \brief Initializes miscellaneous peripherals 
///
/// \param None
/// \return Nothing
//////////////////////////////////////////////////////////////////////////
void lllsysInitOtherPeripherals(void)
{
	loopMeasurePinEnable();
	isrMeasurePinEnable();
}

//////////////////////////////////////////////////////////////////////////
/// \brief Enables interrupts globally
///
/// \param None
/// \return Nothing
//////////////////////////////////////////////////////////////////////////
void lllsysEnableInterrupts(void)
{
	sei();
}

//////////////////////////////////////////////////////////////////////////
/// \brief Disables globally interrupts
///
/// \param None
/// \return Nothing
//////////////////////////////////////////////////////////////////////////
void lllsysDisableInterrupts(void)
{
	cli();	
}

//////////////////////////////////////////////////////////////////////////
/// \brief Sets the sleep mode and the frequency of the wake up signal
///
/// \param None
/// \return Nothing
//////////////////////////////////////////////////////////////////////////
void lllsysSleepAndWakeUpInit(void)
{
	//set_sleep_mode(SLEEP_MODE_IDLE);

	TCCR3A = 0x00;
	
	// wake up every millisecond
	OCR3A = F_CPU/1000;

	TIMSK3 |= (1<<OCIE3A);
	TCCR3B = (1<<WGM32)|(1<<CS30); // CTC MODE; OCR3A TOP; CLK = F_CPU/1 - NO PRESCALING
}

//////////////////////////////////////////////////////////////////////////
/// \brief Forces the MCU to go into the previously set sleep mode
///
/// \param None
/// \return Nothing
//////////////////////////////////////////////////////////////////////////
void lllsysGoIntoSleepMode(void)
{
	sleep_enable();
	sleep_mode();
	sleep_disable();
}

//////////////////////////////////////////////////////////////////////////
/// \brief Match interrupt service routine
//////////////////////////////////////////////////////////////////////////
ISR(TIMER3_COMPA_vect)
{
	static uint8_t timer_idx;
#if (MEASURE_IRQ_TIME == TIMER3_COMPA_ISR_NR)
	isrMeasurePinHigh();
#endif

    QF_tickXISR(0U); /* process time events for rate 0 */
	QF_tickXISR(1U); /* process time events for rate 1 */
	
	if(NULL_PTR != lllsys_timers_ctrl_tab_ptr)
	{
	  for (timer_idx=0;timer_idx<SYS_TIMERS_NR;timer_idx++)
	  {
		  if (0<(*lllsys_timers_ctrl_tab_ptr)[timer_idx].counter)
		  {
            (*lllsys_timers_ctrl_tab_ptr)[timer_idx].counter--;
		  }
	  }
	}

    (*wakeUpCallback)();

#if (MEASURE_IRQ_TIME == TIMER3_COMPA_ISR_NR)
	isrMeasurePinLow();
#endif
}

ISR(PCINT0_vect)
{
	
}