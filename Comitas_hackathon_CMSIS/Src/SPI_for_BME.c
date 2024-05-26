/*
 * SPI_for_TFT.c
 *
 *  Created on: 1 ���. 2023 �.
 *      Author: denlo
 */
#include <SPI_for_BME.h>

#define BUF_SIZE 255
#define BME280_REG_ID 0xD0

uint8_t buffer_SPI_TX[BUF_SIZE];
uint8_t buffer_SPI_RX[BUF_SIZE];

void DMA1_Channel2_IRQHandler() {
	DMA1->IFCR |= DMA_IFCR_CTCIF2;
	SPI_cs_set();
	DMA1_Channel2->CCR &= ~DMA_CCR_EN;
}

void DMA1_Channel3_IRQHandler() {
	DMA1->IFCR |= DMA_IFCR_CTCIF3;
	SPI_cs_set();
	DMA1_Channel3->CCR &= ~DMA_CCR_EN;
}

void init_GPIO_for_SPI() {
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	//PA5-SCK, PA6-MISO, PA7-MOSI
	GPIOA->CRL |= GPIO_CRL_MODE5 | GPIO_CRL_MODE6 | GPIO_CRL_MODE7;
	GPIOA->CRH |= GPIO_CRH_MODE8;
	//Clear CNF (because on f1 GPIO reset state isn't 0x0000...so strange)
	GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_CNF6 | GPIO_CRL_CNF7);
	GPIOA->CRH &= ~GPIO_CRH_CNF8;
	//AF on PA5-PA7
	GPIOA->CRL |= GPIO_CRL_CNF5_1 | GPIO_CRL_CNF6_1 | GPIO_CRL_CNF7_1;
	
	SPI_cs_set();
}

void SPI1_Master_init() {
	init_GPIO_for_SPI();
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	SPI1->CR1 &= ~SPI_CR1_SPE;
	SPI1->CR1 |= SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI;
	SPI1->CR2 |= SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN; 
	DMA_for_SPI_init();
	SPI1->CR1 |= SPI_CR1_SPE;
}

void SPI_cs_set() {
	GPIOA->ODR |= GPIO_ODR_ODR8;
}

void SPI_cs_clear() {
	GPIOA->ODR &= ~GPIO_ODR_ODR8;
}

void DMA_for_SPI_init() {
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	//2 CHANNEL - RX       3 CHANNEL - TX
	
	//3 channel - read from memory so set DIR bit
	DMA1_Channel3->CCR |= DMA_CCR_DIR | DMA_CCR_MINC | DMA_CCR_TCIE;
	DMA1_Channel2->CCR |= DMA_CCR_MINC | DMA_CCR_TCIE;
	
	DMA1_Channel2->CMAR = (uint32_t)(&buffer_SPI_RX[0]);
	DMA1_Channel2->CPAR = (uint32_t)(&(SPI1->DR));
	DMA1_Channel3->CNDTR = 0;
	DMA1_Channel3->CMAR = (uint32_t)(&buffer_SPI_TX[0]);
	DMA1_Channel3->CPAR = (uint32_t)(&(SPI1->DR));
  DMA1_Channel3->CNDTR = 0;

	SPI1->CR2 |= SPI_CR2_TXDMAEN;
	NVIC_EnableIRQ(DMA1_Channel2_IRQn);
	NVIC_SetPriority(DMA1_Channel2_IRQn, 1);
	NVIC_EnableIRQ(DMA1_Channel3_IRQn);
	NVIC_SetPriority(DMA1_Channel3_IRQn, 1);
}

void SPI1_SendData_BME(uint8_t amount_of_byte) {
	DMA1_Channel3->CCR &= ~DMA_CCR_EN;
	DMA1_Channel3->CNDTR = amount_of_byte;

	SPI_cs_clear();
	for(int i = 0; i < 10; i++);
	DMA1_Channel3->CCR |= DMA_CCR_EN;
}

void SPI1_GetData_BME(uint8_t amount_of_byte) {
	DMA1_Channel2->CCR &= ~DMA_CCR_EN;
	DMA1_Channel2->CNDTR = amount_of_byte;
	DMA1_Channel3->CCR &= ~DMA_CCR_EN;
	DMA1_Channel3->CNDTR = amount_of_byte;

	SPI_cs_clear();
	for(int i = 0; i < 10; i++);
	DMA1_Channel2->CCR |= DMA_CCR_EN;
	DMA1_Channel3->CCR |= DMA_CCR_EN;
}

void BME_init() {
	buffer_SPI_TX[0] = 0xE0;
	buffer_SPI_TX[1] = 0xB6;
	SPI1_SendData_BME(2);
	//Byte consist of the SPI reg address (without 8 bit) and read command(bit 8 - "1")
	buffer_SPI_TX[0] = BME280_REG_ID | 0x80;
	buffer_SPI_TX[1] = 0x00;
	SPI1_GetData_BME(2);
}
