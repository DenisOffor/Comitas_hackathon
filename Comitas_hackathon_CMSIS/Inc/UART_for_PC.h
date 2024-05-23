/*
 * UART_for_PC.h
 *
 *  Created on: 13 ���. 2023 �.
 *      Author: denlo
 */

#ifndef UART_FOR_PC_H_
#define UART_FOR_PC_H_

#include "common.h"

void DMA1_Channel4_5_IRQHandler(void);
void init_GPIO_for_USART();
void init_USART();
void init_DMA_for_USART();
void UART_send_data(uint8_t ch8);

#endif /* UART_FOR_PC_H_ */