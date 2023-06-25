/*
 * ****************************************************
 * File:	spi.h
 * Project:	RF communication with STM32 and nRF24L01+
 * MCU:		STM32L053R8
 * Others:	nRF24L01+
 * Author:	Nikos Grigoriadis
 * Date:	01/01/2022
 * ****************************************************
 */

#ifndef SPI_H_
#define SPI_H_

#include "CMSIS/stm32l0xx.h"


//Definitions of RX/TX buffers size
#define SPI_TX_BUFFER_SIZE  34
#define SPI_RX_BUFFER_SIZE  34
#define USER_BUFFER_SIZE    32

//RX/TX Buffers
uint8_t SPI_TX_BUFFER[SPI_TX_BUFFER_SIZE];
uint8_t SPI_RX_BUFFER[SPI_RX_BUFFER_SIZE];
uint8_t USER_BUFFER[USER_BUFFER_SIZE];

//Variables used in SPI TX/RX NVIC function for correct operation.
uint16_t SPI_TX_SIZE;
uint16_t SPI_TX_COUNTER;
uint16_t SPI_RX_COUNTER;

/**
 * 	@function init_spi
 *
 *	@brief Simple function initialise SPI.
 *
 * 	@return void
 */
void init_spi(void);

/**
 * 	@function spi_transmit
 *
 *	@brief Send via SPI the buffer in Interrupt mode,
 *	no need to wait.
 *
 * 	@return 1 if transmission begin, 0 if previous transmission is still on.
 */
uint8_t spi_transmit(uint8_t *buffer,uint16_t size);

/**
 * 	@function spi_transmit_wait
 *
 *	@brief Send via SPI the buffer and wait until all
 *	the buffer send.
 *
 * 	@return void
 */
void spi_transmit_wait(uint8_t *buffer,uint16_t size);

/**
 * 	@function spi_transmit_wait_byte
 *
 *	@brief Send via SPI a byte and wait until successfull send.
 *
 * 	@return void
 */
void spi_transmit_wait_byte(uint8_t byte);

/**
 * 	@function spi_clear_rx_buffer
 *
 *  @brief Fill with 0s the SPI_RX_BUFFER.
 *
 * 	@return void
 */
void spi_clear_rx_buffer(void);

#endif /* SPI_H_ */
