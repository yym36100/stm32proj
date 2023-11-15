/************************************************************************************
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
#include "GT911.h"
#include "ctpiic.h"
#include "delay.h"
#include "touch.h"
#include "lcd.h"
#include "exti.h"

extern u8 touch_flag;

/*****************************************************************************
 * @name       :void GT911_int_sync(u16 ms)
 * @date       :2019-06-20 
 * @function   :GT911 initialization synchronization
 * @parameters :ms:Synchronization delay
 * @retvalue   :None
******************************************************************************/
void GT911_int_sync(u16 ms)
{
	INT_CTRL=0;
	delay_ms(ms);
	INT_IN();
}	

/*****************************************************************************
 * @name       :void GT911_reset_guitar(u8 addr)
 * @date       :2019-06-20 
 * @function   :GT911 reset
 * @parameters :add:IIC bus address
 * @retvalue   :None
******************************************************************************/
void GT911_reset_guitar(u8 addr)
{
	INT_CTRL=1;
	RST_CTRL=1;
	delay_ms(20);
	RST_CTRL=0;
	INT_CTRL=0;
	delay_ms(20);
	INT_CTRL=(addr == 0x28);
	delay_ms(20);
	RST_CTRL=1;
	delay_ms(20);
}

/*****************************************************************************
 * @name       :void GT911_reset(void)
 * @date       :2019-06-20 
 * @function   :GT911 reset
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void GT911_reset(void)
{
	RST_CTRL=0;
	delay_ms(10);
	RST_CTRL=1;
	delay_ms(10);
}

/*****************************************************************************
 * @name       :void GT911_gpio_init(void)
 * @date       :2019-06-20 
 * @function   :GT911 IC gpio initialization
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void GT911_gpio_init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC,ENABLE);//ʹ��PORTB,Cʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //PB1 ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����	 
	GPIO_Init(GPIOB, &GPIO_InitStructure);				  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //PF11 �������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		  
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 
#if SCAN_TYPE
	Touch_EXTI_Init();
#endif		
	CTP_IIC_Init();
	RST_OUT();
	INT_OUT();
	GT911_reset();
}

/*****************************************************************************
 * @name       :u8 GT9XX_WriteHandle (u16 addr)
 * @date       :2019-06-20 
 * @function   :GT911 IC write handle
 * @parameters :addr:register address
 * @retvalue   :the status of writen
******************************************************************************/
u8 GT9XX_WriteHandle (u16 addr)
{
	u8 status;

	CTP_IIC_Start();
	CTP_IIC_Send_Byte(GT9XX_IIC_WADDR); //д����ָ��
	CTP_IIC_Wait_Ack(); 
	CTP_IIC_Send_Byte((u8)(addr >> 8)); //д��16λ��ַ
	CTP_IIC_Wait_Ack(); 
	CTP_IIC_Send_Byte((u8)addr);
	CTP_IIC_Wait_Ack(); 
	status = SUCCESS;
	return status;	
}

/*****************************************************************************
 * @name       :u8 GT9XX_WriteData (u16 addr,u8 value)
 * @date       :2019-06-20 
 * @function   :GT911 write data to register
 * @parameters :addr:register address
                value:the value for writen
 * @retvalue   :the status of writen
******************************************************************************/
u8 GT9XX_WriteData (u16 addr,u8 value)
{
	u8 status;
	CTP_IIC_Start();

	GT9XX_WriteHandle(addr);
	CTP_IIC_Send_Byte(value);
	CTP_IIC_Wait_Ack(); 
	CTP_IIC_Stop();	
	status = SUCCESS;
	return status;
}

/*****************************************************************************
 * @name       :u8 GT9XX_ReadData (u16 addr, u8 cnt, u8 *value)
 * @date       :2019-06-20 
 * @function   :GT911 read data to register
 * @parameters :addr:register address
                cnt:the number of data which is read
                value:the point of read data
 * @retvalue   :the status of writen
******************************************************************************/
u8 GT9XX_ReadData (u16 addr, u8 cnt, u8 *value)
{
	u8 status;
	u8 i;

	status = ERROR;
	CTP_IIC_Start();
	GT9XX_WriteHandle(addr);
	CTP_IIC_Start();
	delay_us(20);
	CTP_IIC_Send_Byte(GT9XX_IIC_RADDR);
	CTP_IIC_Wait_Ack(); 
	for(i = 0 ; i < cnt; i++)
	{
		value[i]=CTP_IIC_Read_Byte(i == (cnt - 1)?0:1);
	}					
	CTP_IIC_Stop();
	status = SUCCESS;
	return (status);	
}

/*****************************************************************************
 * @name       :u8 GT911_Init(void)
 * @date       :2019-06-20 
 * @function   :GT911 IC initialization
 * @parameters :None
 * @retvalue   :the status of initialization
******************************************************************************/
u8 GT911_Init(void)
{
	u8 touchIC_ID[4];	
	GT911_gpio_init();
//	GT9xx_Eint_Init();
	GT911_reset_guitar(GT9XX_IIC_WADDR);
	GT911_int_sync(50);
	GT9XX_ReadData (GT9XX_ID_ADDR,4,touchIC_ID);
	if( touchIC_ID[0] == '9' )
	{
//		printf("Touch ID: %s \r\n",touchIC_ID);
		//GT9xx_send_config();
		return 1;
	}
	else
	{
		//printf("Touch Error\r\n");
		return 0;
	}
}

/*****************************************************************************
 * @name       :u8 Touch_Get_Count(void)
 * @date       :2019-06-20 
 * @function   :get the count of read data
 * @parameters :None
 * @retvalue   :the count of read data
******************************************************************************/
u8 Touch_Get_Count(void)
{
	u8 count[1] = {0};
	GT9XX_ReadData (GT9XX_READ_ADDR,1,count);	//read touch data
	return (count[0]&0x0f);
}

const u16 TPX[] = {0x8150,0x8158,0x8160,0x8168,0x8170}; //���������������ݵ�ַ��1~5��

/*****************************************************************************
 * @name       :u8 GT911_Scan(void)
 * @date       :2019-06-20 
 * @function   :Query GT911 trigger status
 * @parameters :None
 * @retvalue   :the status of GT911 trigger
******************************************************************************/
u8 GT911_Scan(void)
{
	u8 buf[42];
	u8 i=0;
	u8 res=0;
	u8 temp;
	u8 tempsta;
#if SCAN_TYPE
	if(touch_flag)
#else	
 	static u8 t=0;//���Ʋ�ѯ���,�Ӷ�����CPUռ����   
	t++;
	if((t%10)==0||t<10)//����ʱ,ÿ����10��CTP_Scan�����ż��1��,�Ӷ���ʡCPUʹ����
#endif
	{ 
		GT9XX_ReadData(GT9XX_READ_ADDR, 42, buf);
		if((buf[0]&0X80)&&((buf[0]&0XF)<6))
		{	
			temp=0;	
			GT9XX_WriteData(GT9XX_READ_ADDR,temp);
		}		
		if((buf[0]&0XF)&&((buf[0]&0XF)<6))
		{
			for(i=0;i<(buf[0]&0XF);i++)
			{
				switch(buf[1+i*8])
				{
					case 4:
					{
						temp |= 1<<4;
						break;
					}
					case 3:
					{
						temp |= 1<<3;
						break;
					}
					case 2:
					{
						temp |= 1<<2;
						break;
					}
					case 1:
					{
						temp |= 1<<1;
						break;
					}
					case 0:
					{	
						temp |= 1<<0;
						break;
					}
					default:
					break;				
				}
			}
//			temp=0XFF<<(buf[0]&0XF);		//����ĸ���ת��Ϊ1��λ��,ƥ��tp_dev.sta���� 
			tempsta=tp_dev.sta;			//���浱ǰ��tp_dev.staֵ
			tp_dev.sta=temp|TP_PRES_DOWN|TP_CATH_PRES; 
			tp_dev.x[4]=tp_dev.x[0];	//���津��0������
			tp_dev.y[4]=tp_dev.y[0];
			for(i=0;i<CTP_MAX_TOUCH;i++)
			{
				if(tp_dev.sta&(1<<i))	//������Ч?
				{
					//GT9XX_ReadData(TPX[i],4,buf);	//��ȡXY����ֵ
					if(lcddev.dir==0)
					{
						tp_dev.y[i]=((u16)buf[3+i*8]<<8)+buf[2+i*8];
						tp_dev.x[i]=lcddev.width-(((u16)buf[5+i*8]<<8)+buf[4+i*8]);
					}
					else if(lcddev.dir==1)
					{
						tp_dev.x[i]=((u16)buf[3+i*8]<<8)+buf[2+i*8];
						tp_dev.y[i]=((u16)buf[5+i*8]<<8)+buf[4+i*8];
					}
					else if(lcddev.dir==2)
					{
						tp_dev.y[i]=lcddev.height-(((u16)buf[3+i*8]<<8)+buf[2+i*8]);
						tp_dev.x[i]=((u16)buf[5+i*8]<<8)+buf[4+i*8];
					}
					else if(lcddev.dir==3)
					{
						tp_dev.x[i]=lcddev.width-(((u16)buf[3+i*8]<<8)+buf[2+i*8]);
						tp_dev.y[i]=lcddev.height-(((u16)buf[5+i*8]<<8)+buf[4+i*8]);						
					}  
					//printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
				}
			} 
			res=1;
			if(tp_dev.x[0]>lcddev.width||tp_dev.y[0]>lcddev.height)//�Ƿ�����(���곬����)
			{ 
				if((buf[0]&0XF)>1)		//��������������,�򸴵ڶ�����������ݵ���һ������.
				{
					tp_dev.x[0]=tp_dev.x[1];
					tp_dev.y[0]=tp_dev.y[1];
					#if !SCAN_TYPE
					t=0;				//����һ��,��������������10��,�Ӷ����������
					#endif
				}else					//�Ƿ�����,����Դ˴�����(��ԭԭ����)  
				{
					tp_dev.x[0]=tp_dev.x[4];
					tp_dev.y[0]=tp_dev.y[4];
					buf[0]=0X80;		
					tp_dev.sta=tempsta;	//�ָ�tp_dev.sta
				}
			}
			#if !SCAN_TYPE
			else t=0;					//����һ��,��������������10��,�Ӷ����������
			#endif
		}
	}
	if((buf[0]&0X8F)==0X80)//�޴����㰴��
	{ 
	#if SCAN_TYPE
	touch_flag=0;
	#endif
		if(tp_dev.sta&TP_PRES_DOWN)	//֮ǰ�Ǳ����µ�
		{
			tp_dev.sta&=~(1<<7);	//��ǰ����ɿ�
		}else						//֮ǰ��û�б�����
		{ 
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
			tp_dev.sta&=0XE0;	//�������Ч���	
		}	
	}
#if !SCAN_TYPE
	if(t>240)t=10;//���´�10��ʼ����
#endif
	return res;
}

