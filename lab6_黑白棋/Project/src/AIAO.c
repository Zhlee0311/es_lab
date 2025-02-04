/***********************************************************************
�ļ����ƣ�LED.C
��    �ܣ�led  IO��ʼ��
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺
***********************************************************************/
#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dac.h"
void AI_Configuration()
{      
////ADC�ֶ�ת��,�ڶ�ʱ��������ת��
//		ADC_InitTypeDef ADC_InitStructure;

//	  GPIO_InitTypeDef GPIO_InitStructure;
//  /* ADC1 configuration ------------------------------------------------------*/
//		RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1| RCC_APB2Periph_GPIOC, ENABLE); 		
//	
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//  GPIO_Init(GPIOC, &GPIO_InitStructure);	
//	
//  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
////	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
//  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
//  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
//  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
//  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
//  ADC_InitStructure.ADC_NbrOfChannel = 1;
//  ADC_Init(ADC1, &ADC_InitStructure);

//// Enable ADC1 
//  ADC_Cmd(ADC1, ENABLE);

//  /* Enable ADC1 reset calibaration register */   
//  ADC_ResetCalibration(ADC1);
//  /* Check the end of ADC1 reset calibration register */
//  while(ADC_GetResetCalibrationStatus(ADC1));

//  /* Start ADC1 calibaration */
//  ADC_StartCalibration(ADC1);
//  /* Check the end of ADC1 calibration */
//  while(ADC_GetCalibrationStatus(ADC1));
//	ADC_SoftwareStartConvCmd(ADC1,ENABLE);	
	

	
	/*****************�Ĵ�����**********************/
	//�ȳ�ʼ��IO��
 	RCC->APB2ENR|=1<<4;    //ʹ��PORTC��ʱ�� 
	GPIOC->CRL&=0XFFFFFF0F;//PC1 anolog���� 
	RCC->APB2ENR|=1<<9;    //ADC1ʱ��ʹ��	  
	RCC->APB2RSTR|=1<<9;   //ADC1��λ
	RCC->APB2RSTR&=~(1<<9);//��λ����	    
	RCC->CFGR&=~(3<<14);   //��Ƶ��������	
	//SYSCLK/DIV2=12M ADCʱ������Ϊ12M,ADC���ʱ�Ӳ��ܳ���14M!
	//���򽫵���ADC׼ȷ���½�! 
	RCC->CFGR|=2<<14;      	 
	ADC1->CR1&=0XF0FFFF;   //����ģʽ����
	ADC1->CR1|=0<<16;      //��������ģʽ  
	ADC1->CR1&=~(1<<8);    //��ɨ��ģʽ	  
	ADC1->CR2&=~(1<<1);    //����ת��ģʽ
	ADC1->CR2&=~(7<<17);	   
	ADC1->CR2|=7<<17;	   //�������ת��  
	ADC1->CR2|=1<<20;      //ʹ�����ⲿ����(SWSTART)!!!	����ʹ��һ���¼�������
	ADC1->CR2&=~(1<<11);   //�Ҷ���	 
	
	ADC1->CR2|=1<<23;      //ʹ���¶ȴ�����
	
	ADC1->SQR1&=~(0XF<<20);
	ADC1->SQR1|=0<<20;     //1��ת���ڹ��������� Ҳ����ֻת����������1 			
	
	//����ͨ��11�Ĳ���ʱ��
	ADC1->SMPR1&=~(3*1);   //ͨ��11����ʱ�����	  
 	ADC1->SMPR1|=7<<(3*1); //ͨ��11  239.5����,��߲���ʱ�������߾�ȷ��	 
	
	ADC1->SMPR1&=~(7<<3*6);//���ͨ��16ԭ��������	 
	ADC1->SMPR1|=7<<(3*6); //ͨ��16  239.5����,��߲���ʱ�������߾�ȷ��	 
	
	ADC1->CR2|=1<<0;	   //����ADת����	 
	ADC1->CR2|=1<<3;       //ʹ�ܸ�λУ׼  
	while(ADC1->CR2&1<<3); //�ȴ�У׼���� 			 
    //��λ��������ò���Ӳ���������У׼�Ĵ�������ʼ�����λ��������� 		 
	ADC1->CR2|=1<<2;        //����ADУ׼	   
	while(ADC1->CR2&1<<2);  //�ȴ�У׼����
	//��λ����������Կ�ʼУ׼������У׼����ʱ��Ӳ�����  
	
}


void DAC1_Configuration(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    DAC_InitTypeDef DAC_InitType;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );      //??PORTA????
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                 // ????
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;          //?`???
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
     
	
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );      //??DAC????



    DAC_InitType.DAC_Trigger=DAC_Trigger_None;    //??????? TEN1=0
    DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//???????
    DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//???????
    DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;    //DAC1?????? BOFF1=1
    DAC_Init(DAC_Channel_1,&DAC_InitType);     //???DAC??1

    DAC_Cmd(DAC_Channel_1, ENABLE);  //??DAC1

    DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12??????????DAC?



}


//���ADC1ĳ��ͨ����ֵ
//ch:ͨ��ֵ 0~16
//����ֵ:ת�����
u16 Get_Adc(u8 ch)   
{
	//����ת������	  		 
	ADC1->SQR3&=0XFFFFFFE0;//��������1 ͨ��ch
	ADC1->SQR3|=ch;		  			    
	ADC1->CR2|=1<<22;       //��������ת��ͨ�� 
	while(!(ADC1->SR&1<<1));//�ȴ�ת������	 	   
	return ADC1->DR;		//����adcֵ	
}
//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 
//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��,��λ:��.)
short Get_Temprate(void)
{
	u32 adcx;
	short result;
 	double temperate;
	adcx=Get_Adc_Average(16,20);	//��ȡͨ��16,20��ȡƽ��
	temperate=(float)adcx*(3.3/4096);		//��ѹֵ 
	temperate=(1.43-temperate)/0.0043+25;	//ת��Ϊ�¶�ֵ 	 
	result=temperate*100;					//����100��.
	return result;
}

//vol:0~3300,??0~3.3V
void AO_test(u16 dac)
{
    DAC_SetChannel1Data(DAC_Align_12b_R,dac);//12??????????DAC?
		//DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
}


u16 AItmp,Temprate;
int flag=20;
void AIAO_test()
{

	while(1)
	{
		AItmp=Get_Adc(11);
		Temprate=Get_Temprate();
		if(flag-->0)
		{
			printf("%d\t%.2f��\n",AItmp,(float)Temprate/100);
		}
		//ADC_RegularChannelConfig(ADC1, ADC_Channel_11 , 1, ADC_SampleTime_28Cycles5 );		  			    
		//ADC_SoftwareStartConvCmd(ADC1, ENABLE);   // ת��һ��
		
		AO_test(AItmp);
		delay_ms(1000);	
	}
}
