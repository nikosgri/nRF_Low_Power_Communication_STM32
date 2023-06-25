/*
 * ****************************************************
 * File:	timers.h
 * Project:	RF communication with STM32 and nRF24L01+
 * MCU:		STM32L053R8
 * Others:	nRF24L01+
 * Author:	Nikos Grigoriadis
 * Date:	01/01/2022
 * ****************************************************
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include "CMSIS/stm32l0xx.h"

/**
 * 	@function init_timer2
 *
 *	@brief Initialize Timer 2
 *
 * 	@return void
 */
void init_timer2(void);

#endif /* TIMERS_H_ */
