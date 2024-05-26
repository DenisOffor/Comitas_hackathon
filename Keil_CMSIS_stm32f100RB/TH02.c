

#include "TH02.h"
uint8_t start_state = 0;

void init_GPIO_for_I2C() {
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	GPIOB->CRL &= ~GPIO_CRL_CNF6;
	GPIOB->CRL &= ~GPIO_CRL_CNF7;
	GPIOB->CRL |= GPIO_CRL_MODE6 | GPIO_CRL_MODE7 | GPIO_CRL_CNF6 | GPIO_CRL_CNF7;
}

void I2C_for_TH_init() {
	init_GPIO_for_I2C();

	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	I2C1->OAR2 &= ~I2C_OAR2_ENDUAL;
	I2C1->CR1 &= ~I2C_CR1_ENGC;
	I2C1->CR1 &= ~I2C_CR1_NOSTRETCH;
	I2C1->CR1 &= ~I2C_CR1_PE;

	I2C1->CR2 = I2C_CR2_FREQ_3; //8
	I2C1->CCR |= 0x28;
	I2C1->TRISE |= 0x0009;
	I2C1->CR1 |= 0x00000000;
	I2C1->CR1 |= I2C_CR1_PE;
}

void I2C_TH_SendByte(uint8_t addr, uint8_t byte) {
	I2C1->CR1 |= I2C_CR1_ACK;
	I2C1->CR1 |= I2C_CR1_START;
	while((I2C1->SR1 & I2C_SR1_SB) != I2C_SR1_SB);
	(void) I2C1->SR1;
	I2C1->DR = (addr << 1) | 0x00;
	while((I2C1->SR1 & I2C_SR1_ADDR) != I2C_SR1_ADDR);
	(void) I2C1->SR1;  // clear ADDR
	(void) I2C1->SR2;  // clear ADDR
	I2C1->DR= byte;
	while((I2C1->SR1 & I2C_SR1_TXE) != I2C_SR1_TXE);
	I2C1->CR1 |= I2C_CR1_STOP;
}

void I2C_TH_Send_NBytes(uint8_t addr, uint8_t *data, uint8_t size) {
	I2C1->CR1 |= I2C_CR1_ACK;
	I2C1->CR1 |= I2C_CR1_START;
	while((I2C1->SR1 & I2C_SR1_SB) != I2C_SR1_SB);
	(void) I2C1->SR1;
	I2C1->DR = (addr << 1) | 0x00;
	while((I2C1->SR1 & I2C_SR1_ADDR) != I2C_SR1_ADDR);
	(void) I2C1->SR1;  // clear ADDR
	(void) I2C1->SR2;  // clear ADDR
	for (uint8_t i = 0; i < size; i++)
	{
		I2C1->DR = data[i];
		while((I2C1->SR1 & I2C_SR1_TXE) != I2C_SR1_TXE);
	}

	I2C1->CR1 |= I2C_CR1_STOP;
}


void I2C_Get_Data(uint8_t addr, uint8_t *data, uint8_t size) {
	while((I2C1->SR2 & I2C_SR2_BUSY) == I2C_SR2_BUSY);
	I2C1->CR1 |= I2C_CR1_ACK;
	I2C1->CR1 |= I2C_CR1_START;
	while((I2C1->SR1 & I2C_SR1_SB) != I2C_SR1_SB);
	(void) I2C1->SR1;
	I2C1->DR = (addr << 1) | 0x01;
	while((I2C1->SR1 & I2C_SR1_ADDR) != I2C_SR1_ADDR);
	(void) I2C1->SR1;  // clear ADDR
	(void) I2C1->SR2;  // clear ADDR
	for(int i = 0;i < 6; i++)
  {
    if(i < 5)
    {
      while (!READ_BIT(I2C1->SR1, I2C_SR1_RXNE)){}
      data[i] = I2C1->DR & 0xFF;
    }
    else
    {
			I2C1->CR1 &= ~I2C_CR1_ACK;
			I2C1->CR1 |= I2C_CR1_STOP;
      while (!READ_BIT(I2C1->SR1, I2C_SR1_RXNE)){}
			data[i] = I2C1->DR & 0xFF;
    }
	}
}


void TH_measure_temperature() {
	if(start_state == 0) {
		uint8_t data[3] = {0xAC, 0x33, 0x00};
		I2C_TH_Send_NBytes(0x38, &data[0], 3);
		start_state = 1;
		return;
	}
	
	for(int i = 0; i < 100000; i++);

	if(start_state == 1) {
		uint8_t result[6];
		I2C_Get_Data(0x38, &(result[0]), 6);
		I2C_Get_Data(0X38, &(result[0]), 6);
		float buffer = (((uint32_t)result[3] & 15) << 16) | ((uint32_t)result[4] << 8) | result[5];
		output_data.temperature_TH02 = (float)(buffer * 200.00 / 1048576.00) - 50.00;
		start_state = 0;
	}
}