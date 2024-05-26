

#include "TH02.h"
uint8_t start_state = 0;

void init_GPIO_for_I2C() {
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	GPIOB->CRL &= ~GPIO_CRL_CNF6;
	GPIOB->CRL &= ~GPIO_CRL_CNF7;
	//AF open drain
	GPIOB->CRL |= GPIO_CRL_MODE6 | GPIO_CRL_MODE7 | GPIO_CRL_CNF6 | GPIO_CRL_CNF7;
}

void I2C_for_TH_init() {
	init_GPIO_for_I2C();

	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	I2C1->CR1 &= ~I2C_CR1_PE;
	//I2C in stm32f103 work really strange, so i try do to all strange thing to make it work
	//but in proteus it didn't work :/ it work in real life with AHT20 sensor (because i find it for test I2C)
	I2C1->OAR2 &= ~I2C_OAR2_ENDUAL;
	I2C1->CR1 &= ~I2C_CR1_ENGC;
	I2C1->CR1 &= ~I2C_CR1_NOSTRETCH;

	//
	I2C1->CR2 = I2C_CR2_FREQ_3; //8
	I2C1->CCR |= 0x28;
	I2C1->TRISE |= 0x0019;
	I2C1->CR1 |= 0x00000000;
	I2C1->CR1 |= I2C_CR1_PE;
}

void I2C_TH_SendByte(uint8_t addr, uint8_t byte) {
	//all I2C operation done in match with datasheet (rm091 smth like that)
	I2C1->CR1 |= I2C_CR1_ACK;
	I2C1->CR1 |= I2C_CR1_START;
	while((I2C1->SR1 & I2C_SR1_SB) != I2C_SR1_SB);
	(void) I2C1->SR1;
	I2C1->DR = (addr << 1) | 0x00;
	while((I2C1->SR1 & I2C_SR1_ADDR) != I2C_SR1_ADDR);
	//again all done in match with datasheet, so there was info that need to clear that bytes by reading
	(void) I2C1->SR1;  // clear ADDR
	(void) I2C1->SR2;  // clear ADDR
	I2C1->DR= byte;
	while((I2C1->SR1 & I2C_SR1_TXE) != I2C_SR1_TXE);
	I2C1->CR1 |= I2C_CR1_STOP;
}

void I2C_TH_Send_NBytes(uint8_t addr, uint8_t *data, uint8_t size) {
	//all I2C operation done in match with datasheet (rm091 smth like that)
	I2C1->CR1 |= I2C_CR1_START;
	while((I2C1->SR1 & I2C_SR1_SB) != I2C_SR1_SB);
	(void) I2C1->SR1;
	I2C1->DR = (addr << 1) | 0x00;
	//again all done in match with datasheet, so there was info that need to clear that bytes by reading
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
	//all I2C operation done in match with datasheet (rm091 smth like that)
	while((I2C1->SR2 & I2C_SR2_BUSY) == I2C_SR2_BUSY);
	I2C1->CR1 |= I2C_CR1_ACK;
	I2C1->CR1 |= I2C_CR1_START;
	while((I2C1->SR1 & I2C_SR1_SB) != I2C_SR1_SB);
	(void) I2C1->SR1;
	I2C1->DR = (addr << 1) | 0x01;
	while((I2C1->SR1 & I2C_SR1_ADDR) != I2C_SR1_ADDR);
	(void) I2C1->SR1;  // clear ADDR
	(void) I2C1->SR2;  // clear ADDR
	for(int i = 0;i < size; i++)
	{
    if(i < size - 1)
    {
      while ((I2C1->SR1 & I2C_SR1_RXNE) != I2C_SR1_RXNE);
      data[i] = I2C1->DR & 0xFF;
    }
    else
    {
    		//last transfer need to set NACK in order to stop transfer from slave
			I2C1->CR1 &= ~I2C_CR1_ACK;
			I2C1->CR1 |= I2C_CR1_STOP;
			while ((I2C1->SR1 & I2C_SR1_RXNE) != I2C_SR1_RXNE);
			data[i] = I2C1->DR & 0xFF;
    }
	}
}

void TH_measure_temperature() {
	//send command for start measurement
	if(start_state == 0) {
		uint8_t data[2] = {0x03, 0x01};
		I2C_TH_Send_NBytes(TH_ADDRESS, &data[0], 2);
		start_state = 1;
		return;
	}
	
	///there are need to be done some timing or FSM like in DS18B20 but i really want to make it work in PROTEUS and don't have much time
	///maybe i would not forget to write it...
	for(int i = 0; i < 100000; i++);

	//read temperature and humidity from TH02 and convert it
	uint8_t result[2];
	I2C_TH_SendByte(TH_ADDRESS, 0x01);
	I2C_Get_Data(TH_ADDRESS, &(result[0]), 2);
	uint16_t buffer = (result[0]) | (result[1] << 8);
	output_data.temperature_TH02 = (float)(buffer/32 - 50);
	start_state = 0;

}
