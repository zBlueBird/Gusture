#include "FDC2214.h"
#include "SysTick.h"
#include "iic.h"

/*******************************************************************************
* 函 数 名         : AT24CXX_Init
* 函数功能         : AT24CXX初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
#define SLAVE_ADDR  0x2A
#define SLAVE_ADDR_W    SLAVE_ADDR<<1
#define SLAVE_ADDR_R    (SLAVE_ADDR<<1)+1



void WriteWord2214(unsigned char REG_Address, unsigned int REG_Data)
{
    unsigned char REG_Data_H, REG_Data_L;
    REG_Data_L = (unsigned char)(REG_Data & 0xFF);
    REG_Data_H = (unsigned char)((REG_Data & 0xFF00) >> 8);

    IIC_Start();
    IIC_Send_Byte(SLAVE_ADDR_W);
    IIC_Wait_Ack();
    IIC_Send_Byte(REG_Address);
    IIC_Wait_Ack();
    IIC_Send_Byte(REG_Data_H);
    IIC_Wait_Ack();
    IIC_Send_Byte(REG_Data_L);
    IIC_Wait_Ack();
    IIC_Stop();
}

unsigned int ReadWord2214(unsigned char REG_Address)
{
    unsigned int REG_Data;
    unsigned char REG_Data_H = 1;
    unsigned char REG_Data_L = 1;
    IIC_Start();
    IIC_Send_Byte(SLAVE_ADDR_W);
    IIC_Wait_Ack();
    IIC_Send_Byte(REG_Address);
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte(SLAVE_ADDR_R);
    IIC_Wait_Ack();
    REG_Data_H = IIC_Read_Byte(1);
    REG_Data_L = IIC_Read_Byte(0);
    IIC_Stop();
    REG_Data = REG_Data_H;
    REG_Data <<= 8;
    REG_Data |= REG_Data_L;
    return REG_Data;
}


unsigned long FDC2214_GetResult(unsigned char Channal)
{
    unsigned long temp = 0;
    switch (Channal)
    {
    case 0:
        {
            temp = ReadWord2214(DATA_CH0);
//                    printf("%zd\n",temp);
            temp = temp << 16;
//                    printf("%zd\n",temp);
            temp |= (ReadWord2214(DATA_LSB_CH0));
//                      printf("%zd\n",temp);
        }
        break;
    case 1:
        {
            temp = ReadWord2214(DATA_CH1);
            temp = temp << 16;
            temp |= (ReadWord2214(DATA_LSB_CH1));
        }
        break;
    case 2:
        {
            temp = ReadWord2214(DATA_CH2);
            temp = temp << 16;
            temp |= (ReadWord2214(DATA_LSB_CH2));
        }
        break;
    case 3:
        {
            temp = ReadWord2214(DATA_CH3);
            temp = temp << 16;
            temp |= (ReadWord2214(DATA_LSB_CH3));
        }
        break;
    default:
        break;
    }
    return temp;
}



void WriteByte2214(unsigned char REG_Address, unsigned char REG_Data)
{
    IIC_Start();
    IIC_Send_Byte(SLAVE_ADDR_W);
    IIC_Wait_Ack();
    IIC_Send_Byte(REG_Address);
    IIC_Wait_Ack();
    IIC_Send_Byte(REG_Data);
    IIC_Wait_Ack();
    IIC_Stop();
}

unsigned char ReadByte2214(unsigned char REG_Address)
{
    unsigned char REG_Data;
    IIC_Start();
    IIC_Send_Byte(SLAVE_ADDR_W);
    IIC_Wait_Ack();
    IIC_Send_Byte(REG_Address);
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte(SLAVE_ADDR_R);
    IIC_Wait_Ack();
    REG_Data = IIC_Read_Byte(0);
    IIC_Stop();
    return REG_Data;
}



void FDC2214_Init()
{
    IIC_Init();

    while (ReadWord2214(DEVICE_ID) != 0x3055);

    WriteWord2214(CONFIG, 0x2801);
    WriteWord2214(RCOUNT0, 0xFFFF);
    WriteWord2214(RCOUNT1, 0xFFFF);
    WriteWord2214(RCOUNT2, 0xFFFF);
    WriteWord2214(RCOUNT3, 0xFFFF);
    WriteWord2214(SETTLECOUNT0, 0x0100);
    WriteWord2214(SETTLECOUNT1, 0x0100);
    WriteWord2214(SETTLECOUNT2, 0x0100);
    WriteWord2214(SETTLECOUNT3, 0x0100);
    WriteWord2214(CLOCK_DIVIDERS_CH0, 0x1002);  //FIN_DIVIDER0 =1;FREF_DIVIDER0 = 4
    WriteWord2214(CLOCK_DIVIDERS_CH1, 0x1002);
    WriteWord2214(CLOCK_DIVIDERS_CH2, 0x1002);  //FIN_DIVIDER0 =1;FREF_DIVIDER0 = 4
    WriteWord2214(CLOCK_DIVIDERS_CH3, 0x1002);
    WriteWord2214(ERROR_CONFIG, 0x0000);
    WriteWord2214(MUX_CONFIG, 0x420D);
    WriteWord2214(DRIVE_CURRENT_CH0, 0x4800);
    WriteWord2214(DRIVE_CURRENT_CH1, 0x4800);
    WriteWord2214(DRIVE_CURRENT_CH2, 0x4800);
    WriteWord2214(DRIVE_CURRENT_CH3, 0x4800);
    //  WriteWord1314(RESET_DEVICE,0x0400);
    WriteWord2214(CONFIG, 0x1401);
}

#include "usart1.h"
int dianrongzhi(int f1)
{
    double pi = 3.141592;
    double L = 18;
    double C = 0.000033;
    double k = 1.19;
    double cs, fs, x;
    unsigned long date1;
    int c;

    date1 = FDC2214_GetResult(f1);

    fs = (date1 / 16);
    printf("[FDC2214] CH0 value1 :%#f\r\n", fs);
    fs = (fs / 256);
    printf("[FDC2214] CH0 value2 :%#f\r\n", fs);
    fs = (fs / 256);
    printf("[FDC2214] CH0 value3 :%#f\r\n", fs);
    fs = (fs / 256);
    printf("[FDC2214] CH0 value4 :%#f\r\n", fs);
    fs = 15000 * fs;
    printf("[FDC2214] CH0 value5 :%#f\r\n", fs);
    x = 2 * pi * fs;
    cs = 1000 / x;
    cs = cs * cs;
    cs = (cs / L - C) * 1000000 - 63;
    cs = cs / 2;
    cs = cs * k;
    cs = 1 / L + 1000 / x + 1000 / x - C;
    c = ((cs * 1000) - 160) * 100;
    printf("[FDC2214] CH0 value :%d\r\n", c);
    return c;


}



















