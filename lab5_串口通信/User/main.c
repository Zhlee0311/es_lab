#include "stm32f10x.h"
#include "Dig.h"
#include "Timer.h"
#include "Serial.h"
#include "Systick.h"

int main(void)
{
    Dig_Init();       // 初始化数字显示模块
    Timer_Init();     // 初始化定时器
    Serial_Init();    // 初始化串口
    SysTick_Init(50); // 初始化SysTick定时器

    uint16_t nums = 0xFFFF;

    uint8_t len = 0x00;
    uint8_t sign = 0x00;

    while (1)
    {
        if (Serial_GetFlag() && Serial_GetRxpos())
        {
            len = Serial_GetRxpos();
            bool res = Serial_FrameHandler();
            if (res)
            {
                sign = 0x01;
            }
            else
            {
                sign = 0x02;
            }
        }
        nums = ((len << 8) & 0xFF00) | (sign & 0x00FF);
        Dig_Config(nums);
        Dig_Set();                                                                   
    }
}
