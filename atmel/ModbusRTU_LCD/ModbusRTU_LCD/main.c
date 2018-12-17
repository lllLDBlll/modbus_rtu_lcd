/*
 * ModbusRTU_LCD.c
 *
 * Created: 14/12/2018 15:03:32
 *  Author: Leonardo D. Batista
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
/* Cabe�alhos e vetores de interrup��es */
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "modbus_rtu.h"
#include "avr_usart.h"
#include "avr_gpio.h"
#include "lcd.h"

int main(void){
	uint8_t x = 0;
	
	//FILE *lcd_stream = inic_stream();
 	//inic_LCD_4bits();// 		
// 	/* Obtem o stream de depura��o */
// 	FILE *debug = get_usart_stream();
// 
// 	/* Inicializa hardware da USART */
// 	USART_Init(B9600);
// 
// 	/* Mensagem incial: terminal do Proteus
// 	 * utiliza final de linha com '\r' */
// 	fprintf(debug,"Teste de debug\n\r");

	modbus_rtu_init();
	frame_t *f;
	
    while(1){
		/* Vai para primeira linha/coluna */
		cmd_LCD(0x80,0);
		/* Imprime msg */
		//fprintf(lcd_stream,"%d", x++);
		
// 		x = USART_rx();
// 		fprintf(debug,"%c\n\r", x);
		f = modbus_rtu_read();
		//fprintf(lcd_stream,"%s", "test123");
		//_delay_ms(1000);
		modbus_rtu_write(f,0);
		//_delay_ms(10);
    }
}
