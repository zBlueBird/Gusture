/*******************************************************************************
基于FDC2214的手势识别控制系统设计
通讯波特率：115200
*******************************************************************************/
#include "oled.h"
#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "key.h"
#include "FDC2214.h"
#include "time.h"
#include "shoushi.h"
#include "usart1.h"

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

#define OLED_ShowString(a,b,c,d)      (printf(c))
#define OLED_ShowChar(a,b,c,d,e)      (printf("%c\r\n",c))

int main()
{
    int flag_p = 0;
    u8 i = 0;

    SysTick_Init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
    LED_Init();

    KEY_Init();
    FDC2214_Init();
    TIM4_Init(10000, 7200 - 1);
    TIM3_Init(1000, 360 - 1);
    //OLED_Init();

    /* wait interrupt */
    /* USART1 config 115200 8-N-1 */
    USART1_Config();
    NVIC_Configuration();

    printf("uart is init\r\n");

    while (1)
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
        OLED_Refresh_Gram();

        if (mod == 1)
        {
            if (flag_s == 1)
            {
                OLED_Clear();
                OLED_ShowString(0, 10, "mode:", 12);
                OLED_ShowChar(30, 10, mod + 48, 12, 1);
                OLED_ShowString(60, 10, "learn1", 12);
                OLED_Refresh_Gram();
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
//                              flag_s=0;
                    OLED_Clear();
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
                    OLED_Refresh_Gram();
                }
                else
                {
                    jiandao = 0;
                    shitou = 0;
                    bu = 0;
                    OLED_Fill(48, 35, 120, 50, 0) ;
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


                OLED_Clear();
                OLED_ShowString(0, 10, "mode:", 12);
                OLED_ShowChar(30, 10, mod + 48, 12, 1);
                OLED_ShowString(60, 10, "learn2", 12);
                OLED_Refresh_Gram();
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
                    OLED_Fill(30, 35, 120, 50, 0) ;
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
        if (i % 60 == 0)
        {
            led1 = !led1;
        }

    }
}



