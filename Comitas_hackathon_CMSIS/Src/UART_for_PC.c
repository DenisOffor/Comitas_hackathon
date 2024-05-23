/*
 * UART_for_PC.c
 *
 *  Created on: 13 ���. 2023 �.
 *      Author: denlo
 */

#include "UART_for_PC.h"

uint8_t UART_tx_buf[] = "Comitas\r\n";


void DMA1_Channel4_IRQHandler(void) {
	DMA1->IFCR |= DMA_IFCR_CTCIF4;
	DMA1_Channel4->CCR &= ~DMA_CCR_EN;
}

void init_GPIO_for_USART() {
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA->CRH |= GPIO_CRH_MODE9 | GPIO_CRH_CNF9_1;
}

void init_USART() {
	init_GPIO_for_USART();
	init_DMA_for_USART();

	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	USART1->CR1 |= USART_CR1_TE;
	USART1->BRR = SystemCoreClock / 115200;
	USART1->CR3 |= USART_CR3_DMAT;
	USART1->CR1 |= USART_CR1_UE;
}


void init_DMA_for_USART() {
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	//USART TX channel - 4
	DMA1_Channel4->CCR |= DMA_CCR_DIR | DMA_CCR_MINC;
	DMA1_Channel4->CMAR = (uint32_t)(&UART_tx_buf[0]);
	DMA1_Channel4->CPAR = (uint32_t)(&(USART1->DR));
	DMA1_Channel4->CCR |= DMA_CCR_TCIE;
	DMA1_Channel4->CNDTR = 0;

	NVIC_EnableIRQ(DMA1_Channel4_IRQn);
	NVIC_SetPriority(DMA1_Channel4_IRQn, 3);
}

void UART_send_data(uint8_t ch8) {
	UART_tx_buf[0] = ch8;
	DMA1_Channel4->CMAR = (uint32_t)(&UART_tx_buf[0]);
	DMA1_Channel4->CNDTR = 1;
	DMA1_Channel4->CCR |= DMA_CCR_EN;
	while(DMA1_Channel4->CCR & DMA_CCR_EN) {};
	for(int i = 0; i < 100; i++);
	flag = 0;
}
