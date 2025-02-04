#include "stm32f10x.h" // Device header
#include "Key.h"
#include "Dig.h"

int main(void)
{
    Key_Init();
    Dig_Init();

    uint8_t state = 0;
    uint16_t port_C = 0;
    uint16_t port_D = 0;

    while (1)
    {
        Dig_Set(&port_C, &port_D);

        state = Key_GetNum();
        switch (state)
        {
        case 0:
            Config_0();
            break;
        case 1:
            Config_1();
            break;
        case 2:
            Config_2();
            break;
        case 3:
            Config_3();
            break;
        default:
            Config_0();
            break;
        }
    }
}
