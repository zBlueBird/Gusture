#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "timer.h"
#include "wdg.h"
#include "spi.h"
#include "Data_sampling.h"
#include "FDC2X14.h"
#include "FM25V02.h"
#include <math.h>
void lcd01(void);
void FDC2214(void);

int main(void)
{

    lcd01();
    FDC2214();
    while (1)
    {
        LCD_ShowxNum(172, 190, 4, 4, 16, 0);

        //SendSS();

        if (fivemsec_flag == 1)
        {
            fivemsec_flag = 0;
            EWDG_Feed();
            ChValue[0] = FDC2X14ReadCH(1);
            ChValue[1] = FDC2X14ReadCH(2);
//          ChValue[2] = FDC2X14ReadCH(3);
//          ChValue[3] = FDC2X14ReadCH(4);
            SendSS();
        }
    }

}
void lcd01(void)
{
    delay_init();            //延时函数初始化
    //uart_init(9600);      //串口初始化为9600
    uart1_init(9600);
    LED_Init();             //初始化与LED连接的硬件接口

    POINT_COLOR = BLACK;

    //LCD_Clear(WHITE);
    //LCD_Init();
}

void FDC2214(void)
{

    NVIC_Configuration();   //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    delay_init();            //延时函数初始化
    uart2_init(230400);     //串口1初始化为115200

    //LCD_Init();
    //SPI2_FM25V02_Init();
    //FDC2X14_Init();
    ESWDG_Init();
    //72M晶振
    TIM3_Int_Init(71, 499); //500us定时



}
