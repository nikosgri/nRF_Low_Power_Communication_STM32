/*
 * ****************************************************
 * File:	spi.c
 * Project:	RF communication with STM32 and nRF24L01+
 * MCU:		STM32L053R8
 * Others:	nRF24L01+
 * Author:	Nikos Grigoriadis
 * Date:	01/01/2022
 * ****************************************************
 */


#include "spi.h"
#include "gpio.h"

//**************************************************************************************************************************************************************

void init_spi(void){

	//Enable SPI clock
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN);

	SET_BIT(SPI1->CR1, SPI_CR1_BR_0);										//fPCLK/4 = 32/4 = 8MHz

	SET_BIT(SPI1->CR1, SPI_CR1_MSTR); 										//Master mode
	SET_BIT(SPI1->CR1, (SPI_CR1_SSM | SPI_CR1_SSI)); 						//Software NSS
	SET_BIT(SPI1->CR2, SPI_CR2_RXNEIE); 									//RX interrupt enable

	SPI_RX_COUNTER = 0;
	SPI_TX_SIZE = 0;

	//Disable chip select pin
	CSN_PIN_DIS();

	//Enable SPI Interrupt
	NVIC_EnableIRQ(SPI1_IRQn);

	//Enable SPI module
	SET_BIT(SPI1->CR1, SPI_CR1_SPE);

}

//**************************************************************************************************************************************************************

uint8_t spi_transmit(uint8_t *buffer,uint16_t size){

	if(!(SPI1->SR & SPI_SR_BSY) && !SPI_TX_SIZE){

		//Copy data to buffer
		for(uint16_t i=1;i<size;i++){
			SPI_TX_BUFFER[i-1] = buffer[i];
		}

		//Data number
		SPI_TX_SIZE = size - 1;
		SPI_TX_COUNTER = 0;
		SPI_RX_COUNTER = 0;

		//Enable CSN pin
		CSN_PIN_EN();

		//Load register with the first byte to be transmitted
		WRITE_REG(SPI1->DR, buffer[0]);

		//Enable SPI TX interrupt
		SET_BIT(SPI1->CR2, SPI_CR2_TXEIE);

		return 1;

	}
	return 0;
}

//**************************************************************************************************************************************************************

void spi_transmit_wait(uint8_t *buffer,uint16_t size){
	while(!spi_transmit(buffer,size));
	while(SPI_TX_COUNTER < (size - 1));
}

//**************************************************************************************************************************************************************

void spi_transmit_wait_byte(uint8_t byte){
	uint8_t buffer[1];
	buffer[0] = byte;
	while(!spi_transmit(buffer,1));
}

//**************************************************************************************************************************************************************

void spi_clear_rx_buffer(void){
	SPI_RX_COUNTER = 0;
	for(uint16_t i=0;i<SPI_RX_BUFFER_SIZE;i++){
		SPI_RX_BUFFER[i] = 0;
	}
}
//**************************************************************************************************************************************************************

