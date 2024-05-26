#ifndef SPI_FOR_BME_H_
#define SPI_FOR_BME_H_

#include "common.h"

void DMA1_Channel2_IRQHandler(void);
void DMA1_Channel3_IRQHandler(void);

void init_GPIO_for_SPI(void);
void SPI1_Master_init(void);
void SPI_cs_set(void);
void SPI_cs_clear(void);
void DMA_for_SPI_init(void);
void SPI1_SendData_BME(uint8_t amount_of_byte);
void SPI1_GetData_BME(uint8_t amount_of_byte);
void BME_init(void);

#endif /* SPI_FOR_BME_H_ */
