/***********************************************************************
�ļ����ƣ�LED.C
��    �ܣ�led  IO��ʼ��
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺
***********************************************************************/
#include "stm32f10x.h"

void PWM_Configuration(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE|RCC_APB2Periph_TIM1|RCC_APB2Periph_AFIO, ENABLE);	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE); //Timer1?????  ?????????? tim1??????? ???tim1
	
// 	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_8 | GPIO_Pin_9  ;  		//
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
// 	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_0 | GPIO_Pin_1  ;  		//
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);	
// 	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_8 | GPIO_Pin_13  ;  		//
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOE, &GPIO_InitStructure);		
	
		GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13  ;  		//
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOE, &GPIO_InitStructure);	
	
		//TIM_DeInit(TIM1); //
	
    //����ʱ������������ã�TIM2Ƶ��Ϊ72M/TIM_Period
		TIM_TimeBaseStructure.TIM_Period = arr;   						//����       
		TIM_TimeBaseStructure.TIM_Prescaler = psc;                        //����Ƶ
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;                    //ʱ�ӷָ�Ϊ0  
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //���ϼ���ģʽ    
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 

    //�������ģʽ
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;               //�����ȵ���ģʽ1 
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //���״̬ʹ��  
		//�������ʹ��
   // TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; 
		
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;        //������Ե�
    //TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
		//TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
		//TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
		//TIM_OCInitStructure.TIM_Pulse = 0;  							//ռ�ձ�
		TIM_OC1Init(TIM1, &TIM_OCInitStructure);						//T3-CH1
		TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);		
		TIM_OC2Init(TIM1, &TIM_OCInitStructure);						//T3-CH2
		TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);		
		TIM_OC3Init(TIM1, &TIM_OCInitStructure);						//T3-CH3
		TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);	
		
		//TIM_ARRPreloadConfig(TIM1, ENABLE);
		TIM_Cmd(TIM1, ENABLE) ;

		//TIM1->RCR = 0;                   
		//TIM_CtrlPWMOutputs(TIM1,ENABLE);        //TIM1��ʱ��
   		
}

#define AIRCR_VECTKEY_MASK    ((uint32_t)0x05FA0000)
void PWM_test()
{
		u16 pwmval=0;
		u8 dir =1;
		SCB->AIRCR = AIRCR_VECTKEY_MASK | NVIC_PriorityGroup_2;     //����NVIC�жϷ���2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	  LED_Configuration();
//		GPIOE->BSRR = GPIO_Pin_8;
		GPIOE->BSRR = GPIO_Pin_9;
//		GPIOE->BSRR = GPIO_Pin_10;
		GPIOE->BSRR = GPIO_Pin_11;
	while(1)
	{
		delay_ms(200);
		if(dir) pwmval++;
		else pwmval--;
		if(pwmval>=900)dir=0;
		if(pwmval==0) dir=1;
		TIM1->CCR1 = pwmval; //ʱ�Ӽ���ֵ���ֵ5000�����ѹ���ֵ5000��Χ��ͬ������ֱ�Ӹ�ֵ
		//TIM1->CCR2 = 0 ; //ʱ�Ӽ���ֵ���ֵ5000�����ѹ���ֵ5000��Χ��ͬ������ֱ�Ӹ�ֵ
//		TIM1->CCR3 = 3000 ; //ʱ�Ӽ���ֵ���ֵ5000�����ѹ���ֵ5000��Χ��ͬ������ֱ�Ӹ�ֵ
	}
}