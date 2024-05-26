#include "ds18b20.h"
#include "TH02.h"
#include "SPI_for_TFT.h"
#include "UART_for_PC.h"
#include "stm32f1xx.h"
#include "common.h"
#include <stdio.h>

int __io_putchar(int ch);

int main() {
	//SPI work in real life between MC, but don't work in PROTEUS, i don't have sensor to test either
	SPI1_Master_init();
	//I2C for TH02 initialization
	I2C_for_TH_init();
	//DS18B20 initialization
	init_DS18B20();
	//UART for data transmit initialization
	init_USART();
	//variables for temperatures and other data
	reset_output_data();
	//in proteus periphery didn't initialize by that moment, so need to little delay
	//in real life periphery don't have that problem
	for(int i = 0; i < 100000; i++);

	//there are two states, first - for temperature measure, second - for temperature output in terminal
	while(1) {
		switch(program_state) {
			case MEASURE_TEMPERATURE:
				DS18B20_measure_temperature();
				TH_measure_temperature();
			break;

			case TRANSMIT_DATA:
				//send string Comitas academya
				UART_send_data(output_data.string_comitas, COMITAS_STRING_SIZE);
				//send other string with temperatures and etc
				printf("temperature DS18B20: %f C\r\n", output_data.temperature_DS18B20);
				printf("temperature ATH20: %f C\r\n", output_data.temperature_TH02);
				program_state = MEASURE_TEMPERATURE;
			break;
		}
	}
}


int __io_putchar(int ch) {
	uint8_t ch8 = ch & 0xFF;
	UART_send_data(&ch8, 1);
	return ch;
}

