#ifndef DS18B20_H_
#define DS18B20_H_

#include "common.h"

//"defines" for quick change of PIN for DQ line of 1-Wire interface
#define PORT_DS18B20 GPIOA
#define RCC_PORT_DS18B20 RCC_APB2ENR_IOPAEN
#define GPIO_MODE_DS18B20 GPIO_CRH_MODE12
#define GPIO_OPEN_DRAIN_DS18B20 GPIO_CRH_CNF12_0
#define PIN_DS18B20 12

//"defines" for quick change of PIN for LED, which will signalize about error
#define PORT_LED GPIOA
#define RCC_PORT_LED RCC_APB2ENR_IOPAEN
#define GPIO_MODE_LED GPIO_CRH_MODE11
#define PIN_LED 11

//commands which needed for chat with DS18B20
#define SKIP_ROM_ADR 0xCC
#define CONVERT_TEMP 0x44
#define READ_DATA 0xBE

//states for FSM ds18b20 measurement
typedef enum DS18B20_CMD{
	TEMPERATURE_CONVERTING = 0,
	TEMPERATURE_READING,
	WAITING_1SEC,
	ERROR_WITH_SENSOR //error state
} DS18B20_CMD;

extern DS18B20_CMD ds18b20_cmd;

//interrupts
void TIM2_IRQHandler(void);

//initialization of periphery
void init_DS18B20(void);
void init_Gpio_for_ds(void);
void init_tim3_for_us(void);
void init_tim2_for_delay(void);

//function for 1-Wire interface
void ds_reset_pulse(void);
void ds_write_bit(uint8_t bit);
uint8_t ds_read_bit(void);
void ds_write_byte(uint8_t byte);
uint8_t ds_read_byte(void);
void TIM_delay_on(void);

//function in order to make FSM beautiful :)
void temperature_measurment_start(void);
void temprepature_measurment_read(void);
//FSM here
void DS18B20_measure_temperature(void);

#endif /* DS18B20_H_ */
