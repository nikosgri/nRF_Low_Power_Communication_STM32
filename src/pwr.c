/*
 * pwr.c
 *
 *  Created on: Feb 17, 2022
 *      Author: Medusa
 */

#include "nvic.h"
#include "spi.h"
#include "flags.h"
#include "gpio.h"
#include "i2c_lcd1602.h"
#include "nrf.h"
#include "init_system.h"

void pwr_enterSleepMode(void){

	/*Disable SysTick Interrupt.*/
	CLEAR_BIT(SysTick->CTRL,SysTick_CTRL_TICKINT_Msk);
	/*Clear SleepOnExit bit.*/
	CLEAR_BIT(SCB->SCR,SCB_SCR_SLEEPONEXIT_Msk);
	/*Clear Deep sleep bit.*/
	CLEAR_BIT(SCB->SCR,SCB_SCR_SLEEPDEEP_Msk);
	/*Enter into low power.*/
	__WFI(); /*Wait For Interrupt command.*/
	/*Resume SysTick Interrupt, on wake up.*/
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}
void reset_clock(){
	/*Testing if MSI is used as system clock*/
	if ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_MSI){
		/*Select PLL as system clock*/
		WRITE_REG(RCC->CFGR,RCC_CFGR_SW_PLL);
		/*Waiting for PLL to switced*/
		while (READ_BIT(RCC->CFGR,RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
	}
	SystemCoreClockUpdate();

}
void reduce_clock_speed(){

	/*Testing if PLL is used as system clock*/
	if ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_PLL){
		/*Select MSI as system clock*/
		WRITE_REG(RCC->CFGR,RCC_CFGR_SW_MSI);
		/*Waiting for PLL to switced*/
		while (READ_BIT(RCC->CFGR,RCC_CFGR_SWS) != RCC_CFGR_SWS_MSI);
	}
	SystemCoreClockUpdate();

}

void prepare_low_power(void){

	reduce_clock_speed();
	pwr_enterSleepMode();
	//configure_gpio_lpower();

}
//configure_gpio_lpower(){
//
//	CLEAR_BIT(RCC->IOPENR, RCC_IOPENR_IOPAEN); // Close port A
//	CLEAR_BIT(RCC->IOPENR, RCC_IOPENR_IOPCEN); // Close port C
//
//	/*Configure all GPIOB ports except PB9 as analog mode.
//
//
//}
void exit_low_power(){

	NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
	//EXTI->PR = EXTI_PR_PIF9;
	lcd_led_on();
	flag0.fl.f16=1;
	reset_clock();

}
