//////////////////////////////////////////////////////////////////////////
/// \file lllsys.h
///
/// \brief The header file for lll/sys module
///
///  All rights released
///
/// \author Jacek Wastag    jwastag@gmail.com
//////////////////////////////////////////////////////////////////////////

#ifndef LLLSYS_H_
#define LLLSYS_H_

#include "typedefs.h"

typedef enum SysTimerIDs_Tag
{
	TIMER_REFRESHER_RUN = 0,
	TIMER_REFRESHER_WAIT,
	TIMER_REFRESHER_MATURATION,
	
	SYS_TIMERS_NR
} SysTimerIDs_T;

typedef struct SysTimer_Tag
{
	uint32_t counter;
	uint8_t event;
} SysTimer_T;

typedef SysTimer_T SysTimersCtrlTable_T[SYS_TIMERS_NR];

void lllsysEnableInterrupts(void);
void lllsysDisableInterrupts(void);
void lllsysRegisterWakeUpCallback(pFunction_t __wakeUpCallback);
void lllsysSleepAndWakeUpInit(void);
void lllsysGoIntoSleepMode(void);
void lllsysSaveMCUstatus(void);
uint8_t lllsysGetMCUstatusCopy(void);
void lllsysRegisterTimersCtrlTable(SysTimersCtrlTable_T * p_timers_ctrl_tab);

#endif /* LLLSYS_H_ */