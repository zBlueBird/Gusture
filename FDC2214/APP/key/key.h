#ifndef _key_H
#define _key_H


#include "system.h"

#define KEY_LEFT_Pin     GPIO_Pin_13| GPIO_Pin_14  |GPIO_Pin_15   //定义K_LEFT管脚

typedef enum
{

    KEY_STATE_NO_ACTION = 0,

    KEY_PRESS_INDEX_1 = 0x01,
    KEY_PRESS_INDEX_2 = 0x02,
    KEY_PRESS_INDEX_3 = 0x04,
    KEY_PRESS_INDEX_4 = 0x08,

    KEY_RELEASE_INDEX_1 = 0x10,
    KEY_RELEASE_INDEX_2 = 0x20,
    KEY_RELEASE_INDEX_3 = 0x40,
    KEY_RELEASE_INDEX_4 = 0x80,

} KEY_STATE;

#define KEY_Port (GPIOB) //定义端口
#define RCC_key RCC_APB2Periph_GPIOB


//使用位操作定义
#define K_UP PBin(13)
#define K_DOWN PBin(14)
#define K_LEFT PBin(15)








void KEY_Init(void);
//void KEY_Scan(void);
uint16_t get_key_value(void);
#endif
