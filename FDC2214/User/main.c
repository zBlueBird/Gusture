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


key_msg_dat g_key_data = {0};

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
    int flag_p = 0;

    SysTick_Init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组

    KEY_Init();
    FDC2214_Init();
    TIM4_Init(10000, 7200 - 1); //learn timeout timer, 10s
    TIM3_Init(30000, 360 - 1);//Debunce timer

    /* wait interrupt */
    /* USART1 config 115200 8-N-1 */
    USART1_Config();
    NVIC_Configuration();

    printf("[UART] uart is init\r\n");

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
                    xunlian1();
                    printf(" gesture: %d\r\n", flag_s1);

                    if (flag_e1 == 1)
                    {
                        printf(" gesture: failed\r\n");
                        flag_e1 = 0;
                    }
                    else
                    {
                        printf(" gesture: success\r\n");
                    }

                    if (flag_s1 == 3)
                    {
                        fenlei1();
                        error1();
                        if (flag_e == 1)
                        {
                            printf(" gesture: failed.....\r\n");
                            flag_e = 0;
                        }
                        else
                        {
                            printf(" gesture: success.....\r\n");
                        }
                    }
                }
                else if (mod == 2)
                {
                    flag_p = zuizhongpanduan();
                    if (flag_p != 0)
                    {
                        OLED_ShowString(0, 35, "flag:", 12);
                        if (flag_p == 1)
                        {
                            shitou = 1;
                            jiandao = 0;
                            bu = 0;
                            OLED_ShowString(48, 35, "Shi Tou", 12);

                        }
                        else if (flag_p == 2)
                        {
                            shitou = 0;
                            jiandao = 1;
                            bu = 0;
                            OLED_ShowString(48, 35, "Jian Dao", 12);
                        }
                        else if (flag_p == 3)
                        {
                            shitou = 0;
                            jiandao = 0;
                            bu = 1;
                            OLED_ShowString(48, 35, "Bu", 12);
                        }
                    }
                    else
                    {
                        printf(" gesture: can not distinguish\r\n");
                        jiandao = 0;
                        shitou = 0;
                        bu = 0;
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
#if 0
    {
        OLED_ShowString(0, 10, "mode:", 12);
        OLED_ShowChar(30, 10, mod + 48, 12, 1);
        if (mod == 1)
        {
            OLED_ShowString(60, 10, "learn1", 12);
        }
        else if (mod == 2)
        {
            OLED_ShowString(60, 10, "judge1", 12);
        }
        else if (mod == 3)
        {
            OLED_ShowString(60, 10, "learn2", 12);
        }
        else if (mod == 4)
        {
            OLED_ShowString(60, 10, "judge2", 12);
        }

        if (mod == 1)
        {
            if (flag_s == 1)
            {
                OLED_ShowString(0, 10, "mode:", 12);
                OLED_ShowChar(30, 10, mod + 48, 12, 1);
                OLED_ShowString(60, 10, "learn1", 12);
                xunlian1();
                OLED_ShowString(0, 35, "gesture:", 12);
                OLED_ShowChar(48, 35, flag_s1 + 48, 12, 1);

                if (flag_e1 == 1)
                {
                    OLED_ShowString(60, 35, "error!!!", 12);
                    flag_e1 = 0;
                }
                else
                {
                    OLED_ShowString(60, 35, "Finish!", 12);
                }
                if (flag_s1 == 3)
                {
                    fenlei1();
                    error1();

                    if (flag_e == 1)
                    {

                        OLED_ShowString(0, 45, "Fail!!!", 12);
                        flag_e = 0;

                    }
                    else
                    {
                        OLED_ShowString(0, 45, "Success!", 12);
                    }
                }
            }
            flag_s = 0;
        }

        else if (mod == 2)
        {

            if (1)//(flag_s == 1)
            {
                flag_p = zuizhongpanduan();
                if (flag_p != 0)
                {
                    OLED_ShowString(0, 35, "flag:", 12);
                    if (flag_p == 1)
                    {
                        shitou = 1;
                        jiandao = 0;
                        bu = 0;
                        OLED_ShowString(48, 35, "Shi Tou", 12);

                    }
                    else if (flag_p == 2)
                    {
                        shitou = 0;
                        jiandao = 1;
                        bu = 0;
                        OLED_ShowString(48, 35, "Jian Dao", 12);
                    }
                    else if (flag_p == 3)
                    {
                        shitou = 0;
                        jiandao = 0;
                        bu = 1;
                        OLED_ShowString(48, 35, "Bu", 12);
                    }
                }
                else
                {
                    jiandao = 0;
                    shitou = 0;
                    bu = 0;
                }
            }
            else
            {
                jiandao = 0;
                shitou = 0;
                bu = 0;

            }


        }

        else if (mod == 3)
        {

            if (flag_s == 1)
            {
                OLED_ShowString(0, 10, "mode:", 12);
                OLED_ShowChar(30, 10, mod + 48, 12, 1);
                OLED_ShowString(60, 10, "learn2", 12);
                xunlian2();
                OLED_ShowString(0, 35, "gesture:", 12);
                OLED_ShowChar(48, 35, flag_s2 + 48, 12, 1);

                if (flag_e1 == 1)
                {
                    OLED_ShowString(60, 35, "error!!!", 12);
                    flag_e1 = 0;
                }
                else
                {
                    OLED_ShowString(60, 35, "Finish!", 12);
                }
                if (flag_s2 == 5)
                {

                    fenlei2();
                    error2();
                    if (flag_e == 1)
                    {

                        OLED_ShowString(0, 45, "Fail!!!", 12);
                        flag_e = 0;

                    }
                    else
                    {
                        OLED_ShowString(0, 45, "Success!", 12);
                    }
                }
                flag_s = 0;
            }
        }

        else if (mod == 4)
        {

            if (flag_s == 1)
            {

                flag_p = zuizhongpanduan1();

                if (flag_p != 0)
                {
                    if (flag_p == 1)
                    {
                        nu1 = 1;
                        nu2 = 0;
                        nu3 = 0;
                        nu4 = 0;
                        nu5 = 0;

                    }
                    else if (flag_p == 2)
                    {
                        nu1 = 0;
                        nu2 = 1;
                        nu3 = 0;
                        nu4 = 0;
                        nu5 = 0;

                    }
                    else if (flag_p == 3)
                    {
                        nu1 = 0;
                        nu2 = 0;
                        nu3 = 1;
                        nu4 = 0;
                        nu5 = 0;

                    }
                    else if (flag_p == 4)
                    {
                        nu1 = 0;
                        nu2 = 0;
                        nu3 = 0;
                        nu4 = 1;
                        nu5 = 0;

                    }
                    else if (flag_p == 5)
                    {
                        nu1 = 0;
                        nu2 = 0;
                        nu3 = 0;
                        nu4 = 0;
                        nu5 = 1;

                    }
                    OLED_ShowString(0, 35, "flag:", 12);
                    OLED_ShowChar(30, 35, flag_p + 48, 12, 1);
                }
                else
                {
                    nu1 = 0;
                    nu2 = 0;
                    nu3 = 0;
                    nu4 = 0;
                    nu5 = 0;
                }
            }
            else
            {

                nu1 = 0;
                nu2 = 0;
                nu3 = 0;
                nu4 = 0;
                nu5 = 0;
            }



        }

        i++;

    }
#endif
}



