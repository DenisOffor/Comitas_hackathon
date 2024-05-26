#include "ds18b20.h"


DS18B20_CMD ds18b20_cmd = TEMPERATURE_CONVERTING;
uint8_t ds_buff[9];
uint16_t temp;

//timer 2 used for program state switch and
void TIM2_IRQHandler() {
	TIM2->SR &= ~TIM_SR_UIF;
	TIM2->CR1 &= ~TIM_CR1_CEN;
	program_state = TRANSMIT_DATA;
	ds18b20_cmd = TEMPERATURE_READING;
}

//there many defines for DS18B20 init for quick GPIO and PIN change if needed
void init_Gpio_for_ds() {
	//PIN for Data line (DQ) of One-wire interface
	RCC->APB2ENR |= RCC_PORT_DS18B20;
	PORT_DS18B20->CRH |= GPIO_MODE_DS18B20;
	GPIOA->CRH &= ~GPIO_CRH_CNF12;
	PORT_DS18B20->CRH |= GPIO_OPEN_DRAIN_DS18B20;
	PORT_DS18B20->ODR |= (1 << PIN_DS18B20);
	
	//PIN for LED, which will signalize about error
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	GPIOA->CRH |= GPIO_CRH_MODE11;
	GPIOA->CRH &= ~GPIO_CRH_CNF11;
}

//timer for generate 1us
void init_tim3_for_us() {
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->ARR = 1000;
	TIM3->PSC = 23;
	TIM3->CR1 |= TIM_CR1_CEN;
} 

//timer for delay between measurements (750 ms needed in max resolution)
void init_tim2_for_delay() {
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->ARR = 8000;
	TIM2->PSC = 2499;
	TIM2->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_SetPriority(TIM2_IRQn, 1);
	TIM2->CR1 |= TIM_CR1_CEN;
}

void ds_reset_pulse()
{
	//line should be in high state
	if((PORT_DS18B20->IDR & GPIO_IDR_IDR12) == 0) {
		ds18b20_cmd = ERROR_WITH_SENSOR;
		return;
	}
	TIM3->CNT = 0;
	//make the line go to low and wait in this state 480-960 us
	PORT_DS18B20->ODR &= ~(1 << PIN_DS18B20);
	while(TIM3->CNT < 480);
	//release the line (let go it to high) and wait to respond of DS18B20 after 15-60 us of wait
	PORT_DS18B20->ODR |= (1 << PIN_DS18B20);
	while(TIM3->CNT < 520);
	//if line was in high state - there are no answer from DS18B20 :(
	if(PORT_DS18B20->IDR & GPIO_IDR_IDR12) {
		ds18b20_cmd = ERROR_WITH_SENSOR;
		return;
	}
	//if we here then no error was detected
	PORT_LED->ODR &= ~(1 << PIN_LED);
	//wait minimum 480us from moment, where we release the line
	while(TIM3->CNT < 960);
}

void ds_write_bit(uint8_t bit)
{
	TIM3->CNT = 0;
	//make the line go to low and wait in this state at least 1 us
  PORT_DS18B20->ODR &= ~(1 << PIN_DS18B20);
  while(TIM3->CNT < 2) {}
  //set bit on line
	if(bit)
		PORT_DS18B20->ODR |= (1 << PIN_DS18B20);
  //"write operation" with bit last 60-120us
  while(TIM3->CNT < 60) {}
  //release line again
  PORT_DS18B20->ODR |= (1 << PIN_DS18B20);
}

void ds_write_byte(uint8_t byte)
{
	for(uint8_t i = 0; i < 8; i++)
		ds_write_bit(byte & (1 << i));
}

uint8_t ds_read_bit()
{
	uint8_t result;
	TIM3->CNT=0;
	//make the line go to low and wait in this state at least 1 us
	PORT_DS18B20->ODR &= ~(1 << PIN_DS18B20);
	while(TIM3->CNT < 2) {}
	//release line in order to DS18B20 set bit on line
	PORT_DS18B20->ODR |=  (1 << PIN_DS18B20);
	//wait at least 15us before read bit
	while(TIM3->CNT < 20) {}
	//read bit
	result = (PORT_DS18B20->IDR & (1 << PIN_DS18B20)) >> PIN_DS18B20;
	while(TIM3->CNT < 60 ) {}
	//"read operation" with bit at least 60
	return result;
}

uint8_t ds_read_byte()
{
	uint8_t result = 0;
	for(uint8_t i = 0; i < 8; i++)
		result |= (ds_read_bit() << i);
	return result;
}

void TIM_delay_on() {
	TIM2->CNT = 0;
	TIM2->CR1 |= TIM_CR1_CEN;
}

void temperature_measurment_start() {
	ds_reset_pulse();
	if(ds18b20_cmd == ERROR_WITH_SENSOR)
		return;
	ds_write_byte(SKIP_ROM_ADR);
	ds_write_byte(CONVERT_TEMP);
	TIM_delay_on();
}

void temprepature_measurment_read() {
	ds_reset_pulse();
	if(ds18b20_cmd == ERROR_WITH_SENSOR)
		return;
	ds_write_byte(SKIP_ROM_ADR);
	ds_write_byte(READ_DATA);

	for(int i = 0; i < 9; i++ )
		ds_buff[i] = ds_read_byte();

	//convert temperature
	temp = ds_buff[1];
	temp = temp << 8;
	temp |= ds_buff[0];
	output_data.temperature_DS18B20 = (float)temp * 0.0625;
}

void init_DS18B20() {
	init_Gpio_for_ds();
	init_tim3_for_us();
	init_tim2_for_delay();
}

void DS18B20_measure_temperature() {
	switch(ds18b20_cmd) {
		case TEMPERATURE_CONVERTING:
			ds18b20_cmd = WAITING_1SEC;
			temperature_measurment_start();
			break;
		case TEMPERATURE_READING:
			ds18b20_cmd = TEMPERATURE_CONVERTING;
			temprepature_measurment_read();
			break;
		case ERROR_WITH_SENSOR:
			PORT_LED->ODR |= (1 << PIN_LED);
			ds18b20_cmd = TEMPERATURE_CONVERTING;
			break;
		case WAITING_1SEC:
		break;
		default: 
			ds18b20_cmd = TEMPERATURE_CONVERTING;
			break;
	}
}
