/************************************************************************************
//STM32����������ָTFTLCD��ۻ��߲��������ڲ����ӵ�STM32����
//=================================��Դ����=======================================//
//     LCDģ��                    STM32��������
//      3.3V          --->             DC3.3V          //��Դ
//      GND           --->             GND              //��Դ��
//=============================Һ���������߽���===================================//
//��ģ��Ĭ��������������Ϊ16λ��������(8λģʽ��ʹ��DB0~DB7)
//     LCDģ��                    STM32��������
//      D0            --->            PD14        -|   
//      D1            --->            PD15         |  
//      D2            --->            PD0          | 
//      D3            --->            PD1          | 
//      D4            --->            PE7          |
//      D5            --->            PE8          |
//      D6            --->            PE9          |
//      D7            --->            PE10         |===>Һ����16λ���������ź�
//      D8            --->            PE11         |
//      D9            --->            PE12         |
//      D10           --->            PE13         |
//      D11           --->            PE14         |
//      D12           --->            PE15         |
//      D13           --->            PD8          |
//      D14           --->            PD9          |
//      D15           --->            PD10        -|
//=============================Һ���������߽���===================================//
//     LCDģ�� 				            STM32�������� 
//      WR            --->            PD5             //Һ����д���ݿ����ź�
//      RD            --->            PD4             //Һ���������ݿ����ź�
//      RS            --->            PD11            //Һ��������/��������ź�
//      REST          --->          ��λ���ţ�Ĭ�ϣ�  //Һ������λ�����źţ�Ҳ��ѡ��PD13��
//      CS            --->            PD7             //Һ����Ƭѡ�����ź�
//      LED_A         --->            PB15            //Һ������������ź�
//===============================������������=====================================//
//���ģ�鲻���������ܻ��ߴ��д������ܣ����ǲ���Ҫ�������ܣ�����Ҫ���д���������
//	   LCDģ��                    STM32�������� 
//      T_IRQ         --->            PB1             //���ݻ���败���������ж��ź�
//      T_DO          --->            PB2             //���败����SPI���߶��ź�
//      T_DIN         --->            PC4             //���败����SPI����д�źŻ���ݴ�����IIC���������ź�
//      T_CS          --->            PC13            //���败����Ƭѡ�����źŻ���ݴ�������λ�ź�
//      T_CLK         --->            PB0             //���败����SPI���߻���ݴ�����IIC����ʱ���ź�
*************************************************************************************/			
#include "key.h"
#include "delay.h"

/*****************************************************************************
 * @name       :void KEY_Init(void)
 * @date       :2018-08-09 
 * @function   :Initialization key GPIO
 * @parameters :None
 * @retvalue   :None
******************************************************************************/ 
void KEY_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOA,GPIOEʱ��
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; //KEY0 KEY1 KEY2��Ӧ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
	
	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP��Ӧ����PA0
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA0
 
} 

/*****************************************************************************
 * @name       :u8 KEY_Scan(void)
 * @date       :2018-08-09 
 * @function   :Key processing,Response priority:KEY0>KEY1>WK_UP!!
 * @parameters :None
 * @retvalue   :0-No buttons were pressed.
								1-The KEY0 button is pressed
								2-The KEY1 button is pressed
								3-The KEY2 button is pressed
								4-The WK_UP button is pressed
******************************************************************************/ 
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY0==0||WK_UP==1))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY0==0)return 1;
//		else if(KEY1==0)return 2;
//		else if(KEY2==0)return 3;
		else if(WK_UP==1)return 4;
	}else if(KEY0==1&&WK_UP==0)key_up=1; 	    
 	return 0;// �ް�������
}