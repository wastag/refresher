/*
 * sys.c
 *
 * Created: 2013-09-26 21:20:17
 *  Author: Administrator
 */ 

#include "sys.h"
#include "keys.h"
#include "lllio.h"
#include "llli2c.h"
#include "expander.h"
#include "qassert.h"
#include "qepn.h"

#define SYS_BEGIN_10MS_LOOP		static int cntr1ms = 0; \
								if(10 == cntr1ms) \
								{

#define SYS_END_10MS_LOOP			cntr1ms = 0; \
								} \
								cntr1ms++;

static SysTimersCtrlTable_T sys_timers_ctrl;

SysTimersCtrlTable_T * sysGetTimersCtrlTableAddress(void)
{
	return &sys_timers_ctrl;
}


void sysInit(void)
{
	lllsysRegisterTimersCtrlTable(&sys_timers_ctrl);
	SysIntTimers();
	lllsysSleepAndWakeUpInit();
}

//////////////////////////////////////////////////////////////////////////
/// \brief 
//////////////////////////////////////////////////////////////////////////
void sysProcess10msLoop(void)
{
	SYS_BEGIN_10MS_LOOP
	{
		lllioButtonsStateControl();
		keysProcess();
		expanderPollCommandsToExecute();
		i2cTimeOut10msCtrl();
	}
	SYS_END_10MS_LOOP
}

void SysIntTimers(void)
{
	uint8_t timer_id;
	
	for (timer_id=0;timer_id<(sizeof(sys_timers_ctrl)/sizeof(sys_timers_ctrl[0]));timer_id++)
	{
	  sys_timers_ctrl[timer_id].counter = 0;
	  sys_timers_ctrl[timer_id].event = 0;
	}
}

void SysSetTimer(uint8_t timer_id, uint32_t time_ms)
{
	SYS_DISABLE_INTERRUPTS();
	sys_timers_ctrl[timer_id].counter = time_ms;
	sys_timers_ctrl[timer_id].event = 0x01;
	SYS_ENABLE_INTERRUPTS();
}
void SysCancelTimer(uint8_t timer_id)
{
	SYS_DISABLE_INTERRUPTS();
	sys_timers_ctrl[timer_id].counter = 0x00000000;
	sys_timers_ctrl[timer_id].event = 0x00;
	SYS_ENABLE_INTERRUPTS();
}

Boolean_t SysGetTimerExpiredEvent(uint8_t timer_id)
{
	Boolean_t l_event = FALSE;

	SYS_DISABLE_INTERRUPTS();
	if ((0x00000000 == sys_timers_ctrl[timer_id].counter) &&
	    (0x00 != sys_timers_ctrl[timer_id].event) )
	{
	  sys_timers_ctrl[timer_id].event = 0x00;
	  l_event = TRUE;
	}
	SYS_ENABLE_INTERRUPTS();

	return l_event;
}

uint32_t SysGetTimer(uint8_t timer_id)
{
	uint32_t l_timer_value;
	
	SYS_DISABLE_INTERRUPTS();
	l_timer_value = sys_timers_ctrl[timer_id].counter;
	SYS_ENABLE_INTERRUPTS();
	
	return l_timer_value;
}


void Q_onAssert(char_t const Q_ROM * const module, int_t location)
{

}