#include "stm32f10x.h" // Device header
#include "Delay.h"


static uint8_t KeyNum = 0;

void Key_Init(void){
    RCC->APB2ENR |= 0x00000008;
    GPIOB->CRL   &= 0xF0FFFFFF;
    GPIOB->CRL   |= 0x08000000;
}

uint8_t Key_GetNum(void){
    if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==Bit_RESET){
        Delay_ms(10);
        while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==Bit_RESET);
        Delay_ms(10);
        KeyNum ++;
    }
    KeyNum %= 4;
    return KeyNum;
}
