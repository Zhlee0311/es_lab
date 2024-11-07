#include "stm32f10x.h" // Device header
#include "LED.h"
#include "Key.h"

int main(void)
{

    LED_Init();
    Key_Init();

    uint8_t state = 0;
    uint32_t data = 0;

    while (1)
    {
        state = Key_GetNum();

        switch (state)
        {
        case 0:
            Mode_0(&data);
            break;
        case 1:
            Mode_1(&data);
            break;
        case 2:
            Mode_2(&data);
            break;
        case 3:
            Mode_3(&data);
            break;
        default:
            Mode_1(&data);
            break;
        }
    }
}
