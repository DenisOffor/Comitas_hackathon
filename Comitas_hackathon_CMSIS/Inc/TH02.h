
#ifndef TH02_H_
#define TH02_H_

#include "common.h"

#define TH_ADDRESS 0x40

void init_GPIO_for_I2C(void);
void I2C_for_TH_init(void);
void I2C_TH_SendByte(uint8_t addr, uint8_t byte);
void I2C_TH_Send_NBytes(uint8_t addr, uint8_t *data, uint8_t size);
void I2C_Get_Data(uint8_t addr, uint8_t *data, uint8_t size);
void TH_measure_temperature(void);

#endif /* TH02_H_ */
