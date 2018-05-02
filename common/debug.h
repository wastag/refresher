//////////////////////////////////////////////////////////////////////////
/// \file debug.h
///
/// \brief Global switches container for debug purposes
///
///    All rights released
///
/// \author Jacek Wastag    jwastag@gmail.com
///
//////////////////////////////////////////////////////////////////////////


#ifndef DEBUG_H_
#define DEBUG_H_

#define DEBUG_MODE_ENABLED	1

#define DEBUG_FW_WATCHDOG	1
/*
#if DEBUG_FW_WATCHDOG
#define debugWatchdogPrintf(arg)	comPrintf(arg)
#else
#define debugWatchdogPrintf(arg)
#endif
*/
#define INT_TIME_MEAS_DISABLE	0
#define INT0_ISR_NR				1
#define INT1_ISR_NR				2
#define INT2_ISR_NR				3
#define INT3_ISR_NR				4
#define INT6_ISR_NR				7
#define PCINT0_ISR_NR			9
#define USB_GENERAL_ISR_NR		10
#define USB_ENDPOINT_ISR_NR		11
#define WDT_ISR_NR				12
#define TIMER1_CAPT_ISR_NR		16
#define TIMER1_COMPA_ISR_NR		17
#define TIMER1_COMPB_ISR_NR		18
#define TIMER1_COMPC_ISR_NR		19
#define TIMER1_OVF_ISR_NR		20
#define TIMER0_COMPA_ISR_NR		21
#define TIMER0_COMPB_ISR_NR		22
#define TIMER0_OVF_ISR_NR		23
#define SPI_ISR_NR				24
#define USART1_RX_ISR_NR		25
#define USART1_UDRE_ISR_NR		26
#define USART1_TX_ISR_NR		27
#define ANALOG_COMP_ISR_NR		28
#define ADC_ISR_NR				29
#define EE_READY_ISR_NR			30
#define TIMER3_CAPT_ISR_NR		31
#define TIMER3_COMPA_ISR_NR		32
#define TIMER3_COMPB_ISR_NR		33
#define TIMER3_COMPC_ISR_NR		34
#define TIMER3_OVF_ISR_NR		35
#define TWI_ISR_NR				36
#define SPM_READY_ISR_NR		37
#define TIMER4_COMPA_ISR_NR		38
#define TIMER4_COMPB_ISR_NR		39
#define TIMER4_COMPD_ISR_NR		40
#define TIMER4_OVF_ISR_NR		41
#define TIMER4_FPF_ISR_NR		42

#if DEBUG_MODE_ENABLED
/// Set specific interrupt number to measure its time duration 
#define MEASURE_IRQ_TIME	USART1_RX_ISR_NR
#else
#define MEASURE_IRQ_TIME	INT_TIME_MEAS_DISABLE
#endif

#endif /* DEBUG_H_ */