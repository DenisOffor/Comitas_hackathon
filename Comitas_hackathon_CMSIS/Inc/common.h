#ifndef COMMON_H_
#define COMMON_H_

#include "stm32f1xx.h"
#include "string.h"

#define COMITAS_STRING_SIZE 18

typedef enum Program_state {
	MEASURE_TEMPERATURE,
	TRANSMIT_DATA
}Program_state;

extern Program_state program_state;
	
typedef struct Output_data {
	float temperature_DS18B20;
	float	temperature_TH02;
	float	humidity_TH02;
	float 	temperature_BME280;
	float	humidity_BME280;
	float	pressure_BME280;
	uint8_t string_comitas[COMITAS_STRING_SIZE];
} Output_data;

extern Output_data output_data;

void reset_output_data(void);
#endif /* COMMON_H_ */
