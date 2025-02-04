#include "stm32f10x.h"

void SysTick_Init(uint32_t us)
{
    SysTick->LOAD = (72 * us) - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

