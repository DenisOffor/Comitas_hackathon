#include "ds18b20.h"
#include "UART_for_PC.h"
#include "TH02.h"
#include "SPI_for_TFT.h"

int __io_putchar(int ch) {
	uint8_t ch8 = ch & 0xFF;
	UART_send_data(ch8);
	return ch;
}

int main() {
	//StartHSE();
	//init_USART();
	//init_DS18B20();
	//I2C_for_TH_init();

	SPI1_Master_init();
	BME_init();

	while(1) {
		//DS18B20_measure_temperature();
		//TH_measure_temperature();
		//DS18B20_measure_temperature();
		//if(flag)
		//	printf(" temp: %f C\r\n", temperature);
	}
}

