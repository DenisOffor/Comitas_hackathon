#include "ds18b20.h"
#include "TH02.h"
#include "UART_for_PC.h"
#include <stdio.h>

int __io_putchar(int ch) {
	uint8_t ch8 = ch & 0xFF;
	UART_send_data(ch8);
	return ch;
}

int main(void)
{
	init_DS18B20();
	init_USART();
	flag = 0;
	TIM_1sec_on();
	I2C_for_TH_init();
	I2C_TH_SendByte(0x40, 0x03);
	GPIOA->ODR |= GPIO_ODR_ODR11;
	//uint8_t data[2] = {0x03, 0x11 };
	//I2C_TH_Send_NBytes(0x40, &data[0], 2);
  while (1)
  {
	  DS18B20_measure_temperature();
	  if(flag == 1)
		  printf(" temp: %d C\r\n", (int8_t)(temperature));
  }

}
