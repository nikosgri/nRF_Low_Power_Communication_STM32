/*
 * ****************************************************
 * File:	nrf.c
 * Project:	RF communication with STM32 and nRF24L01+
 * MCU:		STM32L053R8
 * Others:	nRF24L01+
 * Author:	Nikos Grigoriadis
 * Date:	01/01/2022
 * ****************************************************
 */

#include "nrf.h"
#include "spi.h"
#include "flags.h"
#include "gpio.h"
#include "nvic.h"

//Variables
uint8_t nrf_rx_pipe0_address[4] = {(W_REGISTER |RX_ADDR_P0), 0xAB, 0xCD, 0xFE};		//PIPE0 ADDRESS
uint8_t nrf_tx_address[4] = {(W_REGISTER | TX_ADDR), 0xAB, 0xCD, 0xFE};				//TX ADDRESS

//Functions
void nrf_send_command(uint8_t command);					//Local function send command to nrf
void nrf_write_reg(uint8_t reg, uint8_t data);			//Local function write nrf register
uint8_t nrf_read_reg(uint8_t reg);						//Local function read nrf register
void nrf_write_payload(uint8_t *buffer, uint8_t size);	//Local function write nrf payload


//**************************************************************************************************************************************************************
//General functions
//**************************************************************************************************************************************************************
//**************************************************************************************************************************************************************
//**************************************************************************************************************************************************************

void nrf_send_command(uint8_t command){
	uint8_t buff[1] = {command};
	spi_transmit_wait(buff,1);
}

//**************************************************************************************************************************************************************

void nrf_write_reg(uint8_t reg, uint8_t data){

	uint8_t buff[2] = {(W_REGISTER | reg), data};
	spi_transmit_wait(buff,2);
}

//**************************************************************************************************************************************************************

uint8_t nrf_read_reg(uint8_t reg){

	uint8_t buff[2] = {(R_REGISTER | reg), NOP};
	spi_transmit_wait(buff,2);

	return SPI_RX_BUFFER[1];
}


//**************************************************************************************************************************************************************
//Data functions
uint8_t nrf_handler(void){

	uint8_t response = 0;

	if(flag0.fl.f1){

		flag0.fl.f1 = 0;	//Clear flag
		flag0.fl.f2 = 0;

		CE_DEACTIVATE();

		uint8_t status_reg = nrf_read_reg(STATUS);
		uint8_t RX_DR = (status_reg & 0x40) >> 6;
		uint8_t TX_DS = (status_reg & 0x20) >> 5;
		uint8_t MAX_RT = (status_reg & 0x10) >> 4;

		if(RX_DR){
			//RX FIFO, new data received. Read data.
			//uint8_t data_pipe_available = (status_reg & 0x0E) >> 1;
			nrf_read_payload();
			response = 1;
			if (flag0.fl.f18)
				flag0.fl.f18=0;
		}
		if(TX_DS){
			//Transmit successful. Continue to next data.
			response |= (1 << 1);
		}

		if(MAX_RT){
			//Transmit NOT successful. Retransmit the same or do something else.
			response |= (1 << 2);
			//Print error to LCD
		}


		nrf_write_reg(STATUS, 0x70);	//Clear interrupt flags
		flag0.fl.f4 = 0;					//Enable RX/TX again
	}

	return response;
}

//**************************************************************************************************************************************************************

void nrf_initialize(void){

	CE_DEACTIVATE();							//Disable RX/TX
	nrf_send_command(FLUSH_TX);					//Flush TX FIFO
	nrf_send_command(FLUSH_RX);					//Flush RX FIFO
	nrf_write_reg(STATUS, 0x70);				//Clear interrupt flags
	nrf_write_reg(CONFIG, 0x08); 				//Enable all interrupts, enable CRC, Power DOWN
	nrf_write_reg(EN_AA, 0x01);					//Enable auto-acknowledgment in data pipe 0
	nrf_write_reg(EN_RXADDR, 0x01); 			//Enable data pipe 0
	nrf_write_reg(SETUP_AW, 0x01);				//3 Bytes address
	nrf_write_reg(SETUP_RETR, 0x7F);			//Retransmit every 2000us = 2 ms, up to 15 times
	nrf_write_reg(RF_CH, 0x00);					//Frequency: 2.4GHz
	nrf_write_reg(RF_SETUP, 0x0E);				//2Mbps data rate, 0dBM (max) transmit power
	spi_transmit_wait(nrf_rx_pipe0_address, 4);	//Write RX pipe0 address
	spi_transmit_wait(nrf_tx_address, 4);		//Write TX address
	nrf_write_reg(FEATURE, 0x06);				//Enable dynamic payload, enable payload with ACK
	nrf_write_reg(DYNPD,0x01);					//Enable dynamic payload in data pipe 0

}


//**************************************************************************************************************************************************************
//TX
//**************************************************************************************************************************************************************
//**************************************************************************************************************************************************************

uint8_t nrf_transmit(uint8_t *buffer, uint8_t size){

	if(!flag0.fl.f4){
		CE_DEACTIVATE();
		nrf_write_reg(STATUS, 0x70); //Clear flags
		nrf_send_command(FLUSH_TX);
		nrf_write_payload(buffer, size);
		nrf_write_reg(CONFIG, 0x0A); 	//Enable all interrupts, enable CRC, Power UP in PTX Mode, disable PRIM_RX
		delay_ms(3);					//Delay 1.5ms(Start up)
		CE_ACTIVATE();					//Begin transmit
		flag0.fl.f4 = 1;
		return 1;
	}
	return 0;
}

//**************************************************************************************************************************************************************

//Size must be < 33
void nrf_write_payload(uint8_t *buffer, uint8_t size){

	//Copy data to local buffer
	uint8_t buff[33];
	buff[0] = W_TX_PAYLOAD;
	for(uint8_t i=1;i<(size+1);i++){
		buff[i] = buffer[i-1];
	}

	spi_transmit_wait(buff, size+1); //Write TX FIFO

}



//**************************************************************************************************************************************************************
//RX
//**************************************************************************************************************************************************************
//**************************************************************************************************************************************************************

//RX size must be equal to TX size
uint8_t nrf_receive(uint8_t size){
	if(!flag0.fl.f4){
		CE_DEACTIVATE();
		nrf_write_reg(RX_PW_P0, size);	//Set RX payload
		nrf_write_reg(CONFIG, 0x0B); 	//Enable all interrupts, enable CRC, Power UP and PRIM_RX in PRX Mode
		delay_ms(3);					//Delay 1.5ms(Start up)
		CE_ACTIVATE();					//Begin receive
		flag0.fl.f4 = 1;
		return 1;
	}
	return 0;
}

//**************************************************************************************************************************************************************

uint8_t nrf_read_payload(void){

	uint8_t payload_width = nrf_read_reg(R_RX_PL_WID);
	uint8_t i;

	if(payload_width > 0){
		payload_data[0] = R_RX_PAYLOAD;
		for(i=1;i<33;i++){
			payload_data[i] = NOP;
		}

		spi_transmit_wait(payload_data, payload_width+1);

		for(i=0;i<payload_width;i++){
			payload_data[i] = SPI_RX_BUFFER[i+1];
		}

		return payload_width;
	}
	return 0;
}

//**************************************************************************************************************************************************************

