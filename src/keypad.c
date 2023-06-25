/*
 * keypad.c
 *
 *  Created on: Mar 5, 2022
 *      Author: Medusa
 */


#define ROWS 		4
#define COLS 		3


#include "CMSIS/stm32l0xx.h"
#include "flags.h"
#include "init_system.h"
#include "gpio.h"
#include "pwr.h"
#include "nvic.h"
#include "spi.h"

#include "timers.h"
#include "nrf.h"
#include "i2c_lcd1602.h"


void keypad_scanning(uint8_t key){

	switch(key){
	case 1:
		SET_BIT(GPIOB->BSRR,GPIO_BSRR_BS_0);  // C1 high.
		SET_BIT(GPIOA->BSRR,GPIO_BSRR_BR_1); // C2 low.
		SET_BIT(GPIOA->BSRR,GPIO_BSRR_BR_4); // C3 low.
		if (KEY1_READ()){
			key0.fl.f1=1;
		}else if (KEY2_READ()){
			key0.fl.f4=1;
		}else if (KEY3_READ()){
			key0.fl.f7=1;
		}else if (KEY4_READ()){
			key0.fl.f10=1;
		}
		break;
	case 2:
		SET_BIT(GPIOB->BSRR,GPIO_BSRR_BR_0); // C1 low.
		SET_BIT(GPIOA->BSRR,GPIO_BSRR_BS_1);    // C2 high.
		SET_BIT(GPIOA->BSRR,GPIO_BSRR_BR_4);  // C3 low.
		if (KEY1_READ()){
			key0.fl.f2=1;
		}else if (KEY2_READ()){
			key0.fl.f5=1;
		}else if (KEY3_READ()){
			key0.fl.f8=1;
		}else if (KEY4_READ()){
			key0.fl.f0=1;
		}
		break;
	case 3:
		SET_BIT(GPIOB->BSRR,GPIO_BSRR_BR_0); // C1 low.
		SET_BIT(GPIOA->BSRR,GPIO_BSRR_BR_1);    // C2 low.
		SET_BIT(GPIOA->BSRR,GPIO_BSRR_BS_4);  // C3 high.
		if (KEY1_READ()){
			key0.fl.f3=1;
		}else if (KEY2_READ()){
			key0.fl.f6=1;
		}else if (KEY3_READ()){
			key0.fl.f9=1;
		}else if (KEY4_READ()){
			key0.fl.f11=1;
		}
	}

}
