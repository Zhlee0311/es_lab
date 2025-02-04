#include "stm32f10x.h"
#include "Delay.h"

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
    RCC->APB2ENR |= 3 << 4;

    GPIOC->CRH &= 0xFF0000FF;
    GPIOC->CRH |= 0x00333300;
    GPIOC->ODR |= 0xF << 10;

    GPIOD->CRL &= 0x00000000;
    GPIOD->CRL |= 0x33333333;
    GPIOD->ODR |= 0x000000FF;
}

void Dig_Set(uint16_t *port_C, uint16_t *port_D)
{
    for (int i = 0; i < 4; i++)
    {
        Delay_us(50);
        *port_C = 1 << (10 + i);
        *port_D = Nums_Bit[i];
        Light(*port_C, *port_D);
        
    }
}

void Dig_Config(uint16_t Nums)
{
    for (int i = 0; i < 3; i++)
    {
        Nums_Bit[i] = Nums_Bit[i + 1];
    }
    Nums_Bit[3] = Nums_Seg[Nums];
}
