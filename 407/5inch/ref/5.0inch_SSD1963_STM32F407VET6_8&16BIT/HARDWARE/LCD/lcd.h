/**************************************************** *************************************
//The STM32 connection pin refers to the STM32 pin connected internally to the TFTLCD slot or socket pin
//==================================== Power Wiring ============== =============================//
// LCD module STM32 connection pins
// 3.3V ---> DC3.3V // Power supply
// GND ---> GND //Power Ground
//================================= LCD data cable connection =============== =======================//
//The default data bus type of this module is 16-bit parallel port bus (please use DB0~DB7 for 8-bit mode)
// LCD module STM32 connection pins
// D0 ---> PD14 -|
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
>>>>>// RS ---> PD11 //LCD data/command control signal
>>>// REST ---> reset pin (default) // LCD screen reset control signal (PD13 can also be selected)
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
#ifndef __LCD_H
#define __LCD_H
#include "sys.h"
#include "stdlib.h"

//LCD important parameter set
typedef struct
{
u16 width; //LCD width
u16 height; //LCD height
u16 id; //LCD ID
u8 dir; // Horizontal or vertical screen control: 0, vertical screen; 1, horizontal screen.
u16 wramcmd; //Start writing gram instruction
u16 rramcmd; //Start reading gram instruction
u16 setxcmd; //Set x coordinate command
u16 setycmd; //Set y coordinate command
}_lcd_dev;

//LCD parameter
extern _lcd_dev lcddev; //manage important parameters of LCD
/////////////////////////////////////User configuration area////////// //////////////////////////
#define USE_HORIZONTAL 1 //Define the clockwise rotation direction of the LCD screen 0-0 degree rotation, 1-90 degree rotation, 2-180 degree rotation, 3-270 degree rotation
#define LCD_USE8BIT_MODEL 0 //Define whether the data bus uses 8-bit mode 0, use 16-bit mode. 1, use 8-bit mode

//////////////////////////////////////////////////// //////////////////////////////////
//Define the size of the LCD
#define LCD_W 480
#define LCD_H 800

//The function to be called outside the TFTLCD part
extern u16 POINT_COLOR;//Default red
extern u16 BACK_COLOR; //Background color. The default is white

//////////////////////////////////////////////////// ///////////////////
//-----------------LCD port definition----------------
#define LED 15 //backlight control pin PB15
#define RST 13 //Reset pin PD13

//A triode is used to control the backlight on and off, and the user can also connect to PWM to adjust the brightness of the backlight
#define LCD_LED PBout(LED) //LCD backlight
#define LCD_RST PDout(RST) //reset pin PD13

//LCD address structure
typedef struct
{
#if LCD_USE8BIT_MODEL
vu8 LCD_REG;
vu8 LCD_RAM;
#else
vu16 LCD_REG;
vu16 LCD_RAM;
#endif
} LCD_TypeDef;

//Use Bank1.sector1 of NOR/SRAM, address bit HADDR[27,26]=00 A18 as data command line
#if LCD_USE8BIT_MODEL
//When using 8-bit mode, the STM32 internal address does not need to be shifted one bit to the right
#define LCD_BASE ((u32)(0x60000000 | 0x0000FFFF))
#else
//When using 16-bit mode, note that when setting, STM32 will shift one bit to the right for alignment! 111 1110=0X7E
#define LCD_BASE ((u32)(0x60000000 | 0x0007FFFE))
#endif
#define LCD ((LCD_TypeDef *) LCD_BASE)


//brush color
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40 //brown
#define BRRED 0XFC07 //brown red
#define GRAY 0X8430 // gray
//GUI color

#define DARKBLUE 0X01CF //dark blue
#define LIGHTBLUE 0X7D7C // light blue
#define GRAYBLUE 0X5458 // gray blue
//The above three colors are the colors of PANEL
 
#define LIGHTGREEN 0X841F // light green
#define LIGHTGRAY 0XEF5B //Light gray (PANNEL)
#define LGRAY 0XC618 //Light gray (PANNEL), form background color

#define LGRAYBLUE 0XA651 //Light gray blue (middle layer color)
#define LBBLUE 0X2B12 //Light brown blue (select the reverse color of the item)

void LCD_Init(void);
u16 LCD_read(void);
void LCD_Clear(u16 Color);
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void LCD_DrawPoint(u16 x,u16 y);//draw point
u16 LCD_ReadPoint(u16 x,u16 y); //read point
void LCD_SetWindows(u16 xStar, u16 yStar, u16 xEnd, u16 yEnd);
u16 LCD_RD_DATA(void);//Read LCD data
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue);
void LCD_WR_REG(u16 data);
void LCD_WR_DATA(u16 data);
void LCD_ReadReg(u16 LCD_Reg, u8 *Rval, int n);
void LCD_WriteRAM_Prepare(void);
void LCD_ReadRAM_Prepare(void);
void Lcd_WriteData_16Bit(u16 Data);
u16 Lcd_ReadData_16Bit(void);
void LCD_direction(u8 direction);
u16 Color_To_565(u8 r, u8 g, u8 b);
u16 LCD_Read_ID(void);
void LCD_PWM_BackLightSet(u8 pwm);

//LCD resolution setting
#define SSD_HOR_RESOLUTION 800 //LCD horizontal resolution
#define SSD_VER_RESOLUTION 480 //LCD vertical resolution
//LCD driver parameter setting
#define SSD_HOR_PULSE_WIDTH 1 //horizontal pulse width
#define SSD_HOR_BACK_PORCH 46 //Horizontal front porch
#define SSD_HOR_FRONT_PORCH 210 //Horizontal back porch

#define SSD_VER_PULSE_WIDTH 1 //vertical pulse width
#define SSD_VER_BACK_PORCH 23 // Vertical front porch
#define SSD_VER_FRONT_PORCH 22 // Vertical front porch
//The following parameters are automatically calculated
#define SSD_HT (SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS (SSD_HOR_BACK_PORCH)
#define SSD_VT (SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)

#endif
	 
	 



