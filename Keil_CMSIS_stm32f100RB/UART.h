/*
 * UART_for_PC.h
 *
 *  Created on: 13 ���. 2023 �.
 *      Author: denlo
 */

#ifndef UART_H_
#define UART_H_

#include "common.h"

#define BUF_SIZE 255


void DMA1_Channel4_IRQHandler(void);
void init_GPIO_for_USART(void);
void init_USART(void);
void init_DMA_for_USART(void);
void UART_send_data(uint8_t* data, uint8_t amount_of_byte);
//void UART_send_data(uint8_t amount_of_byte);

#endif /* UART_H_ */
