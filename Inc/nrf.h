/*
 * ****************************************************
 * File:	nrf.h
 * Project:	RF communication with STM32 and nRF24L01+
 * MCU:		STM32L053R8
 * Others:	nRF24L01+
 * Author:	Nikos Grigoriadis
 * Date:	01/01/2022
 * ****************************************************
 */

#ifndef NRF_H_
#define NRF_H_

#include "CMSIS/stm32l0xx.h"

//Define commands
#define R_REGISTER			0x00
#define W_REGISTER			0x20
#define R_RX_PAYLOAD		0x61
#define	W_TX_PAYLOAD		0xA0
#define	FLUSH_TX			0xE1
#define	FLUSH_RX			0xE2
#define REUSE_TX_PL			0xE3
#define	R_RX_PL_WID			0x60
#define	W_ACK_PAYLOAD		0xA8
#define	W_TX_PAYLOAD_NOACK	0xB0
#define	NOP					0xFF

//Register Address
#define CONFIG				0x00
#define	EN_AA				0x01
#define	EN_RXADDR			0x02
#define SETUP_AW			0x03
#define	SETUP_RETR			0x04
#define	RF_CH				0x05
#define RF_SETUP			0x06
#define	STATUS				0x07
#define	OBSERVE_TX			0x08
#define	RPD					0x09
#define	RX_ADDR_P0			0x0A
#define RX_ADDR_P1			0x0B
#define RX_ADDR_P2			0x0C
#define RX_ADDR_P3			0x0D
#define RX_ADDR_P4			0x0E
#define RX_ADDR_P5			0x0F
#define	TX_ADDR				0x10
#define	RX_PW_P0			0x11
#define	RX_PW_P1			0x12
#define	RX_PW_P2			0x13
#define	RX_PW_P3			0x14
#define	RX_PW_P4			0x15
#define	RX_PW_P5			0x16
#define	FIFO_STATUS			0x17
#define DYNPD				0x1C
#define	FEATURE				0x1D

//Needed for nrf_handler_data
#define DATA_RECEIVED		0x01
#define DATA_TX_OK			0x02
#define DATA_TX_ER			0x04

//Variables
uint8_t payload_data[33];	//Hold the received data

//Functions
uint8_t nrf_read_payload(void);
/**
 * 	@function nrf_handler_data
 *
 *	@brief Handler for data mode both RX/TX.
 *
 */
uint8_t nrf_handler(void);

/**
 * 	@function nrf_initialize_data
 *
 *	@brief Initialize nrf in data mode for both RX/TX.
 *
 */
void nrf_initialize(void);




/**
 * 	@function nrf_transmit_data
 *
 *	@brief Transmit data.
 *
 * 	@return void
 */
uint8_t nrf_transmit(uint8_t *buffer, uint8_t size);


/**
 * 	@function nrf_receive_data
 *
 *	@brief Handler for receiving data.
 *
 * 	@return void
 */
uint8_t nrf_receive(uint8_t size);



#endif /* NRF_H_ */
