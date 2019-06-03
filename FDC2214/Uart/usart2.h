#ifndef _UART2_H_
#define _UART2_H_

#include "stm32f10x.h"

#define USART2_REC_LEN           100

#define  GET_NAME     "AT:NAME?\r\n"
#define  SET_NAME     "AT:NAME<HelloKitty>\r\n"
#define  GET_BUAD     "AT:BAUD?\r\n"
#define  GET_ROLE     "AT:ROLE?\r\n"
#define  SET_ENADV    "AT:ENADV1\r\n"
#define  SET_DISADV   "AT:ENADV0\r\n"

#define  RES_STATE_BLE    "Connected\r\n"

void USART2_Config(uint32_t My_BaudRate);
void USART2_printf(USART_TypeDef *USARTx, uint8_t *Data, ...);
void uart2_loop_proc(void);

#endif
