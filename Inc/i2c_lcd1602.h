/**
 ******************************************************************************
 * File:	i2c_lcd1602.h
 * Project:	RF communication with STM32 and nRF24L01+
 * MCU:		STM32L053R8
 * Others:	nRF24L01+
 * Author:	Nikos Grigoriadis
 * Date:	01/01/2022
 ******************************************************************************
 */

#ifndef I2C_LCD1602_H_
#define I2C_LCD1602_H_

#include "CMSIS/stm32l0xx.h"

#define PCF8574_ADDRESS	0x4E	//Address based on A2,A1,A0 (internal pull ups)

//Pins
#define RS	0x01
#define RW	0x02
#define	CS	0x04
#define	LED	0x08

/**
 * 	@function init_lcd
 *
 *	 @brief Initialize LCD1602.
 *
 * 	@return void
 */
void init_lcd(void);

/**
 * 	@function lcd_write_string
 *
 *	 @brief Write a string(st) to LCD of size(st_size) at line(line) and position(pos).
 *
 * 	@return void
 */
void lcd_write_string(uint8_t *st, uint8_t st_size, uint8_t line, uint8_t pos);

/**
 * 	@function lcd_write_char
 *
 *	 @brief Write a character(character) to LCD at line(line) and position(pos).
 *
 * 	@return void
 */
void lcd_write_char(uint8_t character, uint8_t line, uint8_t pos);

/**
 * 	@function clear_display
 *
 *	 @brief Clear the display.
 *
 * 	@return void
 */
void clear_display(void);

void lcd_led_off(void);

void lcd_led_on(void);

#endif /* I2C_LCD1602_H_ */
