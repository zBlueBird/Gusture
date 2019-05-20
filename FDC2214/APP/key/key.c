#include "key.h"
#include "SysTick.h"
#include "oled.h"
extern int mod;
extern int flag_s1;
extern int flag_s;
extern int flag_s2;
/*******************************************************************************
* 函 数 名         : KEY_Init
* 函数功能         : 按键初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; //定义结构体变量
    RCC_APB2PeriphClockCmd(RCC_key, ENABLE);

    GPIO_InitStructure.GPIO_Pin = KEY_LEFT_Pin ;   //选择你要设置的IO口
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //设置传输速率
    GPIO_Init(KEY_Port, &GPIO_InitStructure);         /* 初始化GPIO */

}


void KEY_Scan(void)
{
    static u8 key = 1;
    if (key == 1 && (K_UP == 0 || K_DOWN == 0 || K_LEFT == 0)) //任意一个按键按下
    {
        delay_ms(10);  //消抖
        key = 0;
        if (K_UP == 0)
        {
            mod++;
            flag_s = 0;
            OLED_Clear();
            if (mod == 5)
            {
                mod = 1;
            }
        }
        else if (K_DOWN == 0)
        {

            flag_s = 1;

            if (mod == 1)

            {
                if (flag_s1 == 3)
                {

                    flag_s1 = 0;
                }
            }
            else if (mod == 3)
            {

                if (flag_s2 == 5)
                {
                    flag_s2 = 0;
                }
            }




        }
        else if (K_LEFT == 0)
        {

            flag_s = 0;

        }

    }
    else if (K_UP == 1 && K_DOWN == 1 && K_LEFT == 1) //无按键按下
    {

        key = 1;
    }


}

