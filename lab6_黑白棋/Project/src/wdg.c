#include "wdg.h"
#include "main.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������V3
//���Ź� ��������		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/10
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	  
  
//��ʼ���������Ź�
//prer:��Ƶ��:0~7(ֻ�е�3λ��Ч!)
//��Ƶ����=4*2^prer.�����ֵֻ����256!
//rlr:��װ�ؼĴ���ֵ:��11λ��Ч.
//ʱ�����(���):Tout=((4*2^prer)*rlr)/40 (ms).
void IWDG_Init(u8 prer,u16 rlr) 
{
	IWDG->KR=0X5555;//ʹ�ܶ�IWDG->PR��IWDG->RLR��д		 										  
  	IWDG->PR=prer;  //���÷�Ƶϵ��   
  	IWDG->RLR=rlr;  //�Ӽ��ؼĴ��� IWDG->RLR  
	IWDG->KR=0XAAAA;//reload											   
  	IWDG->KR=0XCCCC;//ʹ�ܿ��Ź�	
}
//ι�������Ź�
void IWDG_Feed(void)
{
	IWDG->KR=0XAAAA;//reload											   
}

void IWDG_test()
{
	LED_Configuration();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�  
	Count_KEY_Init();         	//��ʼ���밴�����ӵ�Ӳ���ӿ�
 	delay_ms(1000);			        //��ʱ500ms,���˿��Կ���DS0���״̬
 	IWDG_Init(4,625);    	//���Ƶ��Ϊ64,����ֵΪ625,���ʱ��Ϊ1s	   
	One_LED_ON(1);				 	//������
	while(1)
	{
		if(KEY_Scan()==1)    //���������������,��ι��
		{
			IWDG_Feed();
		}
		delay_ms(10);
	}		 
}



















