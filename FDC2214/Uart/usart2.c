#include "usart2.h"
#include "misc.h"
#include "stdio.h"
#include <stdarg.h>
#include "stm32f10x_usart.h"

#include "usart1.h"

static uint8_t USART2_RX_BUF[USART2_REC_LEN];
static uint16_t  USART2_RX_STA = 0;

void uart2_loop_proc(void)
{
    uint8_t t;
    uint8_t len;
    if (USART2_RX_STA & 0x8000)
    {
        len = USART2_RX_STA & 0x3f;
        printf("uart2 receive data from ble\r\n");
#if 1

        /*send through uart1*/
        for (t = 0; t < len; t++)
        {
            USART_SendData(USART1, USART2_RX_BUF[t]);
            while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
        }
#endif
        USART2_RX_STA = 0;
    }
}

void USART2_Config(u32 My_BaudRate)
{
    GPIO_InitTypeDef GPIO_InitStrue;
    USART_InitTypeDef USART_InitStrue;
    NVIC_InitTypeDef NVIC_InitStrue;

    // 外设使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    USART_DeInit(USART2);  //复位串口2 -> 可以没有

    // 初始化 串口对应IO口  TX-PA2  RX-PA3
    GPIO_InitStrue.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStrue.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStrue.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStrue);

    GPIO_InitStrue.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStrue.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStrue);

    // 初始化 串口模式状态
    USART_InitStrue.USART_BaudRate = My_BaudRate; // 波特率
    USART_InitStrue.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 硬件流控制
    USART_InitStrue.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // 发送 接收 模式都使用
    USART_InitStrue.USART_Parity = USART_Parity_No; // 没有奇偶校验
    USART_InitStrue.USART_StopBits = USART_StopBits_1; // 一位停止位
    USART_InitStrue.USART_WordLength = USART_WordLength_8b; // 每次发送数据宽度为8位
    USART_Init(USART2, &USART_InitStrue);

    USART_Cmd(USART2, ENABLE); //使能串口
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //开启接收中断

    // 初始化 中断优先级
    NVIC_InitStrue.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStrue.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStrue.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStrue);
}

void USART2_IRQHandler(void) // 串口2中断服务函数
{
    uint8_t res;
    if (USART_GetITStatus(USART2, USART_IT_RXNE)) // 中断标志
    {
        res = USART_ReceiveData(USART2); // 串口2 接收
        //USART_SendData(USART1, res);  // 串口1 发送
#if 1
        //res = USART_ReceiveData(USART1); //(USART1->DR);  //????????
        if ((USART2_RX_STA & 0x8000) == 0)
        {
            if (USART2_RX_STA & 0x4000)
            {
                if (res != 0x0a)
                {
                    USART2_RX_STA = 0;
                }
                else
                {
                    USART2_RX_STA |= 0x8000;
                }
            }
            else //0X0D
            {
                if (res == 0x0d)
                {
                    USART2_RX_STA |= 0x4000;
                }
                else
                {
                    USART2_RX_BUF[USART2_RX_STA & 0X3FFF] = res ;
                    USART2_RX_STA++;
                    if (USART2_RX_STA > (USART_REC_LEN - 1))
                    {
                        USART2_RX_STA = 0;//reset length 0
                    }
                }
            }
        }
#endif
    }

}

#if 0
/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USART1
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
int fputc(int ch, FILE *f)
{
    /* 将Printf内容发往串口 */
    USART_SendData(USART2, (unsigned char) ch);
    while (!(USART2->SR & USART_FLAG_TXE));

    return (ch);
}
#endif
/*
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被USART1_printf()调用
 */
static char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */
/*
 * 函数名：USART1_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口1，即USART1
 *           -Data   要发送到串口的内容的指针
 *             -...    其他参数
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 *         典型应用USART1_printf( USART1, "\r\n this is a demo \r\n" );
 *                   USART1_printf( USART1, "\r\n %d \r\n", i );
 *                   USART1_printf( USART1, "\r\n %s \r\n", j );
 */
void USART2_printf(USART_TypeDef *USARTx, uint8_t *Data, ...)
{
    const char *s;
    int d;
    char buf[16];

    va_list ap;
    va_start(ap, Data);

    while (*Data != 0)      // 判断是否到达字符串结束符
    {
        if (*Data == 0x5c)    //'\'
        {
            switch (*++Data)
            {
            case 'r':                                     //回车符
                USART_SendData(USARTx, 0x0d);
                Data ++;
                break;

            case 'n':                                     //换行符
                USART_SendData(USARTx, 0x0a);
                Data ++;
                break;

            default:
                Data ++;
                break;
            }
        }
        else if (*Data == '%')
        {
            //
            switch (*++Data)
            {
            case 's':                                         //字符串
                s = va_arg(ap, const char *);
                for (; *s; s++)
                {
                    USART_SendData(USARTx, *s);
                    while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
                }
                Data++;
                break;

            case 'd':                                       //十进制
                d = va_arg(ap, int);
                itoa(d, buf, 10);
                for (s = buf; *s; s++)
                {
                    USART_SendData(USARTx, *s);
                    while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
                }
                Data++;
                break;
            default:
                Data++;
                break;
            }
        } /* end of else if */
        else { USART_SendData(USARTx, *Data++); }
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);

    }

    va_end(ap);
}

