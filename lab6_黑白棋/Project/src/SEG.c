/***********************************************************************
�ļ����ƣ�LED.C
��    �ܣ�led  IO��ʼ��
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺
***********************************************************************/
#include "stm32f10x.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_pwr.h"

/***********************************************************************
�������ƣ�SEG_Configuration(void)
��    �ܣ����LED������
���������
���������
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺LED1->PE2,
		  LED2->PE3,
		  LED3->PE4,
		  LED4->PE5,
***********************************************************************/
void SEG_Configuration(void)
{
	
	/*******************�⺯����************************/
//	GPIO_InitTypeDef GPIO_InitStructure;
//	/* Enable the GPIO_LED Clock */
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC , ENABLE); 						 
//			
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | \
//					GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
	/*****************�Ĵ�����**********************/
	RCC->APB2ENR|=3<<4;    //ʹ��PORTC��PORTDʱ��	 
	GPIOC->CRH&=0xFF0000FF; 
	GPIOC->CRH|=0x00333300;//PC.10 11 12 13 �������   	
//	GPIOC->ODR|=15<<10;      //PC.10 11 12 13 �����

	GPIOD->CRL&=0X00000000; 
	GPIOD->CRL|=0X33333333;//PD.0 1 2 3 4 5 6 7 �������   
//  GPIOD->ODR |= 0xFF;      //PD.0 1 2 3 4 5 6 7 �����
	
	GPIOD->BSRR = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;      //����ߵ�ƽ
	GPIOD->BSRR = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIOC->BSRR = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
}

void SEG_test()
{		
		int i ;
		u8 Seg_Code[17]={ 0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xCC,0xA1,0x86,0x8e,0x7F};
		SEG_Configuration();
		while(1)
		{
				GPIO_ResetBits(GPIOC, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_10 | GPIO_Pin_11);
				for( i=0 ; i < 17; i ++ )
				{
					GPIOD->ODR &= 0xff00;
					GPIOD->ODR |= Seg_Code[i];
					delay_ms(1000);
				}
		}
}
