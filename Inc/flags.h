/*
 * ****************************************************
 * File:	flags.h
 * Project:	RF communication with STM32 and nRF24L01+
 * MCU:		STM32L053R8
 * Others:	nRF24L01+
 * Author:	Nikos Grigoriadis
 * Date:	01/01/2022
 * ****************************************************
 */

#ifndef FLAGS_H_
#define FLAGS_H_

#include <stdint.h>

//Flag
typedef union{
	uint16_t clear_flags;
	struct{
		uint8_t f0 : 1;		//Used in delay_ms function as barrier
		uint8_t f1 : 1;		//Used when interrupt from nrf occur
		uint8_t f2 : 1;		//Used from transmit to receive and vise-versa
		uint8_t f3 : 1;		//Used to notify that the button has pressed
		uint8_t f4 : 1;		//Used in data transmit as barrier for RX/TX
		uint8_t f5 : 1;		//Key 1 flag.
		uint8_t f6 : 1;		//Key 2 flag.
		uint8_t f7 : 1;     //Key 3 flag.
		uint8_t f8 : 1;		//Sleep lcd flag.
		uint8_t f9 : 1;     //1 sec flag.
		uint8_t f10 : 1; 	//1 sec flag.
		uint8_t f11 : 1; 	//1 sec flag.
		uint8_t f12 : 1;
		uint8_t f13 : 1;	//Switch of led screen adter 3 past.
		uint8_t f14 : 1;	//Sound detection flag.
		uint8_t f15 : 1;    //Deep sleep mode.
		uint8_t f16 : 1;	//Switch ON/OFF main blue led.
		uint8_t f17 : 1;	//Key 4 flag.
		uint8_t f18 : 1;
		uint8_t f19 : 1;

	}fl;
} flag;

volatile flag flag0;

typedef union{
	uint16_t clear_keys;
	struct{
		uint8_t f0 : 1;
		uint8_t f1 : 1;
		uint8_t f2 : 1;
		uint8_t f3 : 1;
		uint8_t f4 : 1;
		uint8_t f5 : 1;
		uint8_t f6 : 1;
		uint8_t f7 : 1;
		uint8_t f8 : 1;
		uint8_t f9 : 1;
		uint8_t f10 : 1;
		uint8_t f11 : 1;

	}fl;
} key;

volatile key key0;




#endif /* FLAGS_H_ */
