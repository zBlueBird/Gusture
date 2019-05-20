#ifndef _FDC2214_H
#define _FDC2214_H

#include "system.h"
#include "iic.h"


//?????
//#define DATA_MSB_CH0    0x00
#define DATA_CH0    0x00
#define DATA_LSB_CH0    0x01
#define DATA_CH1    0x02
#define DATA_LSB_CH1    0x03
#define DATA_CH2    0x04
#define DATA_LSB_CH2    0x05
#define DATA_CH3    0x06
#define DATA_LSB_CH3    0x07
#define DATA0 0x00
#define DATA1 0x02
#define DATA2 0x04
#define DATA3 0x06
#define RCOUNT0   0x08
#define RCOUNT1   0x09
#define RCOUNT2   0x0A
#define RCOUNT3   0x0B
#define OFFSET_CH0  0x0C
#define OFFSET_CH1  0x0D
#define OFFSET_CH2  0x0E
#define OFFSET_CH3  0x0F
#define SETTLECOUNT0    0x10
#define SETTLECOUNT1    0x11
#define SETTLECOUNT2    0x12
#define SETTLECOUNT3    0x13
#define CLOCK_DIVIDERS_CH0  0x14
#define CLOCK_DIVIDERS_CH1  0x15
#define CLOCK_DIVIDERS_CH2  0x16
#define CLOCK_DIVIDERS_CH3  0x17
#define STATUS  0x18
#define ERROR_CONFIG    0x19
#define CONFIG  0x1A
#define MUX_CONFIG  0x1B
#define RESET_DEVICE    0x1C
#define SYSTEM_CLOCK_CONFIG 0x1D
#define DRIVE_CURRENT_CH0   0x1E
#define DRIVE_CURRENT_CH1   0x1F
#define DRIVE_CURRENT_CH2   0x20
#define DRIVE_CURRENT_CH3   0x21
#define MANUFACTID  0x7E
#define DEVICE_ID   0x7F



void FDC2214_Init(void);
void WriteByte2214(unsigned char REG_Address, unsigned char REG_Data);
unsigned char ReadByte2214(unsigned char REG_Address);
void WriteWord2214(unsigned char REG_Address, unsigned int REG_Data);
unsigned int ReadWord2214(unsigned char REG_Address);
unsigned long FDC2214_GetResult(unsigned char Channal);
unsigned char HexToChar(unsigned char bHex);
int dianrongzhi(int f1);



#endif
