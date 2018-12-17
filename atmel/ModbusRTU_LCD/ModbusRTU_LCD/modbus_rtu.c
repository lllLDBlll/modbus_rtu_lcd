/*
 * modbus.c
 *
 * Created: 14/12/2018 15:04:37
 *  Author: Leonardo D. Batista
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "modbus_rtu.h"
#include "avr_usart.h"
#include "avr_gpio.h"
#include "lcd.h"

#define DEBUG 1

FILE *lcd_stream;
FILE *debug;

typedef struct frame{
	uint8_t addr;
	uint8_t cmd;
	uint8_t reg[2];							//Big Endian
	uint8_t data[2];						//Big Endian
	uint8_t crc[2];							//Big Endian
};

void modbus_rtu_init(){
	lcd_stream = inic_stream();
	debug = get_usart_stream();
	inic_LCD_4bits();
	USART_Init(BAUD);
}

uint16_t modbus_rtu_read(){
	//frame_t *f = malloc(sizeof(frame_t));	
	frame_t f;
	fprintf(debug,"Modbus:\n\r");
	uint8_t buff[9];
	
	for(uint8_t i=0; i<8; i++){
		buff[i] = USART_rx();		//Receive one byte
	}

	f.addr = buff[0];
	f.cmd = buff[1];
	//f.reg = (buff[2] << 8) | buff[3];
	f.reg[0] = buff[2];
	f.reg[1] = buff[3];	
	f.data[0] = buff[4];
	f.data[1] = buff[5];
	//f.crc = (buff[6] << 8) | buff[7];
	f.crc[0] = buff[6];
	f.crc[1] = buff[7];
	
#if DEBUG
	fprintf(debug,"Addr: %c\n\r", f.addr);
	fprintf(debug,"Cmd: %c\n\r", f.cmd);
	fprintf(debug,"Reg: %c", f.reg[0]);
	fprintf(debug,"%c\n\r", f.reg[1]);
	fprintf(debug,"Data: %c", f.data[0]);
	fprintf(debug,"%c\n\r", f.data[1]);
	fprintf(debug,"CRC: %c", f.crc[0]);
	fprintf(debug,"%c\n\r", f.crc[1]);
	fprintf(debug,"%s\r", &buff);
	nibble_data(f.data);
	fprintf(debug,"Data: %c", f.data[0]);
	fprintf(debug,"%c\n\r", f.data[1]);
#endif
}

void nibble_data(uint8_t *data){
	uint8_t tmp;
	tmp = data[0];
	data[0] = data[1];
	data[1] = tmp;
}

uint16_t CRC16_2(uint8_t *buf, int len){
	uint32_t crc = 0xFFFF;
	int i;
	
	for (i = 0; i < len; i++){
		
		crc ^= (uint16_t)buf[i];			// XOR byte into least sig. byte of crc
		
		for (int i = 8; i != 0; i--) {		// Loop over each bit
			if ((crc & 0x0001) != 0) {		// If the LSB is set
				crc >>= 1;					// Shift right and XOR 0xA001
				crc ^= 0xA001;
			}
			else // Else LSB is not set
				crc >>= 1; // Just shift right
		}
	}
	return crc;
}