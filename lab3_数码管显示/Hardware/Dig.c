#include "stm32f10x.h"
#include "Delay.h"
static uint16_t Nums[4];

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
        *port_C = 1 << (10 + i);
        *port_D = Nums[i];
        Light(*port_C, *port_D);
        Delay_us(20);
    }
}

void Config_0(void)
{
    Nums[0] = GPIO_Pin_1 | GPIO_Pin_2;
    Nums[1] = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6;
    Nums[2] = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6;
    Nums[3] = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6;
}

void Config_1(void)
{
    Nums[0] = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    Nums[1] = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    Nums[2] = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    Nums[3] = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
}

void Config_2(void)
{
    Nums[0] = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    Nums[1] = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    Nums[2] = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    Nums[3] = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
}

void Config_3(void)
{
    Nums[0] = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    Nums[1] = GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    Nums[2] = GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    Nums[3] = GPIO_Pin_0 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
}
