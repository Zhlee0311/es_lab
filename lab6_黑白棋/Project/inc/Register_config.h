#ifndef __Register_Config_H
#define __Register_Config_H

#define RCC_AHBENR (*((volatile unsigned int *)0x40021014))  // AHB����ʱ��ʹ�ܼĴ���
#define RCC_APB2ENR (*((volatile unsigned int *)0x40021018)) // APB2 ����ʱ��ʹ�ܼĴ���
#define RCC_APB1ENR (*((volatile unsigned int *)0x4002101C)) // APB1 ����ʱ��ʹ�ܼĴ���

#define GPIOE_BASE_ADDR (0x40011800)
#define GPIOE_CRL (*((volatile unsigned int *)(GPIOE_BASE + 0x00)))  // �˿����õͼĴ���
#define GPIOE_CRH (*((volatile unsigned int *)(GPIOE_BASE + 0x04)))  // �˿����ø߼Ĵ���
#define GPIOE_IDR (*((volatile unsigned int *)(GPIOE_BASE + 0x08)))  // �˿��������ݼĴ���
#define GPIOE_ODR (*((volatile unsigned int *)(GPIOE_BASE + 0x0C)))  // �˿�������ݼĴ���
#define GPIOE_BSRR (*((volatile unsigned int *)(GPIOE_BASE + 0x10))) // �˿�λ����/��λ�Ĵ���
#define GPIOE_BRR (*((volatile unsigned int *)(GPIOE_BASE + 0x14)))  // �˿�λ��λ�Ĵ���

#define GPIOB_BASE_ADDR (0x40010C00)
#define GPIOB_CRL (*((volatile unsigned int *)0x40010C00))  // �˿����õͼĴ���
#define GPIOB_BSRR (*((volatile unsigned int *)0x40010C10)) // �˿����õͼĴ���

#define DMA_BASE_ADDR (0x40020000)
#define DMA_ISR (*((volatile unsigned int *)0x40020000))    // DMA�ж�״̬�Ĵ���
#define DMA_IFCR (*((volatile unsigned int *)0x40020004))   // DMA�жϱ�־����Ĵ���
#define DMA_CCR4 (*((volatile unsigned int *)0x40020044))   // DMAͨ��4���üĴ�����ַ
#define DMA_CCR5 (*((volatile unsigned int *)0x40020058))   // DMAͨ��5���üĴ�����ַ
#define DMA_CNDTR4 (*((volatile unsigned int *)0x40020048)) // DMAͨ��4���������Ĵ�����ַ
#define DMA_CNDTR5 (*((volatile unsigned int *)0x4002005C)) // DMAͨ��5���������Ĵ�����ַ
#define DMA_CPAR4 (*((volatile unsigned int *)0x4002004C))  // DMAͨ��4�����ַ�Ĵ�����ַ
#define DMA_CPAR5 (*((volatile unsigned int *)0x40020060))  // DMAͨ��5�����ַ�Ĵ�����ַ
#define DMA_CMAR4 (*((volatile unsigned int *)0x40020050))  // DMAͨ��4�洢����ַ�Ĵ�����ַ
#define DMA_CMAR5 (*((volatile unsigned int *)0x40020064))  // DMAͨ��5�洢����ַ�Ĵ�����ַ

#define USART1_BASE_ADDR (0x40013800)
#define USART1_DR (*((volatile unsigned int *)0x40013804))  // ����1���ݼĴ�����ַ
#define USART1_CR3 (*((volatile unsigned int *)0x40013814)) // ���ƼĴ��� 3

#define SCB_AIRCR (*((volatile unsigned int *)0xE000ED0C))    // Ӧ���ж�/��λ���ƼĴ���
#define NVIC_ISER ((volatile unsigned int *)0xE000E100)       // Irqʹ�����üĴ���
#define NVIC_IP ((volatile unsigned char *)0xE000E400)        // Irq���ȼ��Ĵ���(�� STM32 ��ֻʹ�� 4 λ���� 4 λ��Ч)
#define EXTI_IMR (*((volatile unsigned int *)0x40010400))     // �ⲿ�ж����μĴ���
#define EXTI_RTSR (*((volatile unsigned int *)0x40010408))    // �����ش���ѡ��Ĵ���
#define EXTI_FTSR (*((volatile unsigned int *)0x4001040c))    // �½��ش���ѡ��Ĵ���
#define EXTI_PR (*((volatile unsigned int *)0x40010414))      // ����Ĵ���
#define AFIO_EXTICR2 (*((volatile unsigned int *)0x4001000C)) // �ⲿ�ж����üĴ���2
#endif
/******END OF FILE****/
