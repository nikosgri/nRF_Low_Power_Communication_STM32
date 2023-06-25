/*
 * ****************************************************
 * File:	nvic.h
 * Project:	RF communication with STM32 and nRF24L01+
 * MCU:		STM32L053R8
 * Others:	nRF24L01+
 * Author:	Nikos Grigoriadis
 * Date:	01/01/2022
 * ****************************************************
 */
 
#ifndef NVIC_H_
#define NVIC_H_

#include "CMSIS/stm32l0xx.h"

typedef struct col_counter{
	uint8_t column_counter;
	uint8_t col;
}col_counter_t;

col_counter_t cl_counter;

uint16_t sleep_counter;

/**
 * 	@function init_external_irq
 *
 *	 @brief Initialize the external interrupts
 *
 * 	@return void
 */
void init_external_irq(void);

/**
 * 	@function delay_ms
 *
 *	 @brief Delay an amount of milliseconds
 *
 * 	@return void
 */
void delay_ms(uint32_t time);
void init_external_irq_sound(void);
void wakeup_interrupt_button(void);

#endif /* NVIC_H_ */
