/***********************************************************************
�ļ����ƣ�LED.C
��    �ܣ�led  IO��ʼ��
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺
***********************************************************************/
#include "stm32f10x.h"



void Counter_Configuration()
{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_ClocksTypeDef RCC_ClockFreq;
//	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;	
//	TIM_ICInitTypeDef TIM_ICInitStructure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	
//	
//	/*PB6-> ?????? */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50M????
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//		
//	
//	  TIM_TimeBaseStructure.TIM_Period = 0x1000;   //ARR,     
//	  TIM_TimeBaseStructure.TIM_Prescaler = 0;       
//	  TIM_TimeBaseStructure.TIM_ClockDivision = 0;    
//	  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
//	  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
//	
//	  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
//	  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
//	  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//	  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//	  TIM_ICInitStructure.TIM_ICFilter = 0x0;
//	  
//	  TIM_ICInit(TIM4, &TIM_ICInitStructure);
//	  TIM_SelectInputTrigger(TIM4, TIM_TS_TI1FP1);
//	
//	  /* Select the slave Mode: Reset Mode */
//	  TIM4->SMCR |= 0x07 ;  //�ⲿ����ģʽ
//	  /* Enable the Master/Slave Mode */

//	TIM_SetCounter(TIM4,0);
//	TIM_Cmd(TIM4, ENABLE);
	
		/*****************�Ĵ�����**********************/
		RCC->APB2ENR|=1<<3;    //ʹ��PORTBʱ��	
		GPIOB->CRL|=0x04000000;  //PB.6��������  
	  
		RCC->APB1ENR|=1<<2;    //ʹ��TIM4ʱ��	
		TIM4->ARR=0x1000;  	//�趨�������Զ���װֵ
		TIM4->PSC=0;  	    //��Ԥ��Ƶ
		
		TIM4->CCMR1|=1<<0;		//CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
		TIM4->CCMR1|=0<<4; 		//IC1F=0000 ���������˲��� ���˲�
		TIM4->CCMR1|=0<<10; 	//IC2PS=00 	���������Ƶ,����Ƶ 

		TIM4->CCER|=0<<1; 		//CC1P=0	�����ز���
		TIM4->CCER|=1<<0; 		//CC1E=1 	�������������ֵ������Ĵ�����
		
		TIM4->SMCR = 0x50 ;   //ѡ��ͬ���������Ĵ���ģʽΪ��ʱ������1
	  TIM4->SMCR |= 0x07;  //�ⲿ����ģʽ
		TIM4->CNT=0;         //��ʼ��������
		TIM4->CR1|=0x01;    //ʹ�ܶ�ʱ��4*/
		
}


int TimeCnt=0;
void Counter_test()
{
	  LED_Configuration();
		Counter_Configuration();
		GPIOE->BSRR = GPIO_Pin_8;
		GPIOE->BSRR = GPIO_Pin_9;
		GPIOE->BSRR = GPIO_Pin_10;
		GPIOE->BSRR = GPIO_Pin_11;
		while(1)
		{
			TimeCnt = TIM4->CNT;
			if(TimeCnt%2==0)
				GPIOE->BSRR = GPIO_Pin_8;
			else GPIOE->BRR = GPIO_Pin_8;
		}
}