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
  SystemInit(); // 初始化系统时钟为72M

  /*
  RS232_Configuration(); // 初始化串口
  NVIC_Configuration();  // 配置NVIC，用于串口中断
  */

  Serial_Init(); // 串口初始化

  // systick_init();      // 初始化系统滴答定时器
  LCD_Configuration(); // 配置与LCD相关的引脚
  Touch_Init();        // 触摸屏的初始化，包括引脚、中断等
  LCD_Init();          // 向ili9341芯片发送指令，包括设置分辨率等，初始化屏幕
  Touch_Check();       // 触摸屏AD值校准
  while (1)
  {
    Game();
  }
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
