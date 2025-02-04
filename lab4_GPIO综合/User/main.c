#include "stm32f10x.h" // Device header
#include "Button.h"
#include "Dig.h"

int main(void)
{
    Dig_Init();
    Button_Init();

    uint16_t port_C = 0;
    uint16_t port_D = 0;

    int last = -2;
    int released = 1;
    while (1)
    {
        int button = Button_Scan();
        if (button != -1)
        {
            if (button != last || (button == last && released))
            {
                released = 0;
                Dig_Config(button);
                last = button;
            }
        }
        else
        {
            released = 1;
        }
        Dig_Set(&port_C, &port_D);
    }
}
