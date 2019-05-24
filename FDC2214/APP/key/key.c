#include "key.h"
#include "SysTick.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "misc.h"
#include "usart1.h"

#define  KEY_LOG_EN  0

extern int mod;
extern int flag_s1;
extern int flag_s;
extern int flag_s2;

KEY_STATE key_value = KEY_STATE_NO_ACTION;
uint16_t key_arr[4] = {GPIO_Pin_6, GPIO_Pin_8, GPIO_Pin_7, GPIO_Pin_9};
uint16_t key_press_arr[4] = {KEY_PRESS_INDEX_1, KEY_PRESS_INDEX_2, KEY_PRESS_INDEX_3, KEY_PRESS_INDEX_4};
uint16_t key_release_arr[4] = {KEY_RELEASE_INDEX_1, KEY_RELEASE_INDEX_2, KEY_RELEASE_INDEX_3, KEY_RELEASE_INDEX_4};

void keyscan_row_gpio_Config(void);
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

    keyscan_row_gpio_Config();


    key_value = KEY_STATE_NO_ACTION;
}
void keyscan_interrupt_config(unsigned char flag)
{
    EXTI_InitTypeDef EXTI_InitStructure;

    /* EXTI line(PC6~PC9) mode config */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource6);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource7);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource8);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);
    EXTI_InitStructure.EXTI_Line = EXTI_Line6 | EXTI_Line7 | EXTI_Line8 | EXTI_Line9;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //下降沿中断
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;

    /*中断源配置*/
    if (1 == flag)
    {
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    }
    else
    {
        EXTI_InitStructure.EXTI_LineCmd = DISABLE;
        EXTI_ClearITPendingBit(EXTI_Line6 | EXTI_Line7 | EXTI_Line8 | EXTI_Line9);
    }
    EXTI_Init(&EXTI_InitStructure);

    {
        NVIC_InitTypeDef NVIC_InitStructure;

        /* Configure one bit for preemption priority */
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

        /* 配置P[A|B|C|D|E]0为中断源 */
        NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;

        /*中断源配置*/
        /*中断源配置*/
        if (1 == flag)
        {
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        }
        else
        {
            NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
            EXTI_ClearITPendingBit(EXTI_Line6 | EXTI_Line7 | EXTI_Line8 | EXTI_Line9);
        }
        NVIC_Init(&NVIC_InitStructure);
    }


}
void keyscan_row_gpio_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* config the extiline clock and AFIO clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

    /* EXTI line gpio config(PC6~PC9) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    keyscan_interrupt_config(1);

    //keyscan_interrupt_config(TRUE);
    {
        NVIC_InitTypeDef NVIC_InitStructure;

        /* Configure one bit for preemption priority */
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

        /* 配置P[A|B|C|D|E]0为中断源 */
        NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;

        /*中断源配置*/
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }

}
unsigned char keyscan_check(void)
{

    /*1. check all row status*/
    if ((Bit_RESET == GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6))
        || (Bit_RESET == GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7))
        || (Bit_RESET == GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8))
        || (Bit_RESET == GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)))
    {
#if KEY_LOG_EN
        printf("[Keyscan] key is still pressed.\r\n");
#endif
        return 1;
    }
    else
    {
        if (key_value == KEY_STATE_NO_ACTION)
        {
#if KEY_LOG_EN
            printf("[Keyscan] key no action.\r\n");
#endif
            return 0;
        }
        else
        {
#if KEY_LOG_EN
            printf("[Keyscan] key is released.\r\n");
#endif
            return 1;
        }
    }
}
void EXTI9_5_IRQHandler(void)
{
    if ((EXTI_GetITStatus(EXTI_Line6) != RESET) //确保是否产生了EXTI Line中断
        || (EXTI_GetITStatus(EXTI_Line7) != RESET)
        || (EXTI_GetITStatus(EXTI_Line8) != RESET)
        || (EXTI_GetITStatus(EXTI_Line9) != RESET))
    {
#if KEY_LOG_EN
        printf("[Keyscan] EXTI9_5_IRQHandler.\r\n");
#endif
        keyscan_interrupt_config(0);

        TIM_Cmd(TIM3, ENABLE); //使能定时器

        //清除中断标志位
        EXTI_ClearITPendingBit(EXTI_Line6 | EXTI_Line7 | EXTI_Line8 | EXTI_Line9);
    }
}
uint16_t key_scan(void)
{

    uint8_t index = 0;
    for (index = 0; index < 4; index ++)
    {
        if (Bit_RESET == GPIO_ReadInputDataBit(GPIOC, key_arr[index]))
        {
            key_value |= key_press_arr[index];
            key_value &= ~key_release_arr[index];
        }
        else
        {
            if (key_value & key_press_arr[index])
            {
                key_value &= ~key_press_arr[index];
                key_value |= key_release_arr[index];
            }
        }
    }

    return key_value;
}

/**
*  called by tim3 interrupter
*/
uint16_t get_key_value(void)
{
    uint16_t key_value = KEY_STATE_NO_ACTION;
    TIM_Cmd(TIM3, DISABLE); //使能定时器
#if KEY_LOG_EN
    printf("[Keyscan] vKeyscan_debunce_callback.\r\n");
#endif
    /*1. check keys whether is still pressed*/
    if (!keyscan_check())
    {
        keyscan_interrupt_config(1);
        return key_value;
    }
    /*2. scan key value*/
    {
        key_value = key_scan();
#if KEY_LOG_EN
        printf("[Keyscan] key_value = %#x.\r\n", key_value);
#endif
    }

    /*3. send to app msg*/
    {
        //app_send_msg(APP_MSG_KEYSCAN, 2, &key_value);
    }

    keyscan_interrupt_config(1);

    return key_value;
#if 0
    static u8 key = 1;
    if (key == 1 && (K_UP == 0 || K_DOWN == 0 || K_LEFT == 0)) //任意一个按键按下
    {
        delay_ms(10);  //消抖
        key = 0;
        if (K_UP == 0)
        {
            mod++;
            flag_s = 0;
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

#endif
}

