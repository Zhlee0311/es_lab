#include "stm32f10x.h"

volatile uint8_t curDigit = 0; // 当前数码管的索引（0-3）

// 分时扫描数码管
static uint16_t Nums_Bit[4];
static uint16_t Nums_Seg[16] = {
    GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5,
    GPIO_Pin_1 | GPIO_Pin_2,
    GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6,
    GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6,
    GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6,
    GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6,
    GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6,
    GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2,
    GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6,
    GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6,
    GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6,
    GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6,
    GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5,
    GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6,
    GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6,
    GPIO_Pin_0 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6,
};

void Light(uint16_t port_C, uint16_t port_D)
{
    GPIOC->BSRR = 0x00003C00;
    GPIOC->BRR = port_C;

    GPIOD->BSRR = 0x000000FF;
    GPIOD->BRR = port_D;
}

void Dig_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_WriteBit(GPIOC, GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13, Bit_SET);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_WriteBit(GPIOD, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, Bit_SET);
}

void Dig_Set(void)
{
    uint16_t port_C;
    uint16_t port_D;
    port_C = 1 << (10 + curDigit);
    port_D = Nums_Bit[curDigit];
    Light(port_C, port_D);
}

void SysTick_Handler(void)
{
    curDigit = (curDigit + 1) % 4;
}

/*
void Dig_Config(uint16_t Nums)
{
    for (int i = 0; i < 3; i++)
    {
        Nums_Bit[i] = Nums_Bit[i + 1];
    }
    Nums_Bit[3] = Nums_Seg[Nums];
}
*/

void Dig_Config(uint16_t Nums)
{

    Nums_Bit[0] = Nums_Seg[(Nums >> 12) & 0x000F];
    Nums_Bit[1] = Nums_Seg[(Nums >> 8) & 0x000F];
    Nums_Bit[2] = Nums_Seg[(Nums >> 4) & 0x000F];
    Nums_Bit[3] = Nums_Seg[Nums & 0x000F];
    /*
    Nums_Bit[0] = GPIO_Pin_1 | GPIO_Pin_2;
    Nums_Bit[1] = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6;
    Nums_Bit[2] = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6;
    Nums_Bit[3] = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6;
    */
}
