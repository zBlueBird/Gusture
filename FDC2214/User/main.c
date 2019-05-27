/*******************************************************************************
基于FDC2214的手势识别控制系统设计
通讯波特率：115200
*******************************************************************************/
#include "system.h"
#include "SysTick.h"
#include "key.h"
#include "FDC2214.h"
#include "time.h"
#include "shoushi.h"
#include "usart1.h"
#include "usart2.h"

key_msg_dat g_key_data = {0};
gesture_data_stg g_data = {0};

u8 mod = 2;
int flag_s = 0;
u8 flag_s1 = 0;
u8 flag_s2 = 0;
u8 flag_e = 0;
u8 flag_e1 = 0;
int max123 = 1895, min123 = 1618;
int max12345 = 1899, min12345 = 1690;
int max0[3], min0[3];
int mid0[3] = {1626, 1746, 1891};
int max[5], min[5];
int mid[5] = {1702, 1749, 1787, 1836, 1895};

#define OLED_ShowString(a,b,c,d)      //(printf(c))
#define OLED_ShowChar(a,b,c,d,e)      //(printf("%c\r\n",c))

int main()
{
    SysTick_Init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组

    KEY_Init();
    //FDC2214_Init();
    TIM4_Init(10000, 7200 - 1); //learn timeout timer, 10s
    TIM3_Init(30000, 360 - 1);//Debunce timer

    /* wait interrupt */
    /* USART1 config 115200 8-N-1 */
    USART1_Config();
    //NVIC_Configuration();

    USART2_Config(115200);

    printf("[UART] uart is init\r\n");
    USART2_printf(USART2, "\r\nuart2 is init\r\n");

    /*输入指令*/
    USART2_printf(USART2, GET_BUAD);
    USART2_printf(USART2, GET_NAME);
    USART2_printf(USART2, GET_ROLE);

    while (1)
    {

        if (g_key_data.key_flag)
        {
            //clear flag
            g_key_data.key_flag = 0;

            //printf("[APP] key value = %#x\r\n", g_key_data.key_value);

            if (g_key_data.key_value & KEY_PRESS_INDEX_2)
            {
                printf(" <============ mode: %d\r\n", mod);
                if (mod == 1)
                {
                    g_data.flag ++;
                    if (g_data.flag % 2 == 0)
                    {
                        printf(" Switch on Learn...\r\n");
                        g_data.index[0] = 1;
                        g_data.value[0] = dianrongzhi(0);
                    }
                    else
                    {
                        printf(" Switch off Learn...\r\n");
                        g_data.index[1] = 2;
                        g_data.value[1] = dianrongzhi(0);
                    }
                }
                else if (mod == 2)
                {
                    uint16_t data = dianrongzhi(0);
                    if ((data > g_data.value[0] - 25) && (data < g_data.value[0] + 25))
                    {
                        printf(" gesture: switch on\r\n");
                    }
                    else if ((data > g_data.value[1] - 25) && (data < g_data.value[1] + 25))
                    {
                        printf("gesture: switch off");
                    }
                    else
                    {
                        printf(" gesture error");
                    }
                }
            }

            else if (g_key_data.key_value & KEY_PRESS_INDEX_1)
            {
                mod++;
                mod = mod % 3;
                printf("\r\n [Mode 0] Gesture Idle\r\n");
                printf(" [Mode 1] Gesture Learn1\r\n");
                printf(" [Mode 2] Gesture Distinguish\r\n");

                printf(" ============> mode: %d\r\n", mod);
            }
        }
    }
}



