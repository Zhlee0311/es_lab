#include "stm32f10x.h"
#include <stdbool.h>
#define USART_BUF_SIZE 200

static uint8_t Rxbuf[USART_BUF_SIZE]; // 接收缓冲区，只负责接受PC机发来的数据
static uint8_t Txbuf[USART_BUF_SIZE]; // 发送缓冲区，只负责发送数据给PC机
static uint8_t Rxpos = 0;             // 接收数据位置，即接收缓冲区的下标
static uint8_t Txpos = 0;             // 发送数据位置，即发送缓冲区的下标
static uint8_t Txlen = 0;             // 待发送的数据长度（字节数）

uint16_t Data[USART_BUF_SIZE] = {0x1111, 0x2222, 0x3333}; // Data 模拟保持寄存器，提前写入三个数据作测试

uint8_t FrameFlag = 0;    // 帧标志位，用于判断是否接收到一帧数据
uint8_t RecvTimeOut = 10; // 接收超时标志位，用于判断是否完全接收到一帧数据

void Serial_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART1, USART_IT_TC, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    USART_Cmd(USART1, ENABLE);
}

uint8_t Serial_GetFlag(void)
{
    if (FrameFlag == 1)
    {
        FrameFlag = 0;
        return 1;
    }
    return 0;
}

uint8_t Serial_GetRxpos(void)
{
    return Rxpos;
} // 当前接收缓冲区的下标，Rxpos即为接收到的数据的字节数

uint16_t Serial_Odd(uint8_t *data, uint8_t len)
{
    uint16_t res = 0;
    for (uint8_t i = 0; i < len; i++)
    {
        uint8_t cnt = 0;
        uint8_t byte = data[i];
        while (byte)
        {
            cnt += byte & 0x01;
            byte >>= 1;
        }
        res += cnt;
    }
    if (res % 2)
    {
        return 0x0000; // 奇数个1,校验位为0
    }
    return 0x0001; // 偶数个1，校验位为1
}

bool Serial_FrameHandler(void)
{
    uint16_t check = Serial_Odd(Rxbuf, Rxpos - 2);
    if (check != ((Rxbuf[Rxpos - 2] << 8) | Rxbuf[Rxpos - 1]))
    {
        Rxpos = 0;
        Txpos = 0;
        Txlen = 0;
        return false; // 帧校验失败
    }

    Txbuf[0] = Rxbuf[0]; // 地址
    Txbuf[1] = Rxbuf[1]; // 功能码
    Rxpos = 0;
    Txlen = 0;

    switch (Txbuf[1])
    {
    case 0x03:
    {
        Txbuf[2] = Rxbuf[5] * 2; // 返回的数据总字节数
        Txlen = Txbuf[2] + 5;    // 地址(1)+功能码(1)+字节数(1)+数据+校验(2)
        for (uint8_t i = 0; i < Rxbuf[5]; i++)
        {
            Txbuf[3 + i * 2] = (Data[Rxbuf[3] + i] >> 8) & 0xFF;
            Txbuf[4 + i * 2] = Data[Rxbuf[3] + i] & 0xFF;
        }
        uint16_t check = Serial_Odd(Txbuf, Txlen - 2);
        Txbuf[Txlen - 2] = (check >> 8) & 0xFF;
        Txbuf[Txlen - 1] = check & 0xFF;
        break;
    }
    case 0x10:
    {
        Txlen = 8; // 地址(1)+功能码(1)+起始地址(2)+寄存器数量(2)+校验(2)
        for (uint8_t i = 2; i <= 5; i++)
        {
            Txbuf[i] = Rxbuf[i];
        }
        uint16_t check = Serial_Odd(Txbuf, Txlen - 2);
        Txbuf[Txlen - 2] = (check >> 8) & 0xFF;
        Txbuf[Txlen - 1] = check & 0xFF;
        // 以下开始写数据
        uint8_t base = Rxbuf[3];
        for (uint8_t i = 0; i < Rxbuf[5]; i++)
        {
            uint8_t high = Rxbuf[7 + i * 2];
            uint8_t low = Rxbuf[8 + i * 2];
            uint16_t wdata = (high << 8) | low;
            Data[base + i] = wdata;
        }

        break;
    }
    default:
        return false;
    }

    // 手动发送第一个字节，启动发送
    Txpos = 0;
    USART_ITConfig(USART1, USART_IT_TC, DISABLE); // 禁用发送中断
    USART_SendData(USART1, Txbuf[Txpos]);
    USART_ITConfig(USART1, USART_IT_TC, ENABLE); // 启用发送中断

    return true;
}

/*void Serial_SendByte(uint8_t byte)
{
    USART_SendData(USART1, byte);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        ;
}

void Serial_SendArray()
{
    for (uint8_t i = 0; i < Txlen; i++)
    {
        Serial_SendByte(Txbuf[i]);
    }
}*/

void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE))
    {
        if (Rxpos < USART_BUF_SIZE)
        {
            Rxbuf[Rxpos++] = USART_ReceiveData(USART1);
        }
        RecvTimeOut = 10;
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
    if (USART_GetITStatus(USART1, USART_IT_TC))
    {
        USART_ClearITPendingBit(USART1, USART_IT_TC);
        Txpos++;
        if (Txpos < Txlen)
        {
            USART_SendData(USART1, Txbuf[Txpos]);
        }
    }
}
