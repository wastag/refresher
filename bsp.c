/*****************************************************************************
* Product: Blinky example, Arduino-UNO board, cooperative QV kernel, GNU-AVR
* Last Updated for Version: 5.4.0
* Date of the Last Update:  2015-04-08
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. state-machine.com.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* Web:   www.state-machine.com
* Email: info@state-machine.com
*****************************************************************************/
#include "qpn.h"
#include "blinky.h"
#include "bsp.h"
#include "lllrc5.h"
#include "lllsys.h"
#include "lllio.h"
#include "rc5.h"

/* the AVR device specific header <avr/io.h> is already included */
/* add other drivers if necessary... */

//Q_DEFINE_THIS_FILE

/* Local-scope objects -----------------------------------------------------*/
/* Arduino-UNO runs off the 16MHz oscillator */
#ifndef F_CPU
#define F_CPU   12000000UL
#endif

/* the on-board LED labeled "L" on Arduino-UNO (PORTB) */
#define LED_L   (1U << 2)

/* BSP functions ===========================================================*/
void BSP_init(void) {
    lllioInitLeds();
	lllioInitRefresher();
	rc5InitDecoder();	
	/* init RC5 receiver driver */
	lllrc5InitModule();

	lllsysSleepAndWakeUpInit();
	lllsysEnableInterrupts();
}

/*..........................................................................*/
void BSP_ledOff(void) {
    lllioSetLEDState(RED_LED, LED_OFF);
}
/*..........................................................................*/
void BSP_ledOn(void) {
    lllioSetLEDState(RED_LED, LED_ON);
}
/*..........................................................................*/
void BSP_terminate(int16_t result) {
    (void)result;
}


/* QF callbacks ============================================================*/
void QF_onStartup(void) {
    
}
/*..........................................................................*/
void QF_onCleanup(void) {
}

/*..........................................................................*/
void QV_onIdle(void) {   /* called with interrupts DISABLED, see NOTE1 */
    /* toggle the User LED, see NOTE2 , not enough LEDs to implement! */
    //PORTB |= LED_L;
    //PORTB &= ~LED_L;

#ifdef NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular AVR MCU.
    */
    SMCR = (0 << SM0) | (1 << SE); /* idle mode, adjust to your project */
    QV_CPU_SLEEP();  /* atomically go to sleep and enable interrupts */
#else
    QF_INT_ENABLE(); /* just enable interrupts */
#endif
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const file, int line) {
    /* implement the error-handling policy for your application!!! */
    QF_INT_DISABLE(); /* disable all interrupts */
    QF_RESET();  /* reset the CPU */
}

/*****************************************************************************
* NOTE01:
* The QV_onIdle() callback is called with interrupts disabled, because the
* determination of the idle condition might change by any interrupt posting
* an event. QV_onIdle() must internally enable interrupts, ideally
* atomically with putting the CPU to the power-saving mode.
*
* NOTE2:
* The User LED is used to visualize the idle loop activity. The brightness
* of the LED is proportional to the frequency of invcations of the idle loop.
* Please note that the LED is toggled with interrupts locked, so no interrupt
* execution time contributes to the brightness of the User LED.
*/
