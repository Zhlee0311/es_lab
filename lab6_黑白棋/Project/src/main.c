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
#include "serial.h"
#include "lcd.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_ICInitTypeDef TIM_ICInitStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;

/* Private function prototypes -----------------------------------------------*/
// void RCC_Configuration(void);
// extern u16 FrameFlag;

int main(void)
{
  // FrameFlag = 0;
  SystemInit(); // ��ʼ��ϵͳʱ��Ϊ72M

  /*
  RS232_Configuration(); // ��ʼ������
  NVIC_Configuration();  // ����NVIC�����ڴ����ж�
  */

  Serial_Init(); // ���ڳ�ʼ��

  // systick_init();      // ��ʼ��ϵͳ�δ�ʱ��
  LCD_Configuration(); // ������LCD��ص�����
  Touch_Init();        // �������ĳ�ʼ�����������š��жϵ�
  LCD_Init();          // ��ili9341оƬ����ָ��������÷ֱ��ʵȣ���ʼ����Ļ
  Touch_Check();       // ������ADֵУ׼
  while (1)
  {
    Game();
  }
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
