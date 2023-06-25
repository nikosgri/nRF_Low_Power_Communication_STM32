/*
 * ****************************************************
 * File:	gpio.h
 * Project:	RF communication with STM32 and nRF24L01+
 * MCU:		STM32L053R8
 * Others:	nRF24L01+
 * Author:	Nikos Grigoriadis
 * Date:	01/01/2022
 * ****************************************************
 */
 
#ifndef GPIO_H_
#define GPIO_H_

#include "CMSIS/stm32l0xx.h"


#define CSN_PIN_EN()		(CLEAR_BIT(GPIOC->ODR, GPIO_ODR_OD9))
#define CSN_PIN_DIS()		(SET_BIT(GPIOC->ODR, GPIO_ODR_OD9))
#define CE_ACTIVATE()		(SET_BIT(GPIOC->ODR, GPIO_ODR_OD7))
#define CE_DEACTIVATE()		(CLEAR_BIT(GPIOC->ODR, GPIO_ODR_OD7))
#define	LED_ON()			(SET_BIT(GPIOB->ODR, GPIO_ODR_OD8))
#define	LED_OFF()			(CLEAR_BIT(GPIOB->ODR, GPIO_ODR_OD8))
#define LED_CHANGE()		(GPIOB->ODR ^= GPIO_ODR_OD8)
#define BUTTON_READ()		(GPIOC->IDR & GPIO_IDR_ID13)
#define KEY1_READ()			(GPIOC->IDR & GPIO_IDR_ID0)
#define KEY2_READ()			(GPIOC->IDR & GPIO_IDR_ID1)
#define KEY3_READ()			(GPIOC->IDR & GPIO_IDR_ID2)
#define KEY4_READ()			(GPIOA->IDR & GPIO_IDR_ID0)

#define MICROPHONE_READ()	(GPIOC->IDR & GPIO_IDR_ID3)

/**
 * 	@function init_gpio
 *
 *	@brief Simple function initialize the pins of
 *	STM MCU.
 *
 * 	@return void
 */
void init_gpio(void);

#endif /* GPIO_H_ */
