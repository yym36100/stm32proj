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

#ifndef __GT911_DRIVER_H
#define __GT911_DRIVER_H

#include "sys.h"


#define RST_OUT()	{	GPIOC->MODER&=~(3<<(13*2));GPIOC->MODER|=1<<13*2;}		//set RSSET pin to output
#define INT_OUT()	{	GPIOB->MODER&=~(3<<(1*2));GPIOB->MODER|=1<<1*2;}		//set RSSET pin to output
#define INT_IN()	{	GPIOB->MODER&=~(3<<(1*2));GPIOB->MODER|=0<<1*2;}		//set RSSET pin to output

//RST--PC13
//INT--PB1
#define RST_CTRL   		PCout(13)	//GT911 RESET pin out high or low
#define INT_CTRL   		PBout(1) 	//GT911 INT pin out high or low
#define INT_GET   		PBin(1) 	//Get GT911 INT pin status


#define GT9XX_IIC_RADDR 0xBB	//IIC read address
#define GT9XX_IIC_WADDR 0xBA	//IIC write address

#define GT9XX_READ_ADDR 0x814E	//touch point information
#define GT9XX_ID_ADDR 0x8140		//ID of touch IC


void GT911_int_sync(u16 ms);
void GT911_reset_guitar(u8 addr);
void GT911_gpio_init(void);
u8 GT9XX_WriteHandle (u16 addr);
u8 GT9XX_WriteData (u16 addr,u8 value);
u8 GT9XX_ReadData (u16 addr, u8 cnt, u8 *value);
u8 GT911_Init(void);
u8 Touch_Get_Count(void);
u8 GT911_Scan(void);
void GT9xx_send_config(void);
void GT9xx_Eint_Init(void);


#endif
