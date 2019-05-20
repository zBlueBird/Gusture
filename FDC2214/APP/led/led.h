#ifndef _led_H
#define _led_H

#include "system.h"

/*  LED时钟端口、引脚定义 */
#define LED_PORT            GPIOC
#define LED_PIN             GPIO_Pin_13
#define LED_PORT_RCC        RCC_APB2Periph_GPIOC


#define LED_PORT1           GPIOA
#define LED_PIN1        GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2
#define LED_PORT_RCC1       RCC_APB2Periph_GPIOA
#define LED_PORT2           GPIOB
#define LED_PIN2        GPIO_Pin_0|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9
#define LED_PORT_RCC2       RCC_APB2Periph_GPIOB

#define led1 PCout(13)


#define shitou  PAout(0)
#define jiandao PBout(9)
#define bu      PBout(6)
#define nu1     PAout(1)
#define nu2         PBout(8)
#define nu3         PBout(0)
#define nu4         PAout(2)
#define nu5         PBout(7)



void LED_Init(void);
void LED_Init1(void);


#endif
