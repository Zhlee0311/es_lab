#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "lcd.h"
#include "font.h"
#include "stm32f10x.h"
#include "serial.h"

// LCD�Ļ�����ɫ�ͱ���ɫ
u16 POINT_COLOR = 0x0000; // ������ɫ
u16 BACK_COLOR = 0xeeee;  // ����ɫ

// ����LCD��Ҫ����
// Ĭ��Ϊ����
_lcd_dev lcddev;

void delay_us(int us)
{
	int i;
	while (us > 0)
	{
		for (i = us; i < 100; i++)
			;
		us--;
	}
}

void delay_ms(int ms)
{
	// int i, j;
	while (ms > 0)
	{

		delay_us(1000);
		ms--;
	}
}

void LCD_Configuration(void)
{

	//	GPIO_InitTypeDef GPIO_InitStructure;
	//	/* Enable the GPIO_LED Clock */
	//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO , ENABLE);
	////	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); //Disable jtag	,Enable SWD
	//
	//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 |GPIO_Pin_7 |GPIO_Pin_11|GPIO_Pin_12 ;
	//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//	GPIO_Init(GPIOD, &GPIO_InitStructure);

	//	GPIO_SetBits(GPIOD, GPIO_Pin_12);  //reset pin
	//
	//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
	//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//

	/*****************�Ĵ�����**********************/
	RCC->APB2ENR |= 7 << 4; // ʹ��PORTC,D,Eʱ��
	GPIOD->CRL &= 0X0F00FFFF;
	GPIOD->CRL |= 0X30330000;
	GPIOD->CRH &= 0XFFF00FFF;
	GPIOD->CRH |= 0X00033000; // PD4 5 7 11 12�������
	GPIOD->ODR |= 0X1000;	  // PD12�ø�

	GPIOC->CRL &= 0XFFFFFFF0;
	GPIOC->CRL |= 0X00000003; // PC0�������
}

void LCD_Config_DIN(void)
{
	/*GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = 0xC703;////GPIO_Pin_14 | GPIO_Pin_15 |GPIO_Pin_1|GPIO_Pin_0|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOD,0xC703);
	GPIO_InitStructure.GPIO_Pin = 0xff80 ; //PE7-15
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOE,0xff80);*/

	/*****************�Ĵ�����**********************/
	// GPIOD->CRL&=0XFFFFFF00;
	// GPIOD->CRL|=0X00000088;
	// GPIOD->CRH&=0X00FFF000;
	// GPIOD->CRH|=0X88000888;

	GPIOD->CRL = 0x34334488;
	GPIOD->CRH = 0x88433888;
	GPIOD->BRR = (uint16_t)0xC703;

	// GPIOE->CRL&=0X0FFFFFFF;
	// GPIOE->CRL|=0X80000000;
	// GPIOE->CRH&=0XFFFFFFFF;
	// GPIOE->CRH|=0X88888888;
	GPIOE->CRL = 0X84444444;
	GPIOE->CRH = 0X88888888;
	GPIOE->BRR = (uint16_t)0xff80;
}

void LCD_Config_DOUT(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = 0xC703; ////GPIO_Pin_14 | GPIO_Pin_15 |GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//
	GPIO_InitStructure.GPIO_Pin = 0xff80; // PE7-15
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	//
	/*GPIOD->CRL&=0XFFFFFF00;
	GPIOD->CRL|=0X00000033;
	GPIOD->CRH&=0X00FFF000;
	GPIOD->CRH|=0X33000333;

	GPIOE->CRL&=0XFFFFFFF0;
	GPIOE->CRL|=0X00000003;
	GPIOE->CRH&=0XFFFFFFFF;
	GPIOE->CRH|=0X33333333;*/

	// GPIOD->CRL=0x34334433;
	// GPIOD->CRH=0x33333333;
	// GPIOE->CRL=0X34444444;
	// GPIOE->CRH=0X33333333;
}

//
void DATAOUT(u16 x) // �������
{
	LCD_Config_DOUT();

	GPIOD->BRR = (0x03 << 14); // LCD_D0,D1; PD14,PD15
	GPIOD->BSRR = (x & 0x0003) << 14;

	GPIOD->BRR = (0x03 << 0); // LCD_D2,D3; PD0,PD1
	GPIOD->BSRR = (x & 0x000c) >> 2;

	GPIOE->BRR = (0x1ff << 7); // LCD_D4-12; PE7-15
	GPIOE->BSRR = (x & 0x1ff0) << 3;

	GPIOD->BRR = (0x07 << 8); // LCD_D13,D14,D15; PD8-10
	GPIOD->BSRR = (x & 0xE000) >> 5;
}

u16 DATAIN() // ��������
{
	u16 tmp = 0;
	LCD_Config_DIN();
	tmp = GPIOD->IDR >> 14;
	tmp |= ((GPIOD->IDR & 0x3) << 2);
	tmp |= ((GPIOE->IDR & 0xff80) >> 3);
	tmp |= ((GPIOD->IDR & 0x0700) << 5);
	return tmp;
}

// д�Ĵ�������
// data:�Ĵ���ֵ
void LCD_WR_REG(u16 data)
{
	LCD_RS_CLR; // д����
	LCD_CS_CLR;
	DATAOUT(data);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;
}
// д���ݺ���
// �������LCD_WR_DATA��,��ʱ�任�ռ�.
// data:�Ĵ���ֵ
void LCD_WR_DATAX(u16 data)
{
	LCD_RS_SET; // д����
	LCD_CS_CLR;
	DATAOUT(data);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;
}
// ��LCD����
// ����ֵ:������ֵ
u16 LCD_RD_DATA(void)
{
	u16 t;

	DATAIN();

	LCD_RS_SET;
	LCD_CS_CLR;
	// ��ȡ����(���Ĵ���ʱ,������Ҫ��2��)
	LCD_RD_CLR;

	t = DATAIN();
	LCD_RD_SET;
	LCD_CS_SET;

	DATAOUT(0xFFFF);
	return t;
}
// д�Ĵ���
// LCD_Reg:�Ĵ������
// LCD_RegValue:Ҫд���ֵ
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{
	LCD_WR_REG(LCD_Reg);
	LCD_WR_DATA(LCD_RegValue);
}
// ���Ĵ���
// LCD_Reg:�Ĵ������
// ����ֵ:������ֵ
u16 LCD_ReadReg(u16 LCD_Reg)
{
	LCD_WR_REG(LCD_Reg); // д��Ҫ���ļĴ�����
	return LCD_RD_DATA();
}
// ��ʼдGRAM
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}
// LCDдGRAM
// RGB_Code:��ɫֵ
void LCD_WriteRAM(u16 RGB_Code)
{
	LCD_WR_DATA(RGB_Code); // дʮ��λGRAM
}

// ��mdk -O1ʱ���Ż�ʱ��Ҫ����
// ��ʱi
void opt_delay(u8 i)
{
	while (i--)
		;
}
// ��ȡ��ĳ�����ɫֵ
// x,y:����
// ����ֵ:�˵����ɫ
u16 LCD_ReadPoint(u16 x, u16 y)
{
	u16 r, g, b;
	if (x >= lcddev.width || y >= lcddev.height)
		return 0; // �����˷�Χ,ֱ�ӷ���
	LCD_SetCursor(x, y);
	if (lcddev.id == 0X9341)
		LCD_WR_REG(0X2E); // 9341/6804/3510/1963 ���Ͷ�GRAMָ��
	DATAIN();			  //

	LCD_RS_SET;
	LCD_CS_CLR;
	// ��ȡ����(��GRAMʱ,��һ��Ϊ�ٶ�)
	LCD_RD_CLR;
	opt_delay(2); // ��ʱ
	r = DATAIN(); // ʵ��������ɫ
	LCD_RD_SET;

	// dummy READ
	LCD_RD_CLR;
	opt_delay(2); // ��ʱ
	r = DATAIN(); // ʵ��������ɫ
	LCD_RD_SET;
	if (lcddev.id == 0X9341) // 9341/NT35310/NT35510Ҫ��2�ζ���
	{
		LCD_RD_CLR;
		opt_delay(2); // ��ʱ
		b = DATAIN(); // ��ȡ��ɫֵ
		LCD_RD_SET;
		g = r & 0XFF; // ����9341,��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ
		g <<= 8;
	}
	LCD_CS_SET;
	DATAOUT(0xFFFF); // ȫ�������

	if (lcddev.id == 0X9341)
		return (((r >> 11) << 11) | ((g >> 10) << 5) | (b >> 11)); // ILI9341/NT35310/NT35510��Ҫ��ʽת��һ��
	else
		return 0x0000;
}
// LCD������ʾ
void LCD_DisplayOn(void)
{
	if (lcddev.id == 0X9341)
		LCD_WR_REG(0X29); // ������ʾ
}
// LCD�ر���ʾ
void LCD_DisplayOff(void)
{
	if (lcddev.id == 0X9341)
		LCD_WR_REG(0X28); // �ر���ʾ
}
// ���ù��λ��
// Xpos:������
// Ypos:������
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
	if (lcddev.id == 0X9341)
	{
		LCD_WR_REG(lcddev.setxcmd);
		LCD_WR_DATA(Xpos >> 8);
		LCD_WR_DATA(Xpos & 0XFF);
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(Ypos >> 8);
		LCD_WR_DATA(Ypos & 0XFF);
	}
}
// ����LCD���Զ�ɨ�跽��
// ע��:�����������ܻ��ܵ��˺������õ�Ӱ��(������9341/6804����������),
// ����,һ������ΪL2R_U2D����,�������Ϊ����ɨ�跽ʽ,���ܵ�����ʾ������.
// dir:0~7,����8������(���嶨���lcd.h)
// 9320/9325/9328/4531/4535/1505/b505/5408/9341/5310/5510/1963��IC�Ѿ�ʵ�ʲ���
void LCD_Scan_Dir(u8 dir)
{
	u16 regval = 0;
	u16 dirreg = 0;
	// u16 temp;

	if (lcddev.id == 0x9341) // 9341/6804/5310/5510/1963,���⴦��
	{
		switch (dir)
		{
		case L2R_U2D: // ������,���ϵ���
			regval |= (0 << 7) | (0 << 6) | (0 << 5);
			break;
		case L2R_D2U: // ������,���µ���
			regval |= (1 << 7) | (0 << 6) | (0 << 5);
			break;
		case R2L_U2D: // ���ҵ���,���ϵ���
			regval |= (0 << 7) | (1 << 6) | (0 << 5);
			break;
		case R2L_D2U: // ���ҵ���,���µ���
			regval |= (1 << 7) | (1 << 6) | (0 << 5);
			break;
		case U2D_L2R: // ���ϵ���,������
			regval |= (0 << 7) | (0 << 6) | (1 << 5);
			break;
		case U2D_R2L: // ���ϵ���,���ҵ���
			regval |= (0 << 7) | (1 << 6) | (1 << 5);
			break;
		case D2U_L2R: // ���µ���,������
			regval |= (1 << 7) | (0 << 6) | (1 << 5);
			break;
		case D2U_R2L: // ���µ���,���ҵ���
			regval |= (1 << 7) | (1 << 6) | (1 << 5);
			break;
		}
		dirreg = 0X36;
		LCD_WriteReg(dirreg, regval);
	}
}
// ����
// x,y:����
// POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(u16 x, u16 y)
{
	LCD_SetCursor(x, y);	// ���ù��λ��
	LCD_WriteRAM_Prepare(); // ��ʼд��GRAM
	LCD_WR_DATA(POINT_COLOR);
}
// ���ٻ���
// x,y:����
// color:��ɫ
void LCD_Fast_DrawPoint(u16 x, u16 y, u16 color) // ��ʹ������λ�á�дGRAM��������һ����ɫ������������LCD_DrawPointʱһ����
{
	if (lcddev.id == 0X9341)
	{
		LCD_WR_REG(lcddev.setxcmd);
		LCD_WR_DATA(x >> 8);
		LCD_WR_DATA(x & 0XFF);
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(y >> 8);
		LCD_WR_DATA(y & 0XFF);
	}
	LCD_RS_CLR;
	LCD_CS_CLR;
	DATAOUT(lcddev.wramcmd); // дָ��
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;
	LCD_WR_DATA(color); // д����
}
// SSD1963 ��������
// pwm:����ȼ�,0~100.Խ��Խ��.
void LCD_SSD_BackLightSet(u8 pwm)
{
	LCD_WR_REG(0xBE);		 // ����PWM���
	LCD_WR_DATA(0x05);		 // 1����PWMƵ��
	LCD_WR_DATA(pwm * 2.55); // 2����PWMռ�ձ�
	LCD_WR_DATA(0x01);		 // 3����C
	LCD_WR_DATA(0xFF);		 // 4����D
	LCD_WR_DATA(0x00);		 // 5����E
	LCD_WR_DATA(0x00);		 // 6����F
}
// ����LCD��ʾ����
// dir:0,������1,����
void LCD_Display_Dir(u8 dir)
{
	if (dir == 0) // ����
	{
		lcddev.dir = 0; // ����
		lcddev.width = 240;
		lcddev.height = 320;
		if (lcddev.id == 0X9341)
		{
			lcddev.wramcmd = 0X2C;
			lcddev.setxcmd = 0X2A;
			lcddev.setycmd = 0X2B;
		}
	}
	else // ����
	{
		lcddev.dir = 1; // ����
		lcddev.width = 320;
		lcddev.height = 240;
		if (lcddev.id == 0X9341)
		{
			lcddev.wramcmd = 0X2C;
			lcddev.setxcmd = 0X2A;
			lcddev.setycmd = 0X2B;
		}
	}
	LCD_Scan_Dir(DFT_SCAN_DIR); // Ĭ��ɨ�跽��
}
// ���ô���,���Զ����û������굽�������Ͻ�(sx,sy).
// sx,sy:������ʼ����(���Ͻ�)
// width,height:���ڿ�Ⱥ͸߶�,�������0!!
// �����С:width*height.
void LCD_Set_Window(u16 sx, u16 sy, u16 width, u16 height)
{
	// u8 hsareg, heareg, vsareg, veareg;
	// u16 hsaval, heaval, vsaval, veaval;
	u16 twidth, theight;
	twidth = sx + width - 1;
	theight = sy + height - 1;
	if (lcddev.id == 0X9341)
	{
		LCD_WR_REG(lcddev.setxcmd);
		LCD_WR_DATA(sx >> 8);
		LCD_WR_DATA(sx & 0XFF);
		LCD_WR_DATA(twidth >> 8);
		LCD_WR_DATA(twidth & 0XFF);
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(sy >> 8);
		LCD_WR_DATA(sy & 0XFF);
		LCD_WR_DATA(theight >> 8);
		LCD_WR_DATA(theight & 0XFF);
	}
}
// ��ʼ��lcd
// �ó�ʼ���������Գ�ʼ������ALIENTEK��Ʒ��LCDҺ����
// ������ռ�ýϴ�flash,�û����Ը����Լ���ʵ�����,ɾ��δ�õ���LCD��ʼ������.�Խ�ʡ�ռ�.
void LCD_Init(void)
{
	delay_ms(5); // delay 50 ms
	LCD_WriteReg(0x0000, 0x0001);
	delay_ms(5); // delay 50 ms
	lcddev.id = LCD_ReadReg(0x0000);

	// ����9341 ID�Ķ�ȡ
	LCD_WR_REG(0XD3);
	LCD_RD_DATA();			   // dummy read
	LCD_RD_DATA();			   // ����0X00
	lcddev.id = LCD_RD_DATA(); // ��ȡ93
	lcddev.id <<= 8;
	lcddev.id |= LCD_RD_DATA(); // ��ȡ41

	if (lcddev.id == 0X9341) // 9341��ʼ��
	{
		LCD_WR_REG(0xCF);
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0xC1);
		LCD_WR_DATAX(0X30);
		LCD_WR_REG(0xED);
		LCD_WR_DATAX(0x64);
		LCD_WR_DATAX(0x03);
		LCD_WR_DATAX(0X12);
		LCD_WR_DATAX(0X81);
		LCD_WR_REG(0xE8);
		LCD_WR_DATAX(0x85);
		LCD_WR_DATAX(0x10);
		LCD_WR_DATAX(0x7A);
		LCD_WR_REG(0xCB);
		LCD_WR_DATAX(0x39);
		LCD_WR_DATAX(0x2C);
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0x34);
		LCD_WR_DATAX(0x02);
		LCD_WR_REG(0xF7);
		LCD_WR_DATAX(0x20);
		LCD_WR_REG(0xEA);
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0x00);
		LCD_WR_REG(0xC0);	// Power control
		LCD_WR_DATAX(0x1B); // VRH[5:0]
		LCD_WR_REG(0xC1);	// Power control
		LCD_WR_DATAX(0x01); // SAP[2:0];BT[3:0]
		LCD_WR_REG(0xC5);	// VCM control
		LCD_WR_DATAX(0x30); // 3F
		LCD_WR_DATAX(0x30); // 3C
		LCD_WR_REG(0xC7);	// VCM control2
		LCD_WR_DATAX(0XB7);
		LCD_WR_REG(0x36); // Memory Access Control
		LCD_WR_DATAX(0x48);
		LCD_WR_REG(0x3A);
		LCD_WR_DATAX(0x55);
		LCD_WR_REG(0xB1);
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0x1A);
		LCD_WR_REG(0xB6); // Display Function Control
		LCD_WR_DATAX(0x0A);
		LCD_WR_DATAX(0xA2);
		LCD_WR_REG(0xF2); // 3Gamma Function Disable
		LCD_WR_DATAX(0x00);
		LCD_WR_REG(0x26); // Gamma curve selected
		LCD_WR_DATAX(0x01);
		LCD_WR_REG(0xE0); // Set Gamma
		LCD_WR_DATAX(0x0F);
		LCD_WR_DATAX(0x2A);
		LCD_WR_DATAX(0x28);
		LCD_WR_DATAX(0x08);
		LCD_WR_DATAX(0x0E);
		LCD_WR_DATAX(0x08);
		LCD_WR_DATAX(0x54);
		LCD_WR_DATAX(0XA9);
		LCD_WR_DATAX(0x43);
		LCD_WR_DATAX(0x0A);
		LCD_WR_DATAX(0x0F);
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0x00);
		LCD_WR_REG(0XE1); // Set Gamma
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0x15);
		LCD_WR_DATAX(0x17);
		LCD_WR_DATAX(0x07);
		LCD_WR_DATAX(0x11);
		LCD_WR_DATAX(0x06);
		LCD_WR_DATAX(0x2B);
		LCD_WR_DATAX(0x56);
		LCD_WR_DATAX(0x3C);
		LCD_WR_DATAX(0x05);
		LCD_WR_DATAX(0x10);
		LCD_WR_DATAX(0x0F);
		LCD_WR_DATAX(0x3F);
		LCD_WR_DATAX(0x3F);
		LCD_WR_DATAX(0x0F);
		LCD_WR_REG(0x2B);
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0x01);
		LCD_WR_DATAX(0x3f);
		LCD_WR_REG(0x2A);
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0xef);
		LCD_WR_REG(0x11); // Exit Sleep
		delay_ms(10);
		LCD_WR_REG(0x29); // display on
	}

	LCD_Display_Dir(0); // Ĭ��Ϊ����
	LCD_LED = 1;		// ��������
	LCD_Clear(BRRED);
}

// ��������
// color:Ҫ���������ɫ
void LCD_Clear(u16 color)
{
	u32 index = 0;
	u32 totalpoint = lcddev.width;
	totalpoint *= lcddev.height; // �õ��ܵ���
	LCD_SetCursor(0x00, 0x0000); // ���ù��λ��
	LCD_WriteRAM_Prepare();		 // ��ʼд��GRAM
	for (index = 0; index < totalpoint; index++)
		LCD_WR_DATA(color);
}
// ��ָ�����������ָ����ɫ
// �����С:(xend-xsta+1)*(yend-ysta+1)
// xsta
// color:Ҫ������ɫ
void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color)
{
	u16 i, j;
	u16 xlen = 0;
	// u16 temp;
	{
		xlen = ex - sx + 1;
		for (i = sy; i <= ey; i++)
		{
			LCD_SetCursor(sx, i);	// ���ù��λ��
			LCD_WriteRAM_Prepare(); // ��ʼд��GRAM
			for (j = 0; j < xlen; j++)
				LCD_WR_DATA(color); // ���ù��λ��
		}
	}
}
// ��ָ�����������ָ����ɫ��
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)
// color:Ҫ������ɫ
void LCD_Color_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 *color)
{
	u16 height, width;
	u16 i, j;
	width = ex - sx + 1;  // �õ����Ŀ��
	height = ey - sy + 1; // �߶�
	for (i = 0; i < height; i++)
	{
		LCD_SetCursor(sx, sy + i); // ���ù��λ��
		LCD_WriteRAM_Prepare();	   // ��ʼд��GRAM
		for (j = 0; j < width; j++)
			LCD_WR_DATA(color[i * width + j]); // д������
	}
}
// ����
// x1,y1:�������
// x2,y2:�յ�����
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	delta_x = x2 - x1; // ������������
	delta_y = y2 - y1;
	uRow = x1;
	uCol = y1;
	if (delta_x > 0)
		incx = 1; // ���õ�������
	else if (delta_x == 0)
		incx = 0; // ��ֱ��
	else
	{
		incx = -1;
		delta_x = -delta_x;
	}
	if (delta_y > 0)
		incy = 1;
	else if (delta_y == 0)
		incy = 0; // ˮƽ��
	else
	{
		incy = -1;
		delta_y = -delta_y;
	}
	if (delta_x > delta_y)
		distance = delta_x; // ѡȡ��������������
	else
		distance = delta_y;
	for (t = 0; t <= distance + 1; t++) // �������
	{
		LCD_DrawPoint(uRow, uCol); // ����
		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance)
		{
			xerr -= distance;
			uRow += incx;
		}
		if (yerr > distance)
		{
			yerr -= distance;
			uCol += incy;
		}
	}
}
// ������
//(x1,y1),(x2,y2):���εĶԽ�����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1, y1, x2, y1);
	LCD_DrawLine(x1, y1, x1, y2);
	LCD_DrawLine(x1, y2, x2, y2);
	LCD_DrawLine(x2, y1, x2, y2);
}
// ��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
// r    :�뾶
// color : Բ����ɫ�������߽���ڲ�
void LCD_Draw_Circle(u16 x0, u16 y0, u8 r, u16 color)
{
	int a, b;
	int di;
	a = 0;
	b = r;
	di = 3 - (r << 1); // �ж��¸���λ�õı�־
	while (a <= b)
	{
		LCD_Fast_DrawPoint(x0 + a, y0 - b, color); // 5
		LCD_Fast_DrawPoint(x0 + b, y0 - a, color); // 0
		LCD_Fast_DrawPoint(x0 + b, y0 + a, color); // 4
		LCD_Fast_DrawPoint(x0 + a, y0 + b, color); // 6
		LCD_Fast_DrawPoint(x0 - a, y0 + b, color); // 1
		LCD_Fast_DrawPoint(x0 - b, y0 + a, color);
		LCD_Fast_DrawPoint(x0 - a, y0 - b, color); // 2
		LCD_Fast_DrawPoint(x0 - b, y0 - a, color); // 7
		a++;
		// ʹ��Bresenham�㷨��Բ
		if (di < 0)
			di += 4 * a + 6;
		else
		{
			di += 10 + 4 * (a - b);
			b--;
		}
	}
	for (int y = y0 - r; y <= y0 + r; y++)
	{
		for (int x = x0 - r; x <= x0 + r; x++)
		{
			if (pow(x - x0, 2) + pow(y - y0, 2) <= pow(r, 2))
			{
				LCD_SetCursor(x, y);
				LCD_WriteRAM_Prepare();
				LCD_WriteRAM(color);
			}
		}
	}
}
// ��ָ��λ����ʾһ���ַ�
// x,y:��ʼ����
// num:Ҫ��ʾ���ַ�:" "--->"~"
// size:�����С 12/16/24
// mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(u16 x, u16 y, u8 num, u8 size, u8 mode)
{
	u8 temp, t1, t;
	u16 y0 = y;
	u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); // �õ�����һ���ַ���Ӧ������ռ���ֽ���
	num = num - ' ';										   // �õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	for (t = 0; t < csize; t++)
	{
		if (size == 12)
			temp = asc2_1206[num][t]; // ����1206����
		else if (size == 16)
			temp = asc2_1608[num][t]; // ����1608����
		else if (size == 24)
			temp = asc2_2412[num][t]; // ����2412����
		else
			return; // û�е��ֿ�
		for (t1 = 0; t1 < 8; t1++)
		{
			if (temp & 0x80)
				LCD_Fast_DrawPoint(x, y, POINT_COLOR);
			else if (mode == 0)
				LCD_Fast_DrawPoint(x, y, BACK_COLOR);
			temp <<= 1;
			y++;
			if (y >= lcddev.height)
				return; // ��������
			if ((y - y0) == size)
			{
				y = y0;
				x++;
				if (x >= lcddev.width)
					return; // ��������
				break;
			}
		}
	}
}
// m^n����
// ����ֵ:m^n�η�.
u32 LCD_Pow(u8 m, u8 n)
{
	u32 result = 1;
	while (n--)
		result *= m;
	return result;
}
// ��ʾ����,��λΪ0,����ʾ
// x,y :�������
// len :���ֵ�λ��
// size:�����С
// color:��ɫ
// num:��ֵ(0~4294967295);
void LCD_ShowNum(u16 x, u16 y, u32 num, u8 len, u8 size)
{
	u8 t, temp;
	u8 enshow = 0;
	for (t = 0; t < len; t++)
	{
		temp = (num / LCD_Pow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1))
		{
			if (temp == 0)
			{
				LCD_ShowChar(x + (size / 2) * t, y, ' ', size, 0);
				continue;
			}
			else
				enshow = 1;
		}
		LCD_ShowChar(x + (size / 2) * t, y, temp + '0', size, 0);
	}
}
// ��ʾ����,��λΪ0,������ʾ
// x,y:�������
// num:��ֵ(0~999999999);
// len:����(��Ҫ��ʾ��λ��)
// size:�����С
// mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
void LCD_ShowxNum(u16 x, u16 y, u32 num, u8 len, u8 size, u8 mode)
{
	u8 t, temp;
	u8 enshow = 0;
	for (t = 0; t < len; t++)
	{
		temp = (num / LCD_Pow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1))
		{
			if (temp == 0)
			{
				if (mode & 0X80)
					LCD_ShowChar(x + (size / 2) * t, y, '0', size, mode & 0X01);
				else
					LCD_ShowChar(x + (size / 2) * t, y, ' ', size, mode & 0X01);
				continue;
			}
			else
				enshow = 1;
		}
		LCD_ShowChar(x + (size / 2) * t, y, temp + '0', size, mode & 0X01);
	}
}
// ��ʾ�ַ���
// x,y:�������
// width,height:�����С
// size:�����С
//*p:�ַ�����ʼ��ַ
void LCD_ShowString(u16 x, u16 y, u16 width, u16 height, u8 size, u8 *p)
{
	u8 x0 = x;
	width += x;
	height += y;
	while ((*p <= '~') && (*p >= ' ')) // �ж��ǲ��ǷǷ��ַ�!
	{
		if (x >= width)
		{
			x = x0;
			y += size;
		}
		if (y >= height)
			break; // �˳�
		LCD_ShowChar(x, y, *p, size, 0);
		x += size / 2;
		p++;
	}
}

const unsigned char CN_DataBuf[][16] = {
	/*--  ��  --*/
	/*--  ??12;  ??????????:?x?=16x16   --*/
	0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0xFF, 0xFE, 0x01, 0x00, 0x01, 0x00,
	0x02, 0x80, 0x02, 0x80, 0x04, 0x40, 0x04, 0x40, 0x08, 0x20, 0x10, 0x10, 0x20, 0x08, 0xC0, 0x06,

	/*--  С  --*/
	/*--  ??12;  ??????????:?x?=16x16   --*/
	0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x11, 0x10, 0x11, 0x08, 0x11, 0x04,
	0x21, 0x04, 0x21, 0x02, 0x41, 0x02, 0x81, 0x02, 0x01, 0x00, 0x01, 0x00, 0x05, 0x00, 0x02, 0x00};

#define TOUCH_READ_TIMES 4 // ��������
u16 x_addata[TOUCH_READ_TIMES], z_addata[TOUCH_READ_TIMES], y_addata[TOUCH_READ_TIMES];
u32 Temp = 0;
//, r, sx, sy = 0;
int xScreenAD, yScreenAD; // x,y���������꣨ģ������
int xScreen, yScreen;	  // x,y��Ļ���꣨���أ�
u16 PointX_ADmin = 0;
u16 PointY_ADmin = 0;
u16 PointX_ADmax = 0;
u16 PointY_ADmax = 0;

#define QPINTERVAL 48
#define QZSIZE 20
#define QPSIZEX 240 // �ֱ���
#define QPSIZEY 320
#define QPSTARTX 25
#define QPSTARTY 40
#define MAX_X 5 // ���������
#define MAX_Y 5 // ���������
#define BGColor BRRED

ChessPiece board[MAX_X][MAX_Y] = {empty};
GameStatus gameStatus = init;
Player currentPlayer = whitePlayer; // ��ʼ��Ϊ����غ�
volatile int PressFlag = 0;			// ���������±�־
volatile int ContFlag = 0;			// ��Ϸ�Ƿ������־
volatile int ModeFlag = 0;			// ��Ϸģʽѡ���־
volatile int gridX = -1;			// ��ǰѡ�еĺ����꣨0-4��
volatile int gridY = -1;			// ��ǰѡ�е������꣨0-4��
volatile int lastX = -1;			// ��һ��ѡ�еĺ�����
volatile int lastY = -1;			// ��һ��ѡ�е�������
int blackCount = 5;					// ���Ӹ���
int whiteCount = 5;					// ���Ӹ���

#define T_CS_SET GPIOA->BSRR = 1 << 15 // Ƭѡ�˿�  		PA15
#define T_WR_SET GPIOB->BSRR = 1 << 5  // д����MOSI			PB5
#define T_SCK_SET GPIOA->BSRR = 1 << 5 // SCK			PA5

#define T_CS_CLR GPIOA->BRR = 1 << 15 // Ƭѡ�˿�  		PA15
#define T_WR_CLR GPIOB->BRR = 1 << 5  // д����	MOSI		PB5
#define T_SCK_CLR GPIOA->BRR = 1 << 5 // SCK			PA5

#define T_IN_STATUE ((GPIOA->IDR & 0X40) ? 1 : 0) // ����MISO PA6

/**
 * @brief ����ӵ��ϲ�������
 */
void FillUp(int x, int y)
{
	int startX = QPSTARTX + x * QPINTERVAL;
	int startY = QPSTARTY + y * QPINTERVAL;
	LCD_DrawLine(startX, startY, startX, startY - QZSIZE);
}

/**
 * @brief ����ӵ��²�������
 */
void FillDown(int x, int y)
{
	int startX = QPSTARTX + x * QPINTERVAL;
	int startY = QPSTARTY + y * QPINTERVAL;
	LCD_DrawLine(startX, startY, startX, startY + QZSIZE);
}

/**
 * @brief ����ӵ��Ҳ�������
 */
void FillRight(int x, int y)
{
	int startX = QPSTARTX + x * QPINTERVAL;
	int startY = QPSTARTY + y * QPINTERVAL;
	LCD_DrawLine(startX, startY, startX + QZSIZE, startY);
}

/**
 * @brief ����ӵ��󲿷�����
 */
void FillLeft(int x, int y)
{
	int startX = QPSTARTX + x * QPINTERVAL;
	int startY = QPSTARTY + y * QPINTERVAL;
	LCD_DrawLine(startX, startY, startX - QZSIZE, startY);
}

/**
 * @brief �������ӣ�ע������Ҫ�ָ��������������
 * @param x: ���ӵĺ�����
 * @param y: ���ӵ�������
 */
void ChessClear(int x, int y)
{
	int startX = QPSTARTX + x * QPINTERVAL;
	int startY = QPSTARTY + y * QPINTERVAL;
	LCD_Draw_Circle(startX, startY, QZSIZE, BGColor);
	if (y == 0)
	{
		FillDown(x, y);
		if (x == 0)
		{
			FillRight(x, y);
		}
		else if (x == 4)
		{
			FillLeft(x, y);
		}
		else
		{
			FillLeft(x, y);
			FillRight(x, y);
		}
	}
	else if (y == 1 || y == 2 || y == 3)
	{
		FillDown(x, y);
		FillUp(x, y);
		if (x == 0)
		{
			FillRight(x, y);
		}
		else if (x == 4)
		{
			FillLeft(x, y);
		}
		else
		{
			FillLeft(x, y);
			FillRight(x, y);
		}
	}
	else if (y == 4)
	{
		FillUp(x, y);
		if (x == 0)
		{
			FillRight(x, y);
		}
		else if (x == 4)
		{
			FillLeft(x, y);
		}
		else
		{
			FillLeft(x, y);
			FillRight(x, y);
		}
	}
}

/**
 * @brief ����һö����
 * @param x�������꣨0-4��
 * @param y�������꣨0-4��
 * @param chessKind: ������ɫ��ö��ֵ ��black/white)
 */
void ChessPut(int x, int y, ChessPiece chessKind)
{
	int startX = QPSTARTX + x * QPINTERVAL;
	int startY = QPSTARTY + y * QPINTERVAL;
	if (chessKind == black)
	{
		LCD_Draw_Circle(startX, startY, QZSIZE, BLACK);
	}
	else if (chessKind == white)
	{
		LCD_Draw_Circle(startX, startY, QZSIZE, WHITE);
	}
}

/**
 * @brief ��ʼ���ڷ�����
 */
void ChessInit()
{
	for (int i = 0; i < 5; i++)
	{
		ChessPut(0, i, white);
		ChessPut(4, i, black);
		board[0][i] = white;
		board[4][i] = black;
	}
}

/**
 * @brief ��������ܷ��ƶ�
 * @attention �����Ǽ���һ�������ϵ������ܷ��ƶ����ڶ�������
 * @attention �����kind�����������飬������������ĳ����ҵĻغ�
 */
bool IsMoveable(int x1, int y1, int x2, int y2, ChessPiece kind)
{
	if (x1 < 0 || x1 >= 5 || y1 < 0 || y1 >= 5 || x2 < 0 || x2 >= 5 || y2 < 0 || y2 >= 5)
	{
		return false;
	}
	if (board[x1][y1] == empty || board[x2][y2] != empty)
	{
		return false;
	}
	if (board[x1][y1] != kind)
	{
		return false;
	}
	if (abs(x1 - x2) + abs(y1 - y2) >= 2)
	{
		return false;
	}
	return true;
}

/**
 * @brief �����ҵ������ܷ��ƶ�
 */
bool IsPlayerMoveable(ChessPiece kind)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (board[i][j] == kind) // ��������Ǵ���������
			{
				for (int dx = -1; dx <= 1; dx++)
				{
					for (int dy = -1; dy <= 1; dy++)
					{
						if (!(dx == 0 && dy == 0) && IsMoveable(i, j, i + dx, j + dy, kind))
						{
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

/**
 * @brief ���ӷ�²�Ĵ�����
 */
void ChessCapture(int x, int y)
{
	int cntX = 0;
	int cntY = 0;
	int beginX = -1;
	int beginY = -1;
	bool flagX = false;
	bool flagY = false;
	for (int i = 0; i < 5; i++)
	{
		if (board[i][y] != empty)
		{
			cntX++;
			if (beginX == -1)
			{
				beginX = i;
			}
		}
		if (board[x][i] != empty)
		{
			cntY++;
			if (beginY == -1)
			{
				beginY = i;
			}
		}
	}
	// ����ĳһ����3������ʱ�Ž��д���
	if (cntX == 3)
	{
		// ������������Ƿ��������������������账��
		for (int i = beginX; i + 2 < 5; i++)
		{
			if (board[i][y] != empty && board[i + 1][y] != empty && board[i + 2][y] != empty)
			{
				flagX = true;
				break;
			}
		}
		// ������������������£����´���
		if (flagX)
		{
			int Me = 0;
			int Enemy = 0;
			ChessPiece Current = board[x][y];
			for (int i = 0; i < 3; i++)
			{
				if (board[i + beginX][y] == Current)
				{
					Me++;
				}
				else
				{
					Enemy++;
				}
			}
			if (Me == 2 && Enemy == 1)
			{
				for (int i = 0; i < 3; i++)
				{
					if (board[i + beginX][y] != Current)
					{
						ChessClear(i + beginX, y);
						if (Current == white)
						{
							blackCount--; // �ƶ����ǰ��ӣ���з��Ǻ���
						}
						else if (Current == black)
						{
							whiteCount--;
						}
						board[i + beginX][y] = empty;
						break;
					}
				}
			}
		}
	}
	if (cntY == 3)
	{
		for (int i = beginY; i + 2 < 5; i++)
		{
			if (board[x][i] != empty && board[x][i + 1] != empty && board[x][i + 2] != empty)
			{
				flagY = true;
				break;
			}
		}
		if (flagY)
		{
			int Me = 0;
			int Enemy = 0;
			ChessPiece Current = board[x][y];
			for (int i = 0; i < 3; i++)
			{
				if (board[x][i + beginY] == Current)
				{
					Me++;
				}
				else
				{
					Enemy++;
				}
			}
			if (Me == 2 && Enemy == 1)
			{
				for (int i = 0; i < 3; i++)
				{
					if (board[x][i + beginY] != Current)
					{
						ChessClear(x, i + beginY);
						if (Current == white)
						{
							blackCount--;
						}
						else if (Current == black)
						{
							whiteCount--;
						}
						board[x][i + beginY] = empty;
						break;
					}
				}
			}
		}
	}
}

/**
 * @brief ��Ϸ�������ж�
 */
bool GameOver()
{
	// �׷�ʤ
	if (blackCount <= 1)
	{
		gameStatus = whiteWin;
		return true;
	}
	// �ڷ�ʤ
	if (whiteCount <= 1)
	{
		gameStatus = blackWin;
		return true;
	}

	// ���˫���Ƿ��п��ƶ�����
	bool blackCanMove = IsPlayerMoveable(black);
	bool whiteCanMove = IsPlayerMoveable(white);

	// ƽ��
	if (!blackCanMove && !whiteCanMove)
	{
		gameStatus = draw;
		return true;
	}

	// �׷�ʤ
	if (!blackCanMove)
	{
		gameStatus = whiteWin;
		return true;
	}

	// �ڷ�ʤ
	if (!whiteCanMove)
	{
		gameStatus = blackWin;
		return true;
	}

	return false;
}

/**
 * @brief �ƶ�����
 */
void ChessMove(int x1, int y1, int x2, int y2)
{
	if (IsMoveable(x1, y1, x2, y2, (currentPlayer == whitePlayer ? white : black)))
	{
		ChessClear(x1, y1);
		ChessPut(x2, y2, currentPlayer == whitePlayer ? white : black);
		board[x2][y2] = board[x1][y1];
		board[x1][y1] = empty;
		ChessCapture(x2, y2);
		if (GameOver())
		{
			return;
		}
		currentPlayer = (currentPlayer == whitePlayer ? blackPlayer : whitePlayer);
	}
}

/**
 * @brief ��ʼ������
 */
void BoardInit()
{
	int i, j;
	int boardWidth = 192;  // ���̿�ȣ��̶�Ϊ 192 ����
	int boardHeight = 192; // ���̸߶ȣ��̶�Ϊ 192 ����
	// �����̵ĺ��� (�����ϱ߽���±߽�)
	for (i = 0; i < MAX_X; i++) // ��5�����ߣ������߽磩
	{
		// ȷ�����߲��ᳬ����Ļ��Χ
		if (QPSTARTY + i * QPINTERVAL <= QPSTARTY + boardHeight)
		{
			LCD_DrawLine(QPSTARTX, QPSTARTY + i * QPINTERVAL, QPSTARTX + boardWidth, QPSTARTY + i * QPINTERVAL);
		}
	}

	// �����̵����� (������߽���ұ߽�)
	for (j = 0; j < MAX_Y; j++) // ��5�����ߣ������߽磩
	{
		// ȷ�����߲��ᳬ����Ļ��Χ
		if (QPSTARTX + j * QPINTERVAL <= QPSTARTX + boardWidth)
		{
			LCD_DrawLine(QPSTARTX + j * QPINTERVAL, QPSTARTY, QPSTARTX + j * QPINTERVAL, QPSTARTY + boardHeight);
		}
	}
}

/**
 * @brief ��ʾ��ǰΪ��һ���Ļغ�/��Ӯ״̬
 */
void ShowStatus()
{
	unsigned char *info = (unsigned char *)malloc(sizeof(unsigned char) * 30); // �����ڴ�
	if (gameStatus == playing)
	{
		if (currentPlayer == whitePlayer)
		{
			snprintf((char *)info, 30, "White's Turn"); // ʹ�� snprintf ����д�볤��
		}
		else
		{
			snprintf((char *)info, 30, "Black's Turn");
		}
	}
	else if (gameStatus == whiteWin)
	{
		snprintf((char *)info, 30, "White Wins!!!");
	}
	else if (gameStatus == blackWin)
	{
		snprintf((char *)info, 30, "Black Wins!!!");
	}
	else if (gameStatus == draw)
	{
		snprintf((char *)info, 30, "You guys Draw!!!");
	}
	else
	{
		snprintf((char *)info, 30, "Unknown Status");
	}

	// ��ʾ�ַ���
	LCD_ShowString(85, 264, 240, 18, 12, info);
	free(info);
	/*
	if (gameStatus == draw)
	{
		sprintf(info, "draw");
	}
	else if (gameStatus == blackWin)
	{
		sprintf(info, "blackWin");
	}
	else if (gameStatus == playing)
	{
		sprintf(info, "playing");
	}
	else
	{
		sprintf(info, "whiteWin");
	}
	LCD_ShowString(80, 270, 240, 12, 12, info);
	*/
}

/**
 * @brief ������ʼ��
 */
void VarInit()
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			board[i][j] = empty;
		}
	}
	gameStatus = playing;
	currentPlayer = whitePlayer;
	PressFlag = 0;
	ContFlag = 0;
	gridX = -1;
	gridY = -1;
	lastX = -1;
	lastY = -1;
	blackCount = 5;
	whiteCount = 5;
}

/**
 * @brief ѡ����Ϸģʽ
 */
void GameMode()
{
	LCD_Clear(BGColor);
	unsigned char *GameName = (unsigned char *)malloc(sizeof(unsigned char) * 30); // �����ڴ�
	unsigned char *Mode_1 = (unsigned char *)malloc(sizeof(unsigned char) * 30);   // �����ڴ�
	unsigned char *Mode_2 = (unsigned char *)malloc(sizeof(unsigned char) * 30);   // �����ڴ�
	snprintf((char *)GameName, 30, "Black VS White");
	snprintf((char *)Mode_1, 30, "Player VS Player");
	snprintf((char *)Mode_2, 30, "Player VS AI");
	while (!ModeFlag)
	{
		LCD_ShowString(40, 60, 200, 18, 24, GameName);
		LCD_DrawRectangle(50, 120, 200, 160);
		LCD_DrawRectangle(50, 200, 200, 240);
		LCD_ShowString(60, 130, 200, 18, 16, Mode_1);
		LCD_ShowString(75, 210, 200, 18, 16, Mode_2);
	}
	free(GameName);
	free(Mode_1);
	free(Mode_2);
}

/**
 * @brief ��Ϸ��ʼ��
 */
void GameInit()
{
	GameMode(); // ѡ��ģʽ��ע�⿪ʼҪ����Ϸ״̬��Ϊinit����������Ϸ״̬����init
	LCD_Clear(BGColor);
	VarInit(); // ���������Ϸ״̬��Ϊplaying
	BoardInit();
	ChessInit();
}

/**
 * @brief ��Ϸ������Ĵ���
 */
void GameCont()
{
	while (!ContFlag)
	{
		ShowStatus();
		LCD_ShowString(60, 290, 240, 18, 12, "Click Here, Try Again");
	}
	ContFlag = 0;	   // ����ѭ����ѱ�־��Ϊ0�����´�ʹ��
	ModeFlag = 0;	   // ��һ����Ҫ����ģʽ�������ģʽ��־��0
	gameStatus = init; // ��Ϸ״̬����Ϊ��ʼ��
	GameInit();
}

/**
 * @brief ��Ϸ��������һ��ѭ��
 */
void Game()
{
	GameInit();
	while (gameStatus == playing)
	{
		ShowStatus();
		/*
		if (PressFlag && gameStatus == playing)
		{
			PressFlag = 0;
			ChessMove(lastX, lastY, gridX, gridY);
		}
		if (gameStatus != playing)
		{
			GameCont();
		}
		*/
		if (ModeFlag == 1)
		{
			if (PressFlag && gameStatus == playing)
			{
				PressFlag = 0;
				ChessMove(lastX, lastY, gridX, gridY);
			}
		}
		else if (ModeFlag == 2)
		{
			if (PressFlag && gameStatus == playing && currentPlayer == whitePlayer)
			{
				PressFlag = 0;
				ChessMove(lastX, lastY, gridX, gridY);
				// �����ƶ��ɹ��ŷ���ս��
				if (currentPlayer == blackPlayer)
				{
					Serial_Send();
				}
			}
			else if (gameStatus == playing && currentPlayer == blackPlayer)
			{
				if (Serial_GetRxFlag())
				{
					uint16_t data = Serial_GetRxData();
					int x1 = data >> 12 & 0x000F;
					int y1 = data >> 8 & 0x0000F;
					int x2 = data >> 4 & 0x0000F;
					int y2 = data & 0x000F;
					ChessMove(x1, y1, x2, y2);
				}
			}
		}
		if (gameStatus != playing)
		{
			GameCont();
		}
	}
}

// spi��д����(ģ��)
u8 SPI_SndRecv(u8 data)
{
	u8 tmp = 0, i;

	delay_us(1);
	for (i = 0; i < 8; i++)
	{
		T_SCK_CLR;

		if (data & 0x80)
			T_WR_SET;
		else
			T_WR_CLR;
		data = data << 1;
		delay_us(1);

		T_SCK_SET;
		tmp = tmp << 1;
		tmp |= T_IN_STATUE;
		delay_us(1);
	}

	return tmp;
} // data�Ƿ��͹�ȥ�����ݣ�tmp�ǽ��յ�������

u32 SPI_X(void)
{
	u16 i;
	for (i = 0; i < TOUCH_READ_TIMES; i++) // ����4��.
	{
		T_CS_CLR;		   // cs
		SPI_SndRecv(0xD0); // ����ָ��

		x_addata[i] = SPI_SndRecv(0x00);
		x_addata[i] <<= 8; // ƴ��2���ֽڵ�����(x����)

		x_addata[i] |= SPI_SndRecv(0x00);
		x_addata[i] >>= 3;

		T_CS_SET;
	}

	Temp = (x_addata[2] + x_addata[3]) >> 1;
	return Temp;

	/*
	r = Temp - 200;
	r *= 240;
	sx = r / (4000 - 200);
	if (sx <= 0 || sx > 240)
		return 0;
	return sx;
	*/
}

u32 SPI_Y(void)
{
	u16 i;
	for (i = 0; i < TOUCH_READ_TIMES; i++)
	{			  // ����4��.
		T_CS_CLR; // cs
		SPI_SndRecv(0x90);
		y_addata[i] = SPI_SndRecv(0x00);
		y_addata[i] <<= 8;
		y_addata[i] |= SPI_SndRecv(0x00);
		y_addata[i] >>= 3;
		T_CS_SET;
	}

	Temp = (y_addata[2] + y_addata[3]) >> 1;
	return Temp;

	/*
	r = Temp - 190;
	r *= 320;
	sy = r / (4000 - 190);
	if (sy <= 0 || sy > 320)
		return 0;
	return sy;
	*/
}

// ����Ϊ�жϵ��������
void TOUCH_SCREEN_INIT()
{

	GPIO_InitTypeDef GPIO_InitStructure;
	// SPI_InitTypeDef SPI_InitStructure;
	/* Enable GPIOB, GPIOC and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); // Disable jtag	,Enable SWD

	/* SPI pins configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure); // PA15�Ǵ�������Ƭѡ,PA5������SPIͨ�ŵ�SCK

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitStructure); // MOSI��PB5

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure); // MISO��PA6

	/*
		 SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//SPI??????????
		 SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//????SPI
		 SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//??SPI?????:SPI????8????
		 SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//??????????:?????
		 SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//???????????
		 SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//(SPI_NSS_Soft)??NSS?????????GPIO??????
		 SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//Fclk/2
		 SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; // Initialize the SPI_FirstBit member
		 SPI_InitStructure.SPI_CRCPolynomial=7;
		 SPI_Init(SPI1, &SPI_InitStructure);
		 SPI_Cmd(SPI1, ENABLE);
	*/
}
static void TOUCH_INT_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Enable GPIOB, GPIOC and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	/* LEDs pins configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
static void TOUCH_INT_EXIT_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // AFIO??
	/* Connect Button EXTI Line to Button GPIO Pin */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);
	/* Configure Button EXTI line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_ClearITPendingBit(EXTI_Line2);
	EXTI_Init(&EXTI_InitStructure);
}
static void TOUCH_InterruptConfig(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Set the Vector Table base address at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
	/* Configure the Priority Group to 2 bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*��ʱ�ò�������ע�͵�
// ���ݲɼ���������ֵ��Ѱ������������
static void getPos(u16 xScreen, u16 yScreen)
{
	u16 x = xScreen, y = yScreen;
	int i, j;
	int dist = 0;
	for (i = QPSTARTX + QPINTERVAL / 2, dist = 9999; i <= QPSTARTX + QPSIZEX; i += QPINTERVAL)
	{
		if (abs(i - x) <= dist)
		{
			xScreen = i;
			dist = abs(i - x);
		}
	}
	for (j = QPSTARTY + QPINTERVAL / 2, dist = 9999; j <= QPSTARTY + QPSIZEY; j += QPINTERVAL)
	{
		if (abs(j - y) <= dist)
		{
			yScreen = j;
			dist = abs(j - y);
		}
	}
	//		play(xScreen/QPINTERVAL, yScreen/QPINTERVAL);
}
*/

/**
 * @brief ���ü�����־
 */
void ContSet(int digX, int digY)
{
	if (digX >= 40 && digX <= 160 && digY >= 265 && digY <= 315)
	{
		ContFlag = 1;
	}
}

/**
 * @brief ����ģʽ��־
 */
void ModeSet(int digX, int digY)
{
	if (digX >= 50 && digX <= 200 && digY >= 120 && digY <= 160)
	{
		ModeFlag = 1; // ģʽ1����Ҷ�ս���
	}
	else if (digX >= 50 && digX <= 200 && digY >= 200 && digY <= 240)
	{
		ModeFlag = 2; // ģʽ2����Ҷ�սAI
	}
}

void TouchScreen()
{
	// ��ȡ�����ģ����
	xScreenAD = SPI_X();
	yScreenAD = SPI_Y();

	// ��Ч���ݲ�����������
	if (xScreenAD < 50 || yScreenAD < 50 || xScreenAD > 4050 || yScreenAD > 4050)
	{
		return;
	}

	// ӳ��ģ����Ϊ�����������أ����ڱ߽��Ѿ�У׼������²Ž��м���
	if (PointX_ADmax && PointX_ADmin && PointY_ADmax && PointY_ADmin)
	{
		xScreen = (double)(xScreenAD - PointX_ADmin) * 180 / (PointX_ADmax - PointX_ADmin) + 30;
		yScreen = (double)(yScreenAD - PointY_ADmin) * 260 / (PointY_ADmax - PointY_ADmin) + 30;

		// ����Ϸ�ǽ���״̬�ҷǳ�ʼ��״̬ʱ�����ü�����־λ
		if (gameStatus != playing && gameStatus != init)
		{
			ContSet(xScreen, yScreen);
		}

		// ���ҽ�����ϷΪ��ʼ��״̬ʱ������ģʽ��־λ
		if (gameStatus == init)
		{
			ModeSet(xScreen, yScreen);
		}

		// ���㵱ǰ��������������
		int newGridX = round((xScreen - QPSTARTX) * 1.0 / QPINTERVAL);
		int newGridY = round((yScreen - QPSTARTY) * 1.0 / QPINTERVAL);

		// �������λ����Ч���ȱ��浱ǰ����Ϊ��һ�����꣬Ȼ����µ�ǰ����
		if (newGridX >= 0 && newGridX < MAX_X && newGridY >= 0 && newGridY < MAX_Y)
		{
			// ������һ�ε�����
			lastX = gridX;
			lastY = gridY;
			// ���µ�ǰ����
			gridX = newGridX;
			gridY = newGridY;
		}
	}
	/*��ʱ�ò�������ע�͵�
	if ((xScreen > 1) && (yScreen > 1) && (xScreen < 240 - 1) && (yScreen < 320 - 1))
	{
		if (!(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2)))
		{
			if ((xScreen < QPSIZEX / QPINTERVAL * QPINTERVAL - 1) && (yScreen <= QPSIZEY / QPINTERVAL * QPINTERVAL - 1))
				getPos(xScreen, yScreen);
		}
	}
	*/
}

void EXTI2_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line2) != RESET)
	{
		delay_ms(20); // ����
		TouchScreen();
		PressFlag = 1;
		EXTI_ClearITPendingBit(EXTI_Line2);
	}
} // �жϷ�����

void Touch_Init()
{
	// ���ô�����������
	TOUCH_SCREEN_INIT();
	// ���ô��������ж�
	TOUCH_INT_config();
	TOUCH_INT_EXIT_Init();
	TOUCH_InterruptConfig();
	//	while(1);
}

// ��ʾ�ɼ����ĵ�λ����
unsigned char showstr[32];

// У׼����������ȡx�����AD���ֵ����Сֵ��y�����AD���ֵ����Сֵ
void Touch_Check()
{
	int status = 0;
	while (1)
	{
		LCD_ShowString(45, 45, 245, 325, 12, "1");

		LCD_ShowString(205, 285, 245, 325, 12, "2");

		LCD_DrawLine(0, 40, 240, 40);
		LCD_DrawLine(0, 280, 240, 280); // �������x=(40 , 200),y=(40,280)
		LCD_ShowString(40 - 2, 40 - 5, 240, 12, 12, "+");
		LCD_DrawLine(40, 0, 40, 320);
		LCD_DrawLine(200, 0, 200, 320);

		// У׼��1
		while (status == 0)
		{
			LCD_ShowString(45, 0, 240, 12, 12, "Press No.1 Point");
			if (PressFlag) // �Ѿ���������
			{
				if (xScreenAD >= 50 && xScreenAD <= 4050 && yScreenAD >= 50 && yScreenAD <= 4050)
				{
					PointX_ADmin = xScreenAD;
					PointY_ADmin = yScreenAD;
					snprintf((char *)showstr, sizeof(showstr), "TouchAD, x:%4d y:%4d   ", xScreenAD, yScreenAD);
					LCD_ShowString(45, 12, 240, 12, 12, showstr);
					status = 1;
					PressFlag = 0;
				}
				else
				{
					snprintf((char *)showstr, sizeof(showstr), "Invalid Position");
					LCD_ShowString(45, 12, 240, 12, 12, showstr);
				}
			}
		}
		// У׼��2
		while (status == 1)
		{
			LCD_ShowString(45, 0, 240, 12, 12, "Press No.2 Point");
			if (PressFlag) // �Ѿ���������
			{
				if (xScreenAD >= 50 && xScreenAD <= 4050 && yScreenAD >= 50 && yScreenAD <= 4050)
				{
					PointX_ADmax = xScreenAD;
					PointY_ADmax = yScreenAD;
					snprintf((char *)showstr, sizeof(showstr), "TouchAD, x:%4d y%4d   ", xScreenAD, yScreenAD);
					LCD_ShowString(45, 24, 240, 12, 12, showstr);
					status = 2;
					PressFlag = 0;
				}
				else
				{
					snprintf((char *)showstr, sizeof(showstr), "Invalid Position");
					LCD_ShowString(45, 24, 240, 12, 12, showstr);
				}
			}
		}
		// ���������У���߼�
		if (PointX_ADmax > PointX_ADmin && PointY_ADmax > PointY_ADmin)
		{

			snprintf((char *)showstr, sizeof(showstr), "Calibration Success");
			LCD_ShowString(45, 0, 240, 12, 12, showstr);
			delay_ms(1500);
			break;
		}
		else
		{
			snprintf((char *)showstr, sizeof(showstr), "Calibration Failed");
			LCD_ShowString(45, 0, 240, 12, 12, showstr);
			status = 0;
			delay_ms(1500);
		}
	}
}

/* ��ʱ�ò�������ע�͵�
void showTouch()
{
	sprintf(showstr, "Press at:%4d y%4d   ", xScreen, yScreen);
	LCD_ShowString(45, 0, 240, 12, 12, showstr);
	LCD_ShowString(xScreen - 2, yScreen - 5, 240, 12, 12, "+"); //+���ĵ�������ƫ��Ϊ2��5
}
*/
