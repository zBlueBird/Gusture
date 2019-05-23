#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
#define USART_REC_LEN           200     //定义最大接收字节数 200
#define EN_USART1_RX            1       //使能（1）/禁止（0）串口1接收

extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u16 USART_RX_STA;                //接收状态标记
//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);
#if defined USART_C
u8 SendBuf[256];
u32 sendPtr;
u32 readPtr;
u32 sendPtr4;
u32 readPtr4;
u8 Send_flag;
u8  USART1_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
u8  USART1_TX_BUF[USART_REC_LEN]; //发送缓冲,最大USART_REC_LEN个字节.末字节为换行符
u16 USART1_RX_STA;              //接收状态标记
u8  USART2_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
u8  USART2_TX_BUF[USART_REC_LEN]; //发送缓冲,最大USART_REC_LEN个字节.末字节为换行符
u16 USART2_RX_STA;              //接收状态标记
u8  USART3_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
u8  USART3_TX_BUF[USART_REC_LEN]; //发送缓冲,最大USART_REC_LEN个字节.末字节为换行符
u16 USART3_RX_STA;              //接收状态标记
u8  USART4_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
u8  USART4_TX_BUF[USART_REC_LEN]; //发送缓冲,最大USART_REC_LEN个字节.末字节为换行符
u16 USART4_RX_STA;              //接收状态标记
u8  USART5_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
u8  USART5_TX_BUF[USART_REC_LEN]; //发送缓冲,最大USART_REC_LEN个字节.末字节为换行符
u16 USART5_RX_STA;              //接收状态标记

void uart1_init(u32 bound); //串口1初始化
void uart2_init(u32 bound); //串口2初始化
void uart3_init(u32 bound); //串口3初始化
void uart4_init(u32 bound); //串口4初始化
void uart5_init(u32 bound); //串口5初始化
void Usart1_Send(u8 data[], u8 len); //串口1发送
void Usart2_Send(u8 data[], u8 len); //串口2发送
void Usart3_Send(u8 data[], u8 len); //串口3发送
void Usart4_Send(u8 data[], u8 len); //串口4发送
void Usart5_Send(u8 data[], u8 len); //串口5发送
void Usart1_Sendbuf(u8 data[], u8 data2[], u16 count);
void Usart4_Sendbuf(u8 data[], u8 data2[], u16 count);
#else

extern u8 SendBuf[256];
extern u32 sendPtr;
extern u32 readPtr;
extern u32 sendPtr4;
extern u32 readPtr4;
extern u8 Send_flag;
extern u8  USART1_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u8  USART1_TX_BUF[USART_REC_LEN]; //发送缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u16 USART1_RX_STA;               //接收状态标记
extern u8  USART2_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u8  USART2_TX_BUF[USART_REC_LEN]; //发送缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u16 USART2_RX_STA;               //接收状态标记
extern u8  USART3_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u8  USART3_TX_BUF[USART_REC_LEN]; //发送缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u16 USART3_RX_STA;               //接收状态标记
extern u8  USART4_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u8  USART4_TX_BUF[USART_REC_LEN]; //发送缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u16 USART4_RX_STA;               //接收状态标记
extern u8  USART5_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u8  USART5_TX_BUF[USART_REC_LEN]; //发送缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u16 USART5_RX_STA;               //接收状态标记

extern void uart1_init(u32 bound); //串口1初始化
extern void uart2_init(u32 bound); //串口2初始化
extern void uart3_init(u32 bound); //串口3初始化
extern void uart4_init(u32 bound); //串口4初始化
extern void uart5_init(u32 bound); //串口5初始化
extern void Usart1_Send(u8 data[], u8 len); //串口1发送
extern void Usart2_Send(u8 data[], u8 len); //串口2发送
extern void Usart3_Send(u8 data[], u8 len); //串口3发送
extern void Usart4_Send(u8 data[], u8 len); //串口4发送
extern void Usart5_Send(u8 data[], u8 len); //串口5发送
extern void Usart1_Sendbuf(u8 data[], u8 data2[], u16 count);
extern void Usart4_Sendbuf(u8 data[], u8 data2[], u16 count);

#endif
#endif


