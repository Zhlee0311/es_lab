#include "main.h"

// ����NVIC����
// NVIC_Group:NVIC���� 0~4 �ܹ�5��
// void MY_NVIC_PriorityGroupConfig()
//{
//	/*�üĴ�����[10:8]3 λ���� PRIGROUP �Ķ���λ������ֵ�涨��ϵͳ��
//	�ж��ٸ����ȼ��жϺ������ȼ��жϡ��� STM32 ֻʹ�ø� 4 λ bits��  ����ܵ�ֵ���£���
//	�� ST �ĺ�����ͷ�ļ��еĶ��壩*/

//	//#define NVIC_PriorityGroup_0 ((uint32_t)0x700) /*!< 0 bits for pre-emption priority
//	//4 bits for subpriority */
//	//#define NVIC_PriorityGroup_1 ((uint32_t)0x600) /*!< 1 bits for pre-emption priority
//	//3 bits for subpriority */
//	//#define NVIC_PriorityGroup_2 ((uint32_t)0x500) /*!< 2 bits for pre-emption priority
//	//2 bits for subpriority */
//	//#define NVIC_PriorityGroup_3 ((uint32_t)0x400) /*!< 3 bits for pre-emption priority
//	//1 bits for subpriority */
//	//#define NVIC_PriorityGroup_4 ((uint32_t)0x300) /*!< 4 bits for pre-emption priority
//	//0 bits for subpriority */
//
//	//����NVIC����Ϊ2
//		SCB_AIRCR |=(0x05FA0000 | NVIC_PriorityGroup_2);  //Ϊ�˷�ֹ�������д������˵���д����Ĵ���������ʱ������Ҫͬʱ������Ĵ����ĸ� 16 λ[31��16]д��֤�֣�Register key�� 0x05FA
//}

// ����NVIC
// NVIC_PreemptionPriority:��ռ���ȼ�
// NVIC_SubPriority       :��Ӧ���ȼ�
// NVIC_Channel           :�жϱ��
// NVIC_Group             :�жϷ��� 0~4
// ע�����ȼ����ܳ����趨����ķ�Χ!����������벻���Ĵ���
// �黮��:
// ��0:0λ��ռ���ȼ�,4λ��Ӧ���ȼ�
// ��1:1λ��ռ���ȼ�,3λ��Ӧ���ȼ�
// ��2:2λ��ռ���ȼ�,2λ��Ӧ���ȼ�
// ��3:3λ��ռ���ȼ�,1λ��Ӧ���ȼ�
// ��4:4λ��ռ���ȼ�,0λ��Ӧ���ȼ�
// NVIC_SubPriority��NVIC_PreemptionPriority��ԭ����,��ֵԽС,Խ����
// void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)
//{
//	u32 temp;
//	MY_NVIC_PriorityGroupConfig();//���÷���
//	temp = NVIC_PreemptionPriority<<(4-NVIC_Group);
//	temp |= NVIC_SubPriority&(0x0f>>NVIC_Group);
//	temp&=0xf;								//ȡ����λ
//   *(NVIC_ISER+NVIC_Channel/32) |=(1<<NVIC_Channel%32);//ʹ���ж�λ(Ҫ����Ļ�,�෴������OK)
//	*(NVIC_IP+NVIC_Channel)|=temp<<4;		//������Ӧ���ȼ����������ȼ�
// }

// �ⲿ�ж����ú���
// ֻ���GPIOA~G;������PVD,RTC��USB����������
// ����:
// GPIOx:0~6,����GPIOA~G
// BITx:��Ҫʹ�ܵ�λ;
// TRIM:����ģʽ,1,������;2,�Ͻ���;3�������ƽ����
// �ú���һ��ֻ������1��IO��,���IO��,���ε���
// �ú������Զ�������Ӧ�ж�,�Լ�������
// void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM)
//{
//	u8 EXTADDR;
//	u8 EXTOFFSET;
//	EXTADDR=BITx/4;//�õ��жϼĴ�����ı��
//	EXTOFFSET=(BITx%4)*4;
//	RCC_APB2ENR|=0x01;//ʹ��io����ʱ��
//	*(AFIO_EXTICR+EXTADDR)&=~(0x000F<<EXTOFFSET);//���ԭ�����ã�����
//	*(AFIO_EXTICR+EXTADDR)|=GPIOx<<EXTOFFSET;//EXTI.BITxӳ�䵽GPIOx.BITx
//	//�Զ�����
//	EXTI_IMR|=1<<BITx;//  ����line BITx�ϵ��ж�
//	if(TRIM&0x01)EXTI_FTSR|=1<<BITx;//line BITx���¼��½��ش���
//	if(TRIM&0x02)EXTI_RTSR|=1<<BITx;//line BITx���¼������ش���
// }

// �ⲿ�жϳ�ʼ������
// ��ʼ��PB6�ж�����.
#include "stm32f10x_exti.h"

extern int keydown;
int temps = 0;

#ifdef _REG_MODE_
void EXTIX_Init(void)
{
	RCC_APB2ENR |= 0x01;		 // ʹ�ܸ���ioʱ�ӣ�AFIO
	AFIO_EXTICR2 &= ~(0xF << 8); // ���EXTI6[3:0]ԭ������
	AFIO_EXTICR2 |= 1 << 8;		 // ѡ��EXTIx�ⲿ�жϵ�����ԴΪPB[x]�� ��0000��PA[x]�� 0001��PB[x]�� 0010��PC[x]�� 0011��PD[x]�� 0100��PE[x]�ţ�
	EXTI_FTSR |= 1 << 6;		 // ����line BIT6���¼�Ϊ�½��ش���ģʽ
	EXTI_IMR |= 1 << 6;			 // ����line BIT6�ϵ��ж�

	// CM3 �п����� 240 ��ʹ��λ������λ��ÿ���ж�ӵ��һ�ԡ�
	// �� 240 �����ӷֲ��� 8 �� 32 λ�Ĵ����У����һ��û�����꣩��
	// ��ʹ��һ���жϣ���Ҫд1��NVIC_ISER�Ĵ�����Ķ�Ӧλ��
	*(NVIC_ISER + EXTI9_5_IRQn / 32) |= (1 << (EXTI9_5_IRQn % 32)); // ʹ�ܶ�Ӧ�ж�λ SETENA

	// SCB_AIRCR�Ĵ�����[10:8]3 λ���� PRIGROUP �Ķ���λ������ֵ�涨��ϵͳ��
	// �ж��ٸ����ȼ��жϺ������ȼ��жϡ��� STM32 ֻʹ�ø� 4 λ bits
	// ����NVIC����Ϊ2
	SCB_AIRCR = (0x05FA0000 | NVIC_PriorityGroup_2); // Ϊ�˷�ֹ�������д������˵���д����Ĵ���������ʱ������Ҫͬʱ������Ĵ����ĸ� 16 λ[31��16]д��֤�֣�Register key�� 0x05FA
	*(NVIC_IP + EXTI9_5_IRQn) |= 0xB << 4;			 // �����������ȼ�Ϊ2,��Ӧ���ȼ�Ϊ3 (0b1011)
}

void EXTI9_5_IRQHandler(void)
{
	if (EXTI_PR & (1 << 6)) // ����Ƿ�Ϊ�趨������line6�ж�
	{
		temps++;
		EXTI_PR = 1 << 6; // ���LINE6�ϵ��жϱ�־λ
	}
}
void LED_Configuration(void)
{
	RCC_APB2ENR |= 1 << 6;	 // ʹ��PORTEʱ��
	GPIOE_CRH &= 0XFFFFFFF0; // ���PE8����ԭ������
	GPIOE_CRH |= 3 << 0;	 // ����CNF8[1:0]Ϊ0x00��ͨ���������ģʽ��MODE8[1:0]Ϊ0x11�����ģʽ

	GPIOE_BSRR = 1 << 8; // ��8bit��1�������ö�Ӧ��ODR8λΪ1����PE8��������ߵ�ƽ
}
void Count_KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2ENR |= 1 << 3; // ʹ��PORTBʱ��
	// PB.6��������
	//	GPIOB_CRL |=0X04000000;
	// PB.6���ó�����ģʽ
	GPIOB_CRL &= 0XF0FFFFFF; //
	GPIOB_CRL |= 0X08000000; // ��������
	GPIOB_BSRR = (1 << 6);

	GPIOB->CRL &= 0xf0fffff;
	GPIOB->CRL |= 0X08000000; // ��������
	GPIOB->BSRR = (1 << 6);
}

#else
void EXTIX_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStruct; //
	EXTI_InitTypeDef EXTI_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		// ʹ�ܸ���ioʱ�ӣ�AFIO
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6); // ѡ��EXTIx�ⲿ�жϵ�����ԴΪPB[x]�� ��0000��PA[x]�� 0001��PB[x]�� 0010��PC[x]�� 0011��PD[x]�� 0100��PE[x]�ţ�

	EXTI_InitStruct.EXTI_Line = EXTI_Line6;				 // line BIT6
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;				 // ʹ��
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;	 // �ж�ģʽ
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; // �½����жϷ�ʽ
	EXTI_Init(&EXTI_InitStruct);						 // ����line BIT6���¼�Ϊ�½��ش���ģʽ

	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;		   // ͨ����
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;		   // ͨ���ж�ʹ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2; // �������ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;		   // �������ȼ�
	NVIC_Init(&NVIC_InitStruct);						   // ��ʼ��
}

void EXTI9_5_IRQHandler(void)
{

	if (EXTI_GetITStatus(EXTI_Line6))
	{
		EXTI_ClearITPendingBit(EXTI_Line6);
		temps++;
	}
}
void LED_Configuration(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  //????? ??????(??????)
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;		   //???????????10
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //????????
	GPIO_Init(GPIOE, &GPIO_InitStruct);
}

void Count_KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;			//????? ??????(??????)
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //???????????10
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		//????????
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

#endif

void EXTI_test()
{
	LED_Configuration(); // ָʾ�����ã���������
	Count_KEY_Init();	 // ��������Ϊ����
	EXTIX_Init();
	while (1)
	{
		// delay_ms(1000);
	}
}
