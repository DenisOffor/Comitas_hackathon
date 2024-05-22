#include "ds18b20.h"

int main(void)
{
	init_DS18B20();

  while (1)
  {
	  DS18B20_measure_temperature();
  }

}
