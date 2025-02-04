#ifndef __UART_H
#define __UART_H

/**
 * @brief 串口引脚的配置
 */
void RS232_Configuration(void);

/**
 * @brief NVIC的配置，用于串口中断
 */
void NVIC_Configuration(void);

#endif
