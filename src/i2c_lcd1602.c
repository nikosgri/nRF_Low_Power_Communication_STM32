/*
 * i2c_lcd1602.c
 *      Author: Nick
 */
 

#include "i2c_lcd1602.h"
#include "init_system.h"
#include "nvic.h"

//Variables
static uint8_t PINS = 0; //Hold the status of the pins

//**************************************************************************************************************************************************************

//Local Functions based on LCD1602 datasheet
void init_i2c(void);
void i2c_write(void);
void lcd_send(uint8_t data_instr, uint8_t pins_data);
void return_home(void);
void entry_mode_set(uint8_t i_d, uint8_t s);
void display_on_off(uint8_t d, uint8_t c, uint8_t b);
void cursor_display_shift(uint8_t s_c, uint8_t r_l);
void function_set(uint8_t dl, uint8_t n, uint8_t f);
void set_cgram_address(uint8_t address);
void set_ddram_address(uint8_t address);
void write_ram(uint8_t data);

//**************************************************************************************************************************************************************

void init_lcd(void){

	init_i2c();

	delay_ms(200); //Power on delay

	PINS |= LED;  //LCD led

	return_home();
	delay_ms(2);
	function_set(0,1,0);
	delay_ms(2);
	display_on_off(1,0,0);
	delay_ms(2);
	clear_display();
	delay_ms(2);

}

//**************************************************************************************************************************************************************

//data_instr = 0 (Instruction), data_instr = 1(Data)
void lcd_send(uint8_t data_instr, uint8_t pins_data){


	if(!data_instr)
		PINS &= ~RS;
	else
		PINS |= RS;

	//Set MSB bits(DB7-DB4)
	PINS &= 0x0F;
	PINS |= (pins_data & 0xF0);
	PINS |= CS;
	i2c_write();
	delay_ms(1);
	PINS &= ~CS;
	i2c_write();

	delay_ms(2); //Need time to save first part of data

	//Set LSB bits(DB3-DB0)
	PINS &= 0x0F;
	PINS |= (pins_data << 4);
	PINS |= CS;
	i2c_write();
	delay_ms(1);
	PINS &= ~CS;
	i2c_write();

	delay_ms(2);  //Need time to save second part of data
}

//**************************************************************************************************************************************************************

void init_i2c(void){

	//Reset I2C1
	SET_BIT(RCC->APB1RSTR, RCC_APB1RSTR_I2C1RST);
	CLEAR_BIT(RCC->APB1RSTR, RCC_APB1RSTR_I2C1RST);

	//Enable I2C Clock
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C1EN);


	SET_BIT(I2C1->CR2, I2C_CR2_AUTOEND); //Automatic end

	MODIFY_REG(I2C1->CR2, I2C_CR2_SADD, (PCF8574_ADDRESS << I2C_CR2_SADD_Pos));

	WRITE_REG(I2C1->TIMINGR, 0x00805C89);	//For 32MHz clock

	SET_BIT(I2C1->CR1, I2C_CR1_PE); //Peripheral enable

}

void i2c_write(void){

	while(READ_BIT(I2C1->ISR, I2C_ISR_BUSY));

	MODIFY_REG(I2C1->CR2, I2C_CR2_NBYTES, (0x01 << I2C_CR2_NBYTES_Pos)); //Set how much data bytes will be send

	CLEAR_BIT(I2C1->CR2, I2C_CR2_RD_WRN); //Set transfer direction to write

	WRITE_REG(I2C1->TXDR, PINS);	//Write data

	SET_BIT(I2C1->CR2, I2C_CR2_START); //Start generation
}

//**************************************************************************************************************************************************************

void lcd_write_string(uint8_t *st, uint8_t st_size, uint8_t line, uint8_t pos){

	//Set Line and position
	if(line == 1){
		set_ddram_address(pos);
	}else{
		set_ddram_address(0x40 + pos);
	}

	for(uint8_t i=0;i<st_size;i++){
		write_ram(st[i]);
	}

}

void lcd_write_char(uint8_t character, uint8_t line, uint8_t pos){

	//Set Line and position
	if(line == 1){
		set_ddram_address(pos);
	}else{
		set_ddram_address(0x40 + pos);
	}

	write_ram(character);


}
//**************************************************************************************************************************************************************
//Mini LCD1602 Functions base on LCD1602 datasheet

void clear_display(void){
	lcd_send(0,0x01);
}

void return_home(void){
	lcd_send(0,0x02);
}

void entry_mode_set(uint8_t i_d, uint8_t s){
	uint8_t data = 0x04;
	if(i_d)
		data |= 0x02;
	if(s)
		data |= 0x01;
	lcd_send(0, data);
}

void display_on_off(uint8_t d, uint8_t c, uint8_t b){
	uint8_t data = 0x08;
	if(d)
		data |= 0x04;
	if(c)
		data |= 0x02;
	if(b)
		data |= 0x01;
	lcd_send(0, data);
}

void cursor_display_shift(uint8_t s_c, uint8_t r_l){
	uint8_t data = 0x10;
	if(s_c)
		data |= 0x08;
	if(r_l)
		data |= 0x04;
	lcd_send(0, data);
}

void function_set(uint8_t dl, uint8_t n, uint8_t f){
	uint8_t data = 0x20;
	if(dl)
		data |= 0x10;
	if(n)
		data |= 0x08;
	if(f)
		data |= 0x04;
	lcd_send(0, data);
}

void set_cgram_address(uint8_t address){
	uint8_t data = 0x40 | address;
	lcd_send(0, data);
}

void set_ddram_address(uint8_t address){
	uint8_t data = 0x80 | address;
	lcd_send(0, data);
}

void write_ram(uint8_t data){
	lcd_send(1, data);
}

void lcd_led_off(void){
	CLEAR_BIT(PINS,LED);
	i2c_write();
}


void lcd_led_on(void){
	SET_BIT(PINS,LED);
	i2c_write();
}

//**************************************************************************************************************************************************************
