/**
  ******************************************************************************
  * @file InputCaptureMode/main.c 
  * @author   MCD Application Team
  * @version  V3.0.0
  * @date     04/27/2009
  * @brief    Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/** @addtogroup InputCaptureMode
  * @{
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
extern void LED_Init(void);
extern void LED_Turn(void);
extern void KEY_Init(void);
extern u8 KEY_Scan(void);

void BoardInit()
{
  /* System Clocks Configuration */
	SystemInit();
}

int main(void)
{
	u8 t=0;
	// ������д��ʼ�����룬�����ʼ���������ӵ�����Ϊ���룬led���ӵ�����Ϊ���
	//�����Ǻ�����Ҳ����ֱ��д���롣����ʹ�ú������÷�ʽ����߳�������Ŀɶ��ԡ�
	LED_Init();
	KEY_Init();
	
	while(1)
	{
			//������д���ƴ��룬����ѭ������led������ɨ���
			//�ع˵�2��ʱ���������������Ѳ�ط�ʽ��ǰ��̨��ʽ
		t= KEY_Scan();
		if (t == 1)
			LED_Turn();
	}
	return 1;
}



/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
