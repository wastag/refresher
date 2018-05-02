/*
 * hardwareconfig.h
 *
 * Created: 2013-10-15 18:17:22
 *  Author: Administrator
 */ 

#ifndef HARDWARECONFIG_H_
#define HARDWARECONFIG_H_

#define GREEN_LED_DIR		DDRC
#define GREEN_LED_PORT		PORTC
#define GREEN_LED_PIN		PC7

#define RED_LED_DIR			DDRE
#define RED_LED_PORT		PORTE
#define RED_LED_PIN			PE2

#define BUTTON_1_DIR		DDRD
#define BUTTON_1_PORT		PORTD
#define BUTTON_1_PIN 		PD4
#define BUTTON_1_STATE		PIND

#define BUTTON_2_DIR		DDRB
#define BUTTON_2_PORT		PORTB
#define BUTTON_2_PIN 		PB7
#define BUTTON_2_STATE		PINB

#define LOOP_MEASURE_DIR	DDRF
#define LOOP_MEASURE_PORT	PORTF
#define LOOP_MEASURE_PIN	PF0

#define ISR_MEASURE_DIR		DDRF
#define ISR_MEASURE_PORT	PORTF
#define ISR_MEASURE_PIN		PF1

#define REFRESHER_DIR       DDRC
#define REFRESHER_PORT      PORTC
#define REFRESHER_PIN       PC6

#define RC5_DIR				DDRE
#define RC5_PORT			PORTE
#define RC5_PIN 			PE6
#define RC5_STATE			PINE

#define SDA_DIR             DDRD
#define SDA_PORT            PORTD
#define SDA_PIN             PD1

#define SCL_DIR             DDRD
#define SCL_PORT            PORTD
#define SCL_PIN             PD0

#endif /* HARDWARECONFIG_H_ */