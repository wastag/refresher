/*
 * sys.h
 *
 * Created: 2013-09-26 21:20:34
 *  Author: Administrator
 */ 


#ifndef SYS_H_
#define SYS_H_

#include "typedefs.h"
#include "lllsys.h" 

#define SYS_DISABLE_INTERRUPTS() lllsysDisableInterrupts()
#define SYS_ENABLE_INTERRUPTS()  lllsysEnableInterrupts()

void sysProcess10msLoop(void);

void SysSetTimer(uint8_t timer_id, uint32_t time_ms);
void SysCancelTimer(uint8_t timer_id);
Boolean_t SysGetTimerExpiredEvent(uint8_t timer_id);
uint32_t SysGetTimer(uint8_t timer_id);
void SysIntTimers(void);
SysTimersCtrlTable_T * sysGetTimersCtrlTableAddress(void);
void sysInit(void);

#endif /* SYS_H_ */