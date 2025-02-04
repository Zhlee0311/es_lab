/***********************************************************************
�ļ����ƣ�LED.C
��    �ܣ�led  IO��ʼ��
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺���´�������ο�
***********************************************************************/
#include "stm32f10x.h"

#define RCC_APB2ENR         (*((volatile unsigned int*)0x40021018))   // APB2 ����ʱ��ʹ�ܼĴ���
#define GPIOE_CRH           (*((volatile unsigned int*)0x40011804))   // �˿����ø߼Ĵ���
#define GPIOE_BSRR          (*((volatile unsigned int*)0x40011810))   // �˿�λ����/��λ�Ĵ���
#define GPIOE_IDR           (*((volatile unsigned int*)0x40011808))   // �˿��������ݼĴ���	
#define GPIOE_ODR           (*((volatile unsigned int*)0x4001180C))   // �˿�������ݼĴ���
	
#define GPIOB_CRL           (*((volatile unsigned int*)0x40010C00))   // �˿����õͼĴ���
#define GPIOB_IDR           (*((volatile unsigned int*)0x40010C08))   // �˿��������ݼĴ���	
#define GPIOB_BSRR          (*((volatile unsigned int*)0x40010C10))   // �˿�λ����/��λ�Ĵ���	

void LED_Init(void)
{
	RCC_APB2ENR |= 1<<6;          //ʹ��PORTEʱ��	
	GPIOE_CRH &=0XFFFFFFF0;       //���PE8����ԭ������  
	GPIOE_CRH |=0x3;			        //����CNF8[1:0]Ϊ0x00��ͨ���������ģʽ��MODE8[1:0]Ϊ0x11�����ģʽ
	GPIOE_BSRR = 1<<8;           //��8bit��1�������ö�Ӧ��ODR8λΪ1����PE8��������ߵ�ƽ  
}

void Delay(unsigned int nCount)
{ 
  while(nCount > 0)
  { 
  	  nCount --;   
  }
}

void LED_Turn(void)
{
	
	while(1)
	{

		GPIOE_BSRR = 1<<24;        //�����Ӧ��ODR8λΪ0����PE8��������͵�ƽ
//		GPIOE_BRR = 1<<8; 
		Delay(0xfffff);
		Delay(0xfffff);
	
	  GPIOE_BSRR = 1<<8; 
		
		Delay(0xfffff);
		Delay(0xfffff);
	}
}

void KEY_Init(void)
{
		RCC_APB2ENR |=1<<3;    //ʹ��PORTBʱ��	
	  GPIOB_CRL &=~(0xf<<24);// PB.6��������	
		GPIOB_CRL |=(0x08<<24);// PB.6��������	
		GPIOB_BSRR = (1<< 6);
}

u8 KEY_Scan(void)
{	 

	u8 key_1= 0;
	if(GPIOB_IDR & 0x40)
			key_1=0;
	else 
  		key_1=1;
	if(key_1==1)
	{
		delay_ms(100);//ȥ���� 
		if(GPIOB_IDR & 0x40)
			key_1=0;

			
		if(key_1==1)
			return 1;
	} 
 	return 0;// �ް�������
}
