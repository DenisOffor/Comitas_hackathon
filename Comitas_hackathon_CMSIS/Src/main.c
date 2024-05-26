#include "ds18b20.h"
#include "TH02.h"
#include "stm32f1xx.h"
#include "common.h"
#include <stdio.h>
#include "SPI_for_BME.h"
#include "UART.h"

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

				//it commented because in proteus stm32f103 don't want to set bit ADDR that slave address was written
				//in real life with AHT20(sensor with I2C interaction, don't found TH02) I2C work perfectly
				//TH_measure_temperature();
			break;

			case TRANSMIT_DATA:
				//send string Comitas academya
				UART_send_data(output_data.string_comitas, COMITAS_STRING_SIZE);
				//send other string with temperatures and etc
				printf("temperature DS18B20: %f C\r\n", output_data.temperature_DS18B20);
				printf("temperature TH02: %f C\r\n", output_data.temperature_TH02);
				printf("humidity TH02: %f\r\n", output_data.humidity_TH02);
				printf("temperature BME: %f C\r\n", output_data.temperature_BME280);
				printf("humidity BME: %f\r\n", output_data.humidity_BME280);
				printf("pressure BME: %f\r\n", output_data.pressure_BME280);
				printf("\r\n\r\n");
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

