/*
 * ****************************************************
 * File:	nvic.c
 * Project:	RF communication with STM32 and nRF24L01+
 * MCU:		STM32L053R8
 * Others:	nRF24L01+
 * Author:	Nikos Grigoriadis
 * Date:	01/01/2022
 * ****************************************************
 */
#include <stdio.h>
#include "pwr.h"
#include "nvic.h"
#include "spi.h"
#include "flags.h"
#include "gpio.h"
#include "nrf.h"
#include "init_system.h"
#include "i2c_lcd1602.h"
#include "keypad.h"


//Variables
uint32_t ms_time = 0; 			//Used by delay_ms function
uint16_t led_ms = 0;			//Time counter for blinking led
uint8_t button_counter = 0;		//Button sampling counter
uint8_t key1_counter = 0;		//Key 1 sampling counter.
uint8_t key2_counter = 0;		//Key 2 sampling counter.
uint8_t key3_counter = 0;		//Key 3 sampling counter.
uint8_t key4_counter = 0;
uint16_t count=0;
uint16_t col_c=0;
//**************************************************************************************************************************************************************

void init_external_irq(void){

	//Enable System configuration controller
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);

	//Enable external interrupt from line 8
	SET_BIT(EXTI->IMR, EXTI_IMR_IM9);
	//Enable interrupt on falling edge from line 8
	SET_BIT(EXTI->FTSR, EXTI_FTSR_FT9);
	//Configure PA8 as interrupt source
	MODIFY_REG(SYSCFG->EXTICR[2],SYSCFG_EXTICR3_EXTI9, SYSCFG_EXTICR3_EXTI9_PB);

	//Enable EXTI4_15 Interrupt
	NVIC_EnableIRQ(EXTI4_15_IRQn);
}
//**************************************************************************************************************************************************************
void init_external_irq_sound(void){

	/*
	 * Microphone is connected on PC3 pin.
	 * So I need to create an interrupt at this port(C) and pin number(3).
	 */

	//System configuration controller was enabled in init_external_irq()


	//Enable external interrupt from line 3
	SET_BIT(EXTI->IMR, EXTI_IMR_IM3);

	//Enable interrupt on falling edge from line 3
	SET_BIT(EXTI->FTSR, EXTI_FTSR_FT3);

	//Set the source input for exti3 interrupt
	MODIFY_REG(SYSCFG->EXTICR[0],SYSCFG_EXTICR1_EXTI3, SYSCFG_EXTICR1_EXTI3_PC);

	//Enable EXTI2_3 Interrupt
	NVIC_EnableIRQ(EXTI2_3_IRQn);

}
void SPI1_IRQHandler(void){

	if(READ_BIT(SPI1->SR, SPI_SR_RXNE)){
		SPI_RX_BUFFER[SPI_RX_COUNTER] = SPI1->DR;
		SPI_RX_COUNTER ++;
	}
	if(READ_BIT(SPI1->SR, SPI_SR_TXE)){
		if(!SPI_TX_SIZE){
			CSN_PIN_DIS();
			CLEAR_BIT(SPI1->CR2, SPI_CR2_TXEIE);
		}else{
			WRITE_REG(SPI1->DR, SPI_TX_BUFFER[SPI_TX_COUNTER]);
			SPI_TX_SIZE --;
			SPI_TX_COUNTER ++;
		}
	}
}

//**************************************************************************************************************************************************************

void EXTI2_3_IRQHandler(void){

	//Interrupt on line 3
	if(READ_BIT(EXTI->PR, EXTI_PR_PIF3)){

		flag0.fl.f1 = 1;
		flag0.fl.f14=1;

		SET_BIT(EXTI->PR,EXTI_PR_PIF3);

	}

}
void EXTI4_15_IRQHandler(void){

	//Interrupt on line 8
	if(READ_BIT(EXTI->PR, EXTI_PR_PIF9)){

		flag0.fl.f1 = 1;

		//Clear flag
		SET_BIT(EXTI->PR, EXTI_PR_PIF9);


	}

}

//**************************************************************************************************************************************************************

void SysTick_Handler(void){


	//------------------------------
	//delay_ms() function
	if(flag0.fl.f0)
		ms_time ++;
	//------------------------------

	//------------------------------
	//Blinking LED
	if (flag0.fl.f16){
		led_ms++;
		if(led_ms == 970){
			LED_ON();
		}else if(led_ms == 1000){
			LED_OFF();
			led_ms = 0;
		}
	}

	//------------------------------

	//------------------------------
	//Button sampling
	if(!flag0.fl.f3){
		if(BUTTON_READ()){
			if(button_counter == 10)
				flag0.fl.f3 = 1;

			button_counter = 0;
		}else{
			if(button_counter < 10) //10 ms sampling
				button_counter ++;
		}
	}

	//------------------------------

	//------------------------------
	//key 1 sampling
	if(!flag0.fl.f5){
		if(!KEY1_READ()){
			if(key1_counter > 10)
				flag0.fl.f5 = 1; // R1 is HIGH
			key1_counter = 0;
		}else{
			if(key1_counter < 10) //10 ms sampling
				key1_counter ++;
		}
	}
	//------------------------------

	//------------------------------
	//key 2 sampling
	if(!flag0.fl.f6){
		if(!KEY2_READ()){
			if(key2_counter > 10)
				flag0.fl.f6 = 1; // R2 is HIGH
			key2_counter = 0;
		}else{
			if(key2_counter < 10) //10 ms sampling
				key2_counter ++;
		}
	}
	//------------------------------

	//------------------------------
	//key 3 sampling
	if(!flag0.fl.f7){
		if(!KEY3_READ()){
			if(key3_counter > 10)
				flag0.fl.f7 = 1; // R3 is HIGH
			key3_counter = 0;
		}else{
			if(key3_counter < 10) //10 ms sampling
				key3_counter ++;
		}
	}
	//------------------------------

	//key 4 sampling
	if(!flag0.fl.f17){
		if(!KEY4_READ()){
			if(key4_counter > 10)
				flag0.fl.f17 = 1; // R3 is HIGH
			key4_counter = 0;
		}else{
			if(key4_counter < 10) //10 ms sampling
				key4_counter ++;
		}
	}
	//------------------------------

	if (flag0.fl.f8){
		sleep_counter ++;
		if (sleep_counter == 1000){
			flag0.fl.f9=1; // Inform that 1sec past.
		}else if (sleep_counter==2000){
			flag0.fl.f10=1;
		}else if (sleep_counter==3000){
			flag0.fl.f11=1;
		}else if (sleep_counter==4000){
			flag0.fl.f12=1;
		}
	}
}

//**************************************************************************************************************************************************************

void TIM2_IRQHandler(void){

	if (TIM2->SR & TIM_SR_UIF ){
		//Clear Timer 2 Flag
		CLEAR_BIT(TIM2->SR, TIM_SR_UIF);
		count++;
		if(count == 50){
			col_c++;
		}else if(count == 330){
		//}else if(count == 1030){
			col_c++;
		}else if(count == 560){
		//}else if(count == 1960){
			col_c++;
		}else if(count == 790){
		//}else if(count == 2890){
			count = 0;
			col_c=0;
		}
		keypad_scanning(col_c);
	}

}

//**************************************************************************************************************************************************************

void delay_ms(uint32_t time){
	ms_time = 0;		//Clear time
	flag0.fl.f0 = 1; 	//Enable count flag
	while(ms_time < time);
//	{
//		CLRWDT();
//	}
	flag0.fl.f0 = 0;	//Disable count flag
}

//**************************************************************************************************************************************************************
