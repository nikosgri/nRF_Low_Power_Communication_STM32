/*
 * ****************************************************
 * File:	timers.c
 * Project:	RF communication with STM32 and nRF24L01+
 * MCU:		STM32L053R8
 * Others:	nRF24L01+
 * Author:	Nikos Grigoriadis
 * Date:	01/01/2022
 * ****************************************************
 */
 

#include "init_system.h"
#include "CMSIS/stm32l0xx.h"
//**************************************************************************************************************************************************************

void init_emb_flash_mem(void){

	/*
	 *  ART ACCELARATOR PREFETCH ENABLE
	 *  LATENCY = 1 WS (Table 13 Reference manual RM0367)
	 */
	SET_BIT(FLASH->ACR, FLASH_ACR_LATENCY);
	SET_BIT(FLASH->ACR, FLASH_ACR_PRFTEN);

}

//**************************************************************************************************************************************************************

void init_rcc(void){


	//HSI16 & PLL => 32MHz clock
	RCC->CR |= RCC_CR_HSION; 											//Enable HSI16
	while(!(RCC->CR & RCC_CR_HSIRDY)); 									//Wait for HSI16 to be ready
	RCC->CR |= RCC_CR_HSIDIVEN; 										//Enable HSI16/4
	while(!(RCC->CR & RCC_CR_HSIDIVF)); 								//Wait for HSI16/4 = 4 MHz to be ready

	RCC->ICSCR = 0x0000B000; 											//No trimming

	RCC->CFGR |= RCC_CFGR_PLLDIV_0; 									//PLLDIV 4MHz/2 = 2MHz
	//RCC->CFGR |= (RCC_CFGR_PLLMUL_2 | RCC_CFGR_PLLMUL_0); 			//PLLMUL 2MHz*16 = 32MHz
	RCC->CFGR |= RCC_CFGR_PLLMUL_2 ; // 24MHz
	//RCC->CFGR |= (1U<<23);
	//RCC->CFGR |= (1U<<22);

	RCC->CR |= RCC_CR_PLLON; 											//Enable PLL
	while((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY); 					//Wait for PLL to be ready

	RCC->CFGR |= RCC_CFGR_SW;    										//PLL as system clock
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS); 					//Wait for PLL to be the system clock

	//Update SystemCoreClock variable
	SystemCoreClockUpdate();

}

//**************************************************************************************************************************************************************

void init_iwdg(void){

	//Unlock IWDG_PR, IWDG_RLR registers
	WRITE_REG(IWDG->KR, 0x5555UL);

	//Set prescaler to 16 => 32kHz/16 = 2kHz
	WRITE_REG(IWDG->PR, 0x02UL);

	//Set reload counter to 500
	//With 2kHz clock we have 500/2000 = 0.25 sec = 250 ms
	WRITE_REG(IWDG->RLR, 500);

	//Start the watchdog
	WRITE_REG(IWDG->KR, 0xCCCCUL);
}

//**************************************************************************************************************************************************************
