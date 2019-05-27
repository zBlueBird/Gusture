#ifndef _UART2_H_
#define _UART2_H_

#include "stm32f10x.h"

#define  GET_NAME "AT:NAME?\r\n"
#define  SET_NAME "AT:NAME<HelloKitty>\r\n"
#define  GET_BUAD "AT:BUAD\r\n"
#define  GET_ROLE "AT:ROLE\r\n"
#define  SET_ADV  "AT:ENADV\r\n"

void USART2_Config(uint32_t My_BaudRate);
void USART2_printf(USART_TypeDef *USARTx, uint8_t *Data, ...);

#endif
