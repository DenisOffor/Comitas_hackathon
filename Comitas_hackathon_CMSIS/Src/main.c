#include "ds18b20.h"
#include "UART_for_PC.h"
#include "TH02.h"

int __io_putchar(int ch) {
	uint8_t ch8 = ch & 0xFF;
	UART_send_data(ch8);
	return ch;
}

int main() {
	//StartHSE();
	init_USART();
	init_DS18B20();
	I2C_for_TH_init();
	for(int i = 0;i < 10000; i++);

	 ds18b20_cmd = TEMPERATURE_CONVERTING;
	//I2C_TH_SendByte(0x20, 0x03);
	while(1) {
		DS18B20_measure_temperature();
		TH_measure_temperature();
		//DS18B20_measure_temperature();
		//if(flag)
		//	printf(" temp: %f C\r\n", temperature);
	}
}

