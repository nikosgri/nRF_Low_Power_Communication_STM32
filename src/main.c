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
 

#include "CMSIS/stm32l0xx.h"
#include "flags.h"
#include "init_system.h"
#include "gpio.h"
#include "pwr.h"
#include "nvic.h"
#include "spi.h"
#include "keypad.h"
#include "timers.h"
#include "nrf.h"
#include "i2c_lcd1602.h"

#define HELLO_SCREEN 	0
#define RECV_SCREEN		1
#define	TX_SCREEN		2
#define ER_SCREEN		3

//Functions
void button_handler(void);
void sound_handler(void);

//Strings for LCD
uint8_t hello_st[6] = {'H','E','L','L','O','!'};
uint8_t recv_st[8] = {'R','E','C','E','I','V','E',':'};
uint8_t tx_st[6] = {'T','X',' ','O','K','!'};
uint8_t error_max_st[12] = { 'E','R','R','O','R',':','M','A','X',' ','R','T'};
uint8_t msg[1];
uint8_t sleep_st[15] = {'G','O','I','N','G',' ','T','O',' ','S','L','E','E','P',':'};
uint8_t sound_st[14] = {'S','O','U','N','D',' ','D','E','T','E','C','T','E','D'};
uint8_t lpwr_st[9] = {'L','O','W',' ','P','O','W','E','R'};
uint8_t sound[1];
uint8_t response;
uint8_t CURRENT_SCREEN = HELLO_SCREEN;
uint8_t counter_col=0;


//**************************************************************************************************************************************************************
int main(void){

	init_emb_flash_mem();					//Initialize latency WS
	init_rcc();								//Initialize clock (RCC)
	init_gpio();							//Initialize GPIOs
	init_spi();								//Initialize SPI
	SysTick_Config(SystemCoreClock/1000);	//1ms delay interrupt(SysTick Timer)
	init_external_irq();					//Initialize interrupts from external source
	init_timer2();
	init_external_irq_sound();



	flag0.clear_flags = 0;					//Clear flags
	key0.clear_keys = 0;					//Clear flags
	nrf_initialize();						//Initialize nRF24L01 for data mode.
	init_lcd();								//Initialize LCD
	sleep_counter = 0;

	delay_ms(200);							//Wait for NRF to power on(POwer on reset state)

	lcd_write_string(hello_st,6,1,0);



	flag0.fl.f16=1;
	while(1){

		//Receiver
		//---------------------

		if(!flag0.fl.f2)
			nrf_receive(1); //Get 1 byte

		response = nrf_handler();

		if(response & DATA_RECEIVED){

			if(CURRENT_SCREEN != RECV_SCREEN){
				clear_display();
				lcd_write_string(recv_st,8,1,0);
			}
			if (payload_data[0]!='S'){
				lcd_write_char(payload_data[0],1,9);
			}else{
				clear_display();
				lcd_write_string(sound_st,14,1,0);
			}
		}else if(response & DATA_TX_ER){
			if(CURRENT_SCREEN != ER_SCREEN){
				clear_display();
				lcd_write_string(error_max_st,12,1,0);
			}
		}else if(response & DATA_TX_OK){
			if(CURRENT_SCREEN != TX_SCREEN){
				clear_display();
				lcd_write_string(tx_st,6,1,0);
			}
		}



		if (flag0.fl.f8){ // Mean that I am in sleep mode.
			if (flag0.fl.f9){

				lcd_write_char('3',1,16);
				flag0.fl.f9=0;
			}else if(flag0.fl.f10){

				lcd_write_char('2',1,16);
				flag0.fl.f10=0;
			}else if(flag0.fl.f11){

				lcd_write_char('1',1,16);
				flag0.fl.f11=0;
			}else if(flag0.fl.f12){

				flag0.fl.f8=0;
				flag0.fl.f12=0;
				flag0.fl.f18=1; //sleep mode.
				flag0.fl.f16=0; // led off.
				sleep_counter=0;
			}
		}
		if (flag0.fl.f18){
			clear_display();
			lcd_led_off();
			prepare_low_power();

		}

		if (!flag0.fl.f18){
			exit_low_power();
		}


		sound_handler();
		button_handler();
		CLRWDT();
	}
}

//**************************************************************************************************************************************************************

void button_handler(void){

	if(flag0.fl.f3){

		flag0.fl.f8 = 1;
		flag0.fl.f14=0;
		//lcd_write_string(sleep_st,15,1,0);
		lcd_write_string(lpwr_st,9,1,0);
		//Clear flag
		flag0.fl.f3 = 0;
	}


	if (key0.fl.f0){
		flag0.fl.f4 = 0;
		msg[0]='0';
		nrf_transmit(msg,1);
		flag0.fl.f4 = 0;
		flag0.fl.f2 = 1;

		key0.fl.f0=0;

	}
	if (key0.fl.f1){
		flag0.fl.f4 = 0;
		msg[0]='1';
		nrf_transmit(msg,1);
		flag0.fl.f4 = 0;
		flag0.fl.f2 = 1;
		key0.fl.f1=0;

	}
	if (key0.fl.f2){
		flag0.fl.f4 = 0;
		msg[0]='2';
		nrf_transmit(msg,1);
		flag0.fl.f4 = 0;
		flag0.fl.f2 = 1;
		key0.fl.f2=0;

	}
	if (key0.fl.f3){
		flag0.fl.f4 = 0;
		msg[0]='3';
		nrf_transmit(msg,1);
		flag0.fl.f4 = 0;
		flag0.fl.f2 = 1;
		key0.fl.f3=0;

	}
	if (key0.fl.f4){
		flag0.fl.f4 = 0;
		msg[0]='4';
		nrf_transmit(msg,1);
		flag0.fl.f4 = 0;
		flag0.fl.f2 = 1;
		key0.fl.f4=0;

	}
	if (key0.fl.f5){
		flag0.fl.f4 = 0;
		msg[0]='5';
		nrf_transmit(msg,1);
		flag0.fl.f4 = 0;
		flag0.fl.f2 = 1;
		key0.fl.f5=0;

	}
	if (key0.fl.f6){
		flag0.fl.f4 = 0;
		msg[0]='6';
		nrf_transmit(msg,1);
		flag0.fl.f4 = 0;
		flag0.fl.f2 = 1;
		key0.fl.f6=0;

	}
	if (key0.fl.f7){
		flag0.fl.f4 = 0;
		msg[0]='7';
		nrf_transmit(msg,1);
		flag0.fl.f4 = 0;
		flag0.fl.f2 = 1;
		key0.fl.f7=0;

	}
	if (key0.fl.f8){
		flag0.fl.f4 = 0;
		msg[0]='8';
		nrf_transmit(msg,1);
		flag0.fl.f4 = 0;
		flag0.fl.f2 = 1;
		key0.fl.f8=0;

	}
	if (key0.fl.f9){
		flag0.fl.f4 = 0;
		msg[0]='9';
		nrf_transmit(msg,1);
		flag0.fl.f4 = 0;
		flag0.fl.f2 = 1;
		key0.fl.f9=0;

	}
	if (key0.fl.f10){
		flag0.fl.f4 = 0;
		msg[0]='*';
		nrf_transmit(msg,1);
		flag0.fl.f4 = 0;
		flag0.fl.f2 = 1;
		key0.fl.f10=0;

	}
	if (key0.fl.f11){
		flag0.fl.f4 = 0;
		msg[0]='#';
		nrf_transmit(msg,1);
		flag0.fl.f4 = 0;
		flag0.fl.f2 = 1;
		key0.fl.f11=0;

	}

}
void sound_handler(void){

	if(flag0.fl.f14){

		flag0.fl.f4 = 0;
		msg[0]='S';
		nrf_transmit(msg,1);
		flag0.fl.f4 = 0;
		flag0.fl.f2 = 1;

		flag0.fl.f14=0;
	}

}
//**************************************************************************************************************************************************************

