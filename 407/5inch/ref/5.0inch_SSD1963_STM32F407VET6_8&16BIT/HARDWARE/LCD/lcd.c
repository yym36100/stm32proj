/**************************************************** *************************************
//The STM32 connection pin refers to the STM32 pin connected internally to the TFTLCD slot or socket pin
//==================================== Power Wiring ============== =============================//
// LCD module STM32 connection pins
// 3.3V ---> DC3.3V // Power supply
// GND ---> GND //Power ground
//================================= LCD data cable connection =============== =======================//
//The default data bus type of this module is 16-bit parallel port bus (please use DB0~DB7 for 8-bit mode)
// LCD module STM32 connection pins
// D0 ---> PD14 - |
// D1 ---> PD15 |
// D2 ---> PD0 |
// D3 ---> PD1 |
// D4 ---> PE7 |
// D5 ---> PE8 |
// D6 ---> PE9 |
// D7 ---> PE10 |===>LCD 16-bit parallel port data signal
// D8 ---> PE11 |
// D9 ---> PE12 |
// D10 ---> PE13 |
// D11 ---> PE14 |
// D12 ---> PE15 |
// D13 ---> PD8 |
// D14 ---> PD9 |
// D15 ---> PD10 -|
//================================= LCD screen control line wiring =============== =======================//
// LCD module STM32 connection pins
// WR ---> PD5 //LCD write data control signal
// RD ---> PD4 //LCD read data control signal
// RS ---> PD13 //LCD data/command control signal
// REST ---> reset pin (default) // LCD screen reset control signal (PD13 can also be selected)
// CS ---> PD7 // LCD chip selection control signal
// LED_A ---> PB15 // LCD backlight control signal
//======================================================================================================================== ==========================//
//If the module does not have a touch function or has a touch function, but does not require a touch function, no touch screen wiring is required
// LCD module STM32 connection pins
// T_IRQ ---> PB1 // capacitive or resistive touch screen touch interrupt signal
// T_DO ---> PB2 //Resistive touch screen SPI bus read signal
// T_DIN ---> PC4 //Resistive touch screen SPI bus write signal or capacitive touch screen IIC bus data signal
// T_CS ---> PC13 //Resistive touch screen chip select control signal or capacitive touch screen reset signal
// T_CLK ---> PB0 //Resistive touch screen SPI bus or capacitive touch screen IIC bus clock signal
***************************************************** *************************************/

#include "lcd.h"
#include "stdlib.h"
#include "delay.h"	 

	   
//Manage important LCD parameters
//Default is vertical screen
_lcd_dev lcddev;

//Brush color, background color
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;	 


u16 LCD_read(void)
{
	vu16 data;  //Prevent being optimized
	data=LCD->LCD_RAM;	
	return data;
}

/*****************************************************************************
 * @name       :void LCD_WR_REG(u16 data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_REG(vu16 data)
{ 
	data= data; //When using-O2 optimization, the delay that must be inserted
	LCD->LCD_REG=data;//Write the serial number of the register to be written
}

/*****************************************************************************
 * @name       :void LCD_WR_DATA(u16 data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_DATA(vu16 data)
{
	data=data;			//When using-O2 optimization, the delay that must be inserted
	LCD->LCD_RAM=data;
}

/*****************************************************************************
 * @name       :u16 LCD_RD_DATA(void)
 * @date       :2018-11-13 
 * @function   :Read an 16-bit value from the LCD screen
 * @parameters :None
 * @retvalue   :read value
******************************************************************************/
u16 LCD_RD_DATA(void)
{
	return LCD_read();
}

/*****************************************************************************
 * @name       :void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{	
	LCD->LCD_REG=LCD_Reg;//Original image
	LCD->LCD_RAM=LCD_RegValue;//Original image     		 
}	   

/*****************************************************************************
 * @name       :u16 LCD_ReadReg(u16 LCD_Reg)
 * @date       :2018-11-13 
 * @function   :read value from specially registers
 * @parameters :LCD_Reg:Register address
 * @retvalue   :read value
******************************************************************************/
void LCD_ReadReg(u16 LCD_Reg,u8 *Rval,int n)
{
	LCD_WR_REG(LCD_Reg); 
	while(n--)
	{		
		*(Rval++) = LCD_RD_DATA();
		delay_us(100);
	}
}

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 

/*****************************************************************************
 * @name       :void LCD_ReadRAM_Prepare(void)
 * @date       :2018-11-13 
 * @function   :Read GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void LCD_ReadRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.rramcmd);
}

/*****************************************************************************
 * @name       :void Lcd_WriteData_16Bit(u16 Data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/	 
void Lcd_WriteData_16Bit(u16 Data)
{
#if LCD_USE8BIT_MODEL	
	 LCD->LCD_RAM = (Data>>8)&0xF8;
	 LCD->LCD_RAM = (Data>>3)&0xFC;
	 LCD->LCD_RAM = Data<<3;
#else
	 LCD->LCD_RAM = Data;  //Write 16-bit color value
#endif
}

u16 Color_To_565(u8 r, u8 g, u8 b)
{
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}

/*****************************************************************************
 * @name       :u16 Lcd_ReadData_16Bit(void)
 * @date       :2018-11-13 
 * @function   :Read an 16-bit value from the LCD screen
 * @parameters :None
 * @retvalue   :read value
******************************************************************************/	
u16 Lcd_ReadData_16Bit(void)
{
	u16 r=0;
#if LCD_USE8BIT_MODEL	
	u16 g=0,b=0;
#endif
	//16bit: red green and blue data
	//8bit:red data
	r = LCD_RD_DATA();
	delay_us(1);
#if LCD_USE8BIT_MODEL
	//8bit:green data	
	g = LCD_RD_DATA();
	delay_us(1);
	//8bit:blue data
	b = LCD_RD_DATA();
	delay_us(1);
	r=Color_To_565(r, g, b);
#endif
	return r;
}

/*****************************************************************************
 * @name       :void LCD_DrawPoint(u16 x,u16 y)
 * @date       :2018-08-09 
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/	
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);
	Lcd_WriteData_16Bit(POINT_COLOR); 
}

/*****************************************************************************
 * @name       :u16 LCD_ReadPoint(u16 x,u16 y)
 * @date       :2018-11-13 
 * @function   :Read a pixel color value at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :the read color value
******************************************************************************/	
u16 LCD_ReadPoint(u16 x,u16 y)
{
	u16 color;
	if(x>=lcddev.width||y>=lcddev.height)
	{
		return 0;	//Out of range, return directly
	}
	LCD_SetCursor(x,y);
	LCD_ReadRAM_Prepare();
	color = Lcd_ReadData_16Bit();
	return color;
}

/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/	
void LCD_Clear(u16 Color)
{
  unsigned int i;
	u32 total_point=lcddev.width*lcddev.height;
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);   
	for(i=0;i<total_point;i++)
	{ 
#if LCD_USE8BIT_MODEL	
	 LCD->LCD_RAM = (Color>>8)&0xF8;
	 LCD->LCD_RAM = (Color>>3)&0xFC;
	 LCD->LCD_RAM = Color<<3;
#else
		LCD->LCD_RAM = Color;
#endif
	}
} 

/*****************************************************************************
 * @name       :void LCD_PWM_BackLightSet(u8 pwm)
 * @date       :2020-05-13
 * @function   :Set the backlight brightness from PWM
 * @parameters :pwm:pwm value 0~100, The greater the value, the brighter
 * @retvalue   :None
******************************************************************************/
void LCD_PWM_BackLightSet(u8 pwm)
{
	LCD_WR_REG(0xBE); //Configure PWM output
	LCD_WR_DATA(0x05); //1 Set the PWM frequency
	LCD_WR_DATA (pwm*2.55);//2 Set the PWM duty cycle
	LCD_WR_DATA(0x01); //3 Set C
	LCD_WR_DATA(0xFF); //4 Set D
	LCD_WR_DATA(0x00); //5 Set E
	LCD_WR_DATA(0x00); //6 Set F
}

/*****************************************************************************
 * @name       :void LCD_GPIOInit(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen GPIO
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef readWriteTiming;
	FSMC_NORSRAMTimingInitTypeDef writeTiming;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE);//Enable PD, PE, PF, PG clock
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);//Enable FSMC clock
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//PB15 push-pull output, control backlight
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//Normal output mode
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//Push-pull output
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//pull up
   GPIO_Init(GPIOB, &GPIO_InitStructure);//Initialization //PB15 push-pull output, control backlight

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//PD13 push-pull output, control reset
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//Normal output mode
//   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//Push-pull output
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//pull up
//   GPIO_Init(GPIOD, &GPIO_InitStructure);//Initialization //PB15 push-pull output, control backlight

   GPIO_InitStructure.GPIO_Pin = (3<<0)|(3<<4)|(7<<8)|(3<<14);//PD0,1,4,5,8,9,10,14, 15 AF OUT
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//multiplex output
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//Push-pull output
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//pull up
   GPIO_Init(GPIOD, &GPIO_InitStructure);//Initialization

   GPIO_InitStructure.GPIO_Pin = (0X1FF<<7);//PE7~15, AF OUT
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//multiplex output
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//Push-pull output
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//pull up
   GPIO_Init(GPIOE, &GPIO_InitStructure);//Initialization

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_13;//PF12, FSMC_A6
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//multiplex output
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//Push-pull output
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//pull up
   GPIO_Init(GPIOD, &GPIO_InitStructure);//Initialization


   GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FSMC);//PD0,AF12
   GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FSMC);//PD1,AF12
   GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
   GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FSMC);
   GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_FSMC);
   GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_FSMC);
   GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_FSMC);
   GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
   GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC);//PD15,AF12
 
   GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC);//PE7,AF12
   GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC);
   GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC);
   GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC);
   GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_FSMC);
   GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_FSMC);
   GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_FSMC);
   GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_FSMC);
   GPIO_PinAFConfig(GPIOE,GPIO_PinSource15,GPIO_AF_FSMC);//PE15,AF12
 
   GPIO_PinAFConfig(GPIOD,GPIO_PinSource7,GPIO_AF_FSMC);
GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_FSMC);


   readWriteTiming.FSMC_AddressSetupTime = 0XF; //Address setup time (ADDSET) is 16 HCLK 1/168M=6ns*16=96ns
   readWriteTiming.FSMC_AddressHoldTime = 0x00; //Address hold time (ADDHLD) mode A is not used
   readWriteTiming.FSMC_DataSetupTime = 60; //Data saving time is 60 HCLK =6*60=360ns
   readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
   readWriteTiming.FSMC_CLKDivision = 0x00;
   readWriteTiming.FSMC_DataLatency = 0x00;
   readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A; //Mode A
    

writeTiming.FSMC_AddressSetupTime =9; //Address setup time (ADDSET) is 9 HCLK =54ns
   writeTiming.FSMC_AddressHoldTime = 0x00; //Address hold time (A
   writeTiming.FSMC_DataSetupTime = 8; //Data saving time is 6ns*9 HCLK=54ns
   writeTiming.FSMC_BusTurnAroundDuration = 0x00;
   writeTiming.FSMC_CLKDivision = 0x00;
   writeTiming.FSMC_DataLatency = 0x00;
   writeTiming.FSMC_AccessMode = FSMC_AccessMode_A; //Mode A

 
   FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;// Here we use NE4, which corresponds to BTCR[6],[7].
   FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // Do not multiplex data address
   FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM; //SRAM
#if LCD_USE8BIT_MODEL
   FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;//Memory data width is 8bit
#else
   FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//Memory data width is 16bit
#endif
   FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable;
   FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable;
   FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
   FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
   FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable; // memory write enable
   FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
   FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // Read and write use different timings
   FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
   FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //Read and write timing
   FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming; //write timing

   FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); //Initialize FSMC configuration

   FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE); // Enable BANK1

//Reconfigure the timing of writing the timing control register to make the WR timing the fastest
FSMC_Bank1E->BWTR[0]&=~(0XF<<0);//Address establishment time (ADDSET) is cleared
FSMC_Bank1E->BWTR[0]&=~(0XF<<8);//Reset the data storage time
FSMC_Bank1E->BWTR[0]|=4<<0; //Address establishment time (ADDSET) is 4 HCLK =24ns
FSMC_Bank1E->BWTR[0]|=4<<8; //Data storage time (DATAST) is 6ns*4 HCLK=24ns
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_RESET(void)
{
	LCD_RST=0;
	delay_ms(100);	
	LCD_RST=1;
	delay_ms(50);
}

/*****************************************************************************
 * @name       :void LCD_Init(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 	 
void LCD_Init(void)
{  
	LCD_GPIOInit();//LCD GPIO initialization	
	delay_ms(100);
	//LCD_RESET(); //If the development board reset pin is not used, this reset function is called
//************* SSD1963 initialization**********//	
	LCD_WR_REG(0xE2);		//Set PLL with OSC = 10MHz (hardware),	Multiplier N = 35, 250MHz < VCO < 800MHz = OSC*(N+1), VCO = 300MHz
	LCD_WR_DATA(0x1D); //Parameter 1 
	LCD_WR_DATA(0x02); //Parameter 2 Divider M=2, PLL=300/(M+1) =100MHz
	LCD_WR_DATA(0x04);//Parameter 3 Validate M and N values   
	delay_us(100);
	LCD_WR_REG(0xE0);		// Start PLL command
	LCD_WR_DATA(0x01);		// enable PLL
	delay_ms(10);
	LCD_WR_REG(0xE0);		// Start PLL command again
	LCD_WR_DATA(0x03);		// now, use PLL output as system clock	
	delay_ms(12);  
	LCD_WR_REG(0x01); //Soft reset
	delay_ms(10);
		
	LCD_WR_REG(0xE6); //Set the pixel frequency, 33MHz
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0xFF);
	LCD_WR_DATA(0xFF);
		
	LCD_WR_REG(0xB0); //Set LCD mode
	LCD_WR_DATA(0x20); //24-bit mode
	LCD_WR_DATA(0x00);//TFT mode 
	
	LCD_WR_DATA((SSD_HOR_RESOLUTION-1)>>8);// Set LCD horizontal pixels
	LCD_WR_DATA(SSD_HOR_RESOLUTION-1);		 
	LCD_WR_DATA((SSD_VER_RESOLUTION-1)>>8);// Set LCD vertical pixels
	LCD_WR_DATA(SSD_VER_RESOLUTION-1);		 
	LCD_WR_DATA(0x00); //RGB sequence 
		
	LCD_WR_REG(0xB4);		//Set horizontal period
	LCD_WR_DATA((SSD_HT-1)>>8);
	LCD_WR_DATA(SSD_HT-1);
	LCD_WR_DATA(SSD_HPS>>8);
	LCD_WR_DATA(SSD_HPS);
	LCD_WR_DATA(SSD_HOR_PULSE_WIDTH-1);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0xB6);		//Set vertical period
	LCD_WR_DATA((SSD_VT-1)>>8);
	LCD_WR_DATA(SSD_VT-1);
	LCD_WR_DATA(SSD_VPS>>8);
	LCD_WR_DATA(SSD_VPS);
	LCD_WR_DATA(SSD_VER_FRONT_PORCH-1);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
		
	LCD_WR_REG(0xF0); //Set the interface between SSD1963 and CPU to 16bit  
#if LCD_USE8BIT_MODEL 
	LCD_WR_DATA(0x00);	//8-bit(565 format) data for 16bpp 
#else
	LCD_WR_DATA(0x03);	//16-bit(565 format) data for 16bpp 
#endif
	LCD_WR_REG(0x29); //Turn on the display
	//Set the PWM output, the backlight is adjustable through the duty cycle 
	LCD_WR_REG(0xD0); //Set automatic white balance DBC
	LCD_WR_DATA(0x00);	//disable
	
	LCD_WR_REG(0xBE); //Configure PWM output
	LCD_WR_DATA(0x05); //1 Set the PWM frequency
	LCD_WR_DATA(0x10); //2 Set the PWM duty cycle
	LCD_WR_DATA(0x01); //3 Set C
	LCD_WR_DATA(0x00); //4 Set D
	LCD_WR_DATA(0x00); //5 Set E 
	LCD_WR_DATA(0x00); //6 Set F 
		
	LCD_WR_REG(0xB8); //Set GPIO configuration
	LCD_WR_DATA(0x03); //2 IO ports are set to output
	LCD_WR_DATA(0x01); //GPIO uses normal IO functions 
	LCD_WR_REG(0xBA);
	LCD_WR_DATA(0X01);//GPIO[1:0]=01, control LCD direction
		
	LCD_PWM_BackLightSet(10);//The backlight is set to the brightest

  //LCD_DIRECT (USE_HORIZONTAL);//Set the LCD display direction
	LCD_direction(3);
	LCD_LED=1;//Light up the backlight	 
	LCD_Clear(WHITE);//Clear full screen white
}
 
/*****************************************************************************
 * @name       :void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
 * @date       :2018-08-09 
 * @function   :Setting LCD display window
 * @parameters :xStar:the bebinning x coordinate of the LCD display window
								yStar:the bebinning y coordinate of the LCD display window
								xEnd:the endning x coordinate of the LCD display window
								yEnd:the endning y coordinate of the LCD display window
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	switch(lcddev.dir)
	{
		case 0:
			xStar=xStar+xEnd;
			xEnd=xStar-xEnd;
			xStar=xStar-xEnd;
			xStar=lcddev.width-xStar-1;
			xEnd=lcddev.width-xEnd-1;
		break;
		case 2:
			yStar=yStar+yEnd;
			yEnd=yStar-yEnd;
			yStar=yStar-yEnd;
			yStar=lcddev.height-yStar-1;
			yEnd=lcddev.height-yEnd-1;
		break;
		case 3:
			xStar=xStar+xEnd;
			xEnd=xStar-xEnd;
			xStar=xStar-xEnd;
			xStar=lcddev.width-xStar-1;
			xEnd=lcddev.width-xEnd-1;
			yStar=yStar+yEnd;
			yEnd=yStar-yEnd;
			yStar=yStar-yEnd;
			yStar=lcddev.height-yStar-1;
			yEnd=lcddev.height-yEnd-1;
		break;
		default:
		break;
	}
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(xStar>>8);
	LCD_WR_DATA(0x00FF&xStar);		
	LCD_WR_DATA(xEnd>>8);
	LCD_WR_DATA(0x00FF&xEnd);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(yStar>>8);
	LCD_WR_DATA(0x00FF&yStar);		
	LCD_WR_DATA(yEnd>>8);
	LCD_WR_DATA(0x00FF&yEnd);

	LCD_WriteRAM_Prepare();	//Start writing to GRAM	
}   

void LCD_SetPostProc(u8 bright, u8 contr, u8 sat){
	LCD_WR_REG(0xbc);	
	LCD_WR_DATA(contr);
	LCD_WR_DATA(bright);
	LCD_WR_DATA(sat);
	LCD_WR_DATA(1);
}

/*****************************************************************************
 * @name       :void LCD_SetCursor(u16 Xpos, u16 Ypos)
 * @date       :2018-08-09 
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);	
} 

/*****************************************************************************
 * @name       :void LCD_direction(u8 direction)
 * @date       :2018-08-09 
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
													2-180 degree
													3-270 degree
 * @retvalue   :None
******************************************************************************/ 
void LCD_direction(u8 direction)
{ 
	lcddev.wramcmd=0x2C;
	lcddev.rramcmd=0x2E;
	lcddev.dir=direction%4;
	switch(lcddev.dir){		  
		case 0:	
			lcddev.setxcmd=0x2B;
			lcddev.setycmd=0x2A;
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;		
			LCD_WriteReg(0x36,(0<<3)|(1<<7)|(0<<6)|(1<<5));
		break;
		case 1:
			lcddev.setxcmd=0x2A;
			lcddev.setycmd=0x2B;
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x36,(0<<3)|(0<<7)|(0<<6)|(0<<5));
		break;
		case 2:
			lcddev.setxcmd=0x2B;
			lcddev.setycmd=0x2A;		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;	
			LCD_WriteReg(0x36,(0<<3)|(0<<7)|(1<<6)|(1<<5));
		break;
		case 3:
			lcddev.setxcmd=0x2A;
			lcddev.setycmd=0x2B;
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x36,(0<<3)|(1<<7)|(1<<6)|(0<<5));
		break;	
		default:break;
	}		
}	 

/*****************************************************************************
 * @name       :u16 LCD_Read_ID(void)
 * @date       :2018-11-13 
 * @function   :Read ID
 * @parameters :None
 * @retvalue   :ID value
******************************************************************************/ 
u16 LCD_Read_ID(void)
{
	u8 val[3] = {0};
	LCD_ReadReg(0xA1,val,3);
	return (val[1]<<8)|val[2];
}
