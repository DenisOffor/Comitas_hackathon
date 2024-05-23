

#include <TH02.h>

void init_GPIO_for_I2C() {
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	GPIOB->CRL |= GPIO_CRL_MODE6 | GPIO_CRL_MODE6 | GPIO_CRL_CNF6 | GPIO_CRL_CNF7;
	GPIOB->ODR |= GPIO_ODR_ODR6 | GPIO_ODR_ODR7;
}

void I2C_for_TH_init() {
	init_GPIO_for_I2C();

	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	I2C1->CR1 &= ~I2C_CR1_PE;

	I2C1->CR2 |= I2C_CR2_FREQ_3; //8
	I2C1->CCR |= 0x0028;
	I2C1->TRISE |= 0x0009;
	I2C1->CR1 |= I2C_CR1_PE | I2C_CR1_ACK;
}

void I2C_TH_SendByte(uint8_t addr, uint8_t byte) {
	I2C1->CR1 |= I2C_CR1_START;
	while((I2C1->SR1 & I2C_SR1_SB) != I2C_SR1_SB);
	(void) I2C1->SR1;
	I2C1->DR = addr & ~0x01;
	while((I2C1->SR1 & I2C_SR1_ADDR) != I2C_SR1_ADDR);
	(void) I2C1->SR1;  // clear ADDR
	(void) I2C1->SR2;  // clear ADDR
	I2C1->DR= byte;
	while((I2C1->SR1 & I2C_SR1_TXE) != I2C_SR1_TXE);
	I2C1->CR1 |= I2C_CR1_STOP;
	while((I2C1->SR1 & I2C_SR1_STOPF) != I2C_SR1_STOPF);
}

void I2C_TH_Send_NBytes(uint8_t addr, uint8_t *data, uint8_t size) {
	I2C1->CR1 |= I2C_CR1_START;
	while((I2C1->SR1 & I2C_SR1_SB) != I2C_SR1_SB);
	(void) I2C1->SR1;
	I2C1->DR = addr & ~0x01;
	while((I2C1->SR1 & I2C_SR1_ADDR) != I2C_SR1_ADDR);
	(void) I2C1->SR1;  // clear ADDR
	(void) I2C1->SR2;  // clear ADDR
	for (uint8_t i = 0; i < size; i++)
	{
		I2C1->DR = data[i];
		while((I2C1->SR1 & I2C_SR1_TXE) != I2C_SR1_TXE) {};
	}
	I2C1->CR1 |= I2C_CR1_STOP;
	while((I2C1->SR1 & I2C_SR1_STOPF) != I2C_SR1_STOPF);
}

