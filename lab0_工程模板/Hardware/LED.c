#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "Key.h"

void output(uint32_t data)
{
    GPIOE->BSRR = 0xF << 8;
    GPIOE->BRR = data;
}

void LED_Init(void)
{
    RCC->APB2ENR |= 0x00000040;
    GPIOE->CRH &= 0xFFFF0000;
    GPIOE->CRH |= 0x00003333;
    GPIOE->ODR |= 0x00000F00;
}

void Mode_0(uint32_t *data)
{
    for (int i = 0; i < 4; i++)
    {
        *data = (0x00000100) << i;
        output(*data);
        Delay_ms(10);
        if (Key_GetNum() != 0)
        {
            break;
        }
    }
}

void Mode_1(uint32_t *data)
{
    *data = 0x00000F00;
    output(*data);
} // 全亮

void Mode_2(uint32_t *data)
{
    *data = 0x00000000;
    output(*data);
} // 全灭

void Mode_3(uint32_t *data)
{
    for (int i = 0; i < 2; i++)
    {
        *data = (*data == 0x00000000) ? 0x00000F00 : 0x00000000;
        output(*data);
        Delay_ms(10);
        if (Key_GetNum() != 3)
        {
            break;
        }
    }
}
