#include "common.h"

Output_data output_data;
Program_state program_state;

void reset_output_data(){
	strcpy(output_data.string_comitas, "Comitas Akademya\r\n");
	output_data.temperature_DS18B20 = 0;
	output_data.temperature_TH02 = 0;
	output_data.humidity_TH02 = 0;
	
	program_state = MEASURE_TEMPERATURE;
}

