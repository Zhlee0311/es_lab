#include "stm32f10x.h"
#include <stdbool.h>

/*
#define USART_BUF_SIZE 200
static uint8_t Rxbuf[USART_BUF_SIZE]; // 接收缓冲区，只负责接收PC机发来的数据
static uint8_t Txbuf[USART_BUF_SIZE]; // 发送缓冲区，只负责发送数据到PC机
static uint8_t Rxpos = 0;             // 接收数据位置，即接收缓冲区的下标
static uint8_t Txpos = 0;             // 发送数据位置，即发送缓冲区的下标
static uint8_t Txlen = 0;             // 待发送数据长度（字节数）
*/

uint8_t Serial_RxData[2]; // 每一帧有两个字节的数据
uint8_t Serial_RxFlag;

typedef enum
{
    empty = 0, // 空
    black,     // 黑棋
    white,     // 白棋
} ChessPiece;

extern ChessPiece board[5][5];

void Serial_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用PA9作为串口，TX
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // PA10上拉输入，RX
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

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // RX寄存器非空_中断
    // USART_ITConfig(USART1, USART_IT_TC, ENABLE);   // 发送完成_中断

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART1, ENABLE);
}

void Serial_Send(void)
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            uint8_t dataToSend = 0x00; // 默认是 empty
            if (board[i][j] == black)
            {
                dataToSend = 0x01; // 黑子
            }
            else if (board[i][j] == white)
            {
                dataToSend = 0x02; // 白子
            }

            USART_SendData(USART1, dataToSend);

            // 等待直到发送完成
            while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
                ;
        }
    }
}

bool Serial_GetRxFlag()
{
    if (Serial_RxFlag == 2) // 收到两个字节才代表一帧
    {
        Serial_RxFlag = 0;
        return true;
    }
    return false;
}

uint16_t Serial_GetRxData()
{
    uint16_t temp = (Serial_RxData[0] << 8) | Serial_RxData[1];
    return temp;
}

void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        Serial_RxData[Serial_RxFlag++] = USART_ReceiveData(USART1);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}
