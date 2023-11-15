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
// RS ---> PD11 //LCD data/command control signal
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

#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "touch.h"
#include "gui.h"
#include "test.h"
#include "led.h"

vu8 bright=128, contr=32, sat=64;

int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//Set system interrupt priority group 2
	delay_init(168); //Initialize the delay function
	LED_Init();
	LCD_Init(); //LCD initialization
	 //loop test
	while(1)
	{
		LCD_SetPostProc(bright,contr,sat);
		main_test(); //Test the main interface
		Test_Pixels();
		Test_Triangle2();
		Test_Read(); //Read ID and color value test
		Test_Color(); //Simple refresh fill test
		Test_FillRec(); //GUI rectangle drawing test
		Test_Circle(); //GUI circle test
		Test_Triangle(); //GUI triangle drawing test
		English_Font_test();//English font example test
		Chinese_Font_test();//Chinese font example test
		Pic_test(); //Picture display example test
		Test_Dynamic_Num(); //Dynamic number display
		Rotate_Test(); //rotate display test
		//If you don't have touch, or don't need touch function, please comment out the following touch screen test items
		//Touch_Test(); //Touch screen handwriting test
	}
}

