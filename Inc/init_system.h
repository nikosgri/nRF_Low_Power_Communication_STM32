/*
 * ****************************************************
 * File:	init_system.h
 * Project:	RF communication with STM32 and nRF24L01+
 * MCU:		STM32L053R8
 * Others:	nRF24L01+
 * Author:	Nikos Grigoriadis
 * Date:	01/01/2022
 * ****************************************************
 */

#ifndef INIT_SYSTEM_H_
#define INIT_SYSTEM_H_

#include "CMSIS/stm32l0xx.h"

//Clear WatchDog Timer Command
#define CLRWDT()	(IWDG->KR = 0xAAAAUL)


/**
 * 	@function init_emb_flash_mem
 *
 *	@brief Simple function initialize latency for CPU to
 *	wait for instruction fetch.
 *
 * 	@return void
 */
void init_emb_flash_mem(void);

/**
 * 	@function init_rcc
 *
 *	@brief Simple function initialize RCC (clocks) of
 *	STM MCU.
 *
 * 	@return void
 */
void init_rcc(void);

/**
 * 	@function init_iwdg
 *
 *	@brief Simple function initialize watchdog timer of
 *	STM MCU.
 *
 * 	@return void
 */
void init_iwdg(void);

#endif /* INIT_SYSTEM_H_ */
