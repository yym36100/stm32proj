/************************************************************************************
//STM32连接引脚是指TFTLCD插槽或者插座引脚内部连接的STM32引脚
//=================================电源接线=======================================//
//     LCD模块                    STM32连接引脚
//      3.3V          --->             DC3.3V          //电源
//      GND           --->             GND              //电源地
//=============================液晶屏数据线接线===================================//
//本模块默认数据总线类型为16位并口总线(8位模式请使用DB0~DB7)
//     LCD模块                    STM32连接引脚
//      D0            --->            PD14        -|   
//      D1            --->            PD15         |  
//      D2            --->            PD0          | 
//      D3            --->            PD1          | 
//      D4            --->            PE7          |
//      D5            --->            PE8          |
//      D6            --->            PE9          |
//      D7            --->            PE10         |===>液晶屏16位并口数据信号
//      D8            --->            PE11         |
//      D9            --->            PE12         |
//      D10           --->            PE13         |
//      D11           --->            PE14         |
//      D12           --->            PE15         |
//      D13           --->            PD8          |
//      D14           --->            PD9          |
//      D15           --->            PD10        -|
//=============================液晶屏控制线接线===================================//
//     LCD模块 				            STM32连接引脚 
//      WR            --->            PD5             //液晶屏写数据控制信号
//      RD            --->            PD4             //液晶屏读数据控制信号
//      RS            --->            PD11            //液晶屏数据/命令控制信号
//      REST          --->          复位引脚（默认）  //液晶屏复位控制信号（也可选择PD13）
//      CS            --->            PD7             //液晶屏片选控制信号
//      LED_A         --->            PB15            //液晶屏背光控制信号
//===============================触摸屏触接线=====================================//
//如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
//	   LCD模块                    STM32连接引脚 
//      T_IRQ         --->            PB1             //电容或电阻触摸屏触摸中断信号
//      T_DO          --->            PB2             //电阻触摸屏SPI总线读信号
//      T_DIN         --->            PC4             //电阻触摸屏SPI总线写信号或电容触摸屏IIC总线数据信号
//      T_CS          --->            PC13            //电阻触摸屏片选控制信号或电容触摸屏复位信号
//      T_CLK         --->            PB0             //电阻触摸屏SPI总线或电容触摸屏IIC总线时钟信号
*************************************************************************************/			
#include "myiic.h"
#include "delay.h"

/*****************************************************************************
 * @name       :void IIC_Init(void)
 * @date       :2018-08-09 
 * @function   :Initialize IIC
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟

  //GPIOB8,B9初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	IIC_SCL=1;
	IIC_SDA=1;
}

/*****************************************************************************
 * @name       :void IIC_Start(void)
 * @date       :2018-08-09 
 * @function   :Generating IIC starting signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	

/*****************************************************************************
 * @name       :void IIC_Stop(void)
 * @date       :2018-08-09 
 * @function   :Generating IIC stop signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/  
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}

/*****************************************************************************
 * @name       :u8 IIC_Wait_Ack(void)
 * @date       :2018-08-09 
 * @function   :Wait for the response signal
 * @parameters :None
 * @retvalue   :0-receive response signal successfully
								1-receive response signal unsuccessfully
******************************************************************************/ 
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 

/*****************************************************************************
 * @name       :void IIC_Ack(void)
 * @date       :2018-08-09 
 * @function   :Generate ACK response signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/ 
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}

/*****************************************************************************
 * @name       :void IIC_NAck(void)
 * @date       :2018-08-09 
 * @function   :Don't generate ACK response signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/ 	    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}	

/*****************************************************************************
 * @name       :void IIC_Send_Byte(u8 txd)
 * @date       :2018-08-09 
 * @function   :send a byte data by IIC bus
 * @parameters :txd:Data to be sent
 * @retvalue   :None
******************************************************************************/				 				     	  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 

/*****************************************************************************
 * @name       :u8 IIC_Read_Byte(unsigned char ack)
 * @date       :2018-08-09 
 * @function   :read a byte data by IIC bus
 * @parameters :ack:0-send nACK
									  1-send ACK
 * @retvalue   :Data to be read
******************************************************************************/		    
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}



























