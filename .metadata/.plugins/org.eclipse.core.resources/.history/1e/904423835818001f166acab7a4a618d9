#include "ds18b20.h"


DS18B20_CMD ds18b20_cmd = TEMPERATURE_CONVERTING;
uint8_t ds_buff[9];
uint16_t temp;
float temperature;

void TIM2_IRQHandler() {
	TIM2->SR &= ~TIM_SR_UIF;
	TIM2->CR1 &= ~TIM_CR1_CEN;
	ds18b20_cmd = TEMPERATURE_READING;
}

void init_Gpio_for_ds() {
	//PIN for Data line (DQ) of One-wire interface
	RCC->APB2ENR |= RCC_PORT_DS18B20;
	PORT_DS18B20->CRH |= GPIO_MODE_DS18B20;
	PORT_DS18B20->CRH |= GPIO_OPEN_DRAIN_DS18B20;
	PORT_DS18B20->ODR |= (1 << PIN_DS18B20);

	//PIN for LED, which will signalize about error
	RCC->APB2ENR |= RCC_PORT_LED;
	PORT_LED->CRH |= GPIO_MODE_LED;
	PORT_LED->CRH &= GPIO_PUSH_PULL_LED;
	//PORT_LED->ODR |= (1 << PIN_LED);
}

void init_tim1_for_us() {
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	TIM1->ARR = 1000;
	TIM1->PSC = 8;
}

void init_tim2_for_delay() {
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->ARR = 8000;
	TIM2->PSC = 1000;
	TIM2->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_SetPriority(TIM2_IRQn, 1);
	TIM2->CR1 = TIM_CR1_CEN;
}

void ds_reset_pulse()
{
	uint8_t result;
	//line should be in high state
	if((PORT_DS18B20->IDR & (1 << PIN_DS18B20)) == 0) {
		ds18b20_cmd = ERROR_WITH_SENSOR;
		return;
	}
	TIM1->CNT = 0;
	TIM1->CR1 |= TIM_CR1_CEN;
	//make the line go to low and wait in this state 480-960 us
	PORT_DS18B20->ODR &= ~(1 << PIN_DS18B20);
	while(TIM1->CNT < 480) {};
	//release the line (let go it to high) and wait to respond of DS18B20 after 15-60 us of wait
	PORT_DS18B20->ODR |= (1 << PIN_DS18B20);
	while(TIM1->CNT < 520) {};
	//read line in order to check answer from DS18B20
	result = PORT_DS18B20->IDR & (1 << PIN_DS18B20);
//	//wait minimum 480us from moment, where we release the line
	while(TIM1->CNT < 960) {};
//	//if line was in high state - there are no answer from DS18B20 :(
	if(result)
		ds18b20_cmd = ERROR_WITH_SENSOR;
	TIM1->CR1 &= ~TIM_CR1_CEN;
}

void ds_write_bit(uint8_t bit)
{
	TIM1->CNT = 0;
	TIM1->CR1 |= TIM_CR1_CEN;
	//make the line go to low and wait in this state at least 1 us
    PORT_DS18B20->ODR &= ~(1 << PIN_DS18B20);
    while(TIM1->CNT < 2) {};
    //set bit on line
    if(bit)
	  PORT_DS18B20->ODR |=  (1 << PIN_DS18B20);
    //"write operation" with bit last 60-120us
    while(TIM1->CNT < 60) {};
    //release line again
    PORT_DS18B20->ODR |=  (1 << PIN_DS18B20);
    TIM1->CR1 &= ~TIM_CR1_CEN;
}

void ds_write_byte(uint8_t byte)
{
	for(uint8_t i = 0; i < 8; i++)
		ds_write_bit(byte & (1 << i));
}

uint8_t ds_read_bit()
{
	uint8_t result;
	TIM1->CNT=0;
	TIM1->CR1 |= TIM_CR1_CEN;
	//make the line go to low and wait in this state at least 1 us
	PORT_DS18B20->ODR &= ~(1 << PIN_DS18B20);
	while(TIM1->CNT < 2) {};
	//release line in order to DS18B20 set bit on line
	PORT_DS18B20->ODR |=  (1 << PIN_DS18B20);
	//wait at least 15us before read bit
	while(TIM1->CNT < 15) {};
	//read bit
	result = (PORT_DS18B20->IDR & (1 << PIN_DS18B20)) >> PIN_DS18B20;
	while(TIM1->CNT < 60 ) {};
	//"read operation" with bit at least 60
	TIM1->CR1 &= ~TIM_CR1_CEN;
	return result;
}

uint8_t ds_read_byte()
{
	uint8_t result = 0;
	for(uint8_t i = 0; i < 8; i++)
		result |= (ds_read_bit() << i);
	return result;
}

void TIM_1sec_on() {
	TIM2->CNT = 0;
	TIM2->CR1 |= TIM_CR1_CEN;
}

void temperature_measurment_start() {
	ds_reset_pulse();
	if(ds18b20_cmd == ERROR_WITH_SENSOR)
		return;
	ds_write_byte(SKIP_ROM_ADR);
	ds_write_byte(CONVERT_TEMP);

	TIM_1sec_on();
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
	temperature = (float)temp * 0.0625;
}

void init_DS18B20() {
	init_Gpio_for_ds();
	init_tim1_for_us();
	init_tim2_for_delay();
}

void DS18B20_measure_temperature() {
	switch(ds18b20_cmd) {
		case TEMPERATURE_CONVERTING:
			ds18b20_cmd = WAITING_1SEC;
			temperature_measurment_start();
			break;
		case TEMPERATURE_READING:
			temprepature_measurment_read();
			ds18b20_cmd = TEMPERATURE_CONVERTING;
			break;
		case ERROR_WITH_SENSOR:
			PORT_LED->ODR |= (1 << PIN_LED);
			//ds18b20_cmd = TEMPERATURE_CONVERTING;
			break;
		case WAITING_1SEC:
		break;
		default: ds18b20_cmd = TEMPERATURE_CONVERTING;
			break;
	}
}
