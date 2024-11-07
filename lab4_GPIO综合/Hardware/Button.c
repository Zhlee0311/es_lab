#include "stm32f10x.h" 
#include "Delay.h"

//反转法扫描矩阵键盘 

static uint8_t Button_Index[16] = {
    0xEE, 0xED, 0xEB, 0xE7,
    0xDE, 0xDD, 0xDB, 0xD7,
    0xBE, 0xBD, 0xBB, 0xB7,
    0x7E, 0x7D, 0x7B, 0x77};

void Button_Init(void)
{
    RCC->APB2ENR |= 1 << 6;
}

int Button_Scan(void)
{

    GPIOE->CRL = 0x33338888;
    GPIOE->ODR = 0x0F;
    Delay_us(1);
    uint8_t col = GPIOE->IDR & 0x0F;

    GPIOE->CRL = 0x88883333;
    GPIOE->ODR = 0xF0;
    Delay_us(1);
    uint8_t row = GPIOE->IDR & 0xF0;

    uint8_t button = col + row;

    for (int i = 0; i < 16; i++)
    {
        if (button == Button_Index[i])
        {
            return i; // 0~15
        }
    }
    return -1;
}
