
#include <stdint.h>
#include "main.h"


#include "max7219.h"



static uint8_t MAX_DispData[MAX_NoChainedDisplays * 8] = {0};


extern SPI_HandleTypeDef hspi1;



#define CS(x) HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, x)
#define SPI_Send(x) HAL_SPI_Transmit(&hspi1, x, 1, 10)

void MAX_Init(void){
	// init devices
	uint16_t cfg;
	CS(0); for(int i=0;i<MAX_NoChainedDisplays;i++){ cfg = 0x0900; SPI_Send((uint8_t*)&cfg); } CS(1);// no decode mode
	CS(0); for(int i=0;i<MAX_NoChainedDisplays;i++){ cfg=0x0a00; SPI_Send((uint8_t*)&cfg); } CS(1); // min intensity
	CS(0); for(int i=0;i<MAX_NoChainedDisplays;i++){ cfg = 0x0b07; SPI_Send((uint8_t*)&cfg); } CS(1); // scan limit

	CS(0); for(int i=0;i<MAX_NoChainedDisplays;i++){cfg = 0x0c01; SPI_Send((uint8_t*)&cfg); } CS(1); //wakeup
}


void MAX_Test(void){
	static uint8_t s = 0;
	uint16_t cfg;
	CS(0); for(int i=0;i<MAX_NoChainedDisplays;i++){cfg = (((s%8)+1)<<8) + (rand()&0xff); SPI_Send((uint8_t*)&cfg); } CS(1);
	s++;
}


uint8_t MAX_GetLine(uint8_t dev, uint8_t line) {
	uint8_t x;
	uint8_t y;
	uint8_t line_offs = 8;

	if(dev>7){dev-=8;line_offs = 0;}

	x = dev;
	y= line + line_offs;


	return MAX_DispData[y*8+x];
}

void MAX_UpdateDisp(void) {
	uint16_t cfg;
	uint8_t pixels = 0;
	for (int j = 0; j < 8; j++) {
		CS(0);
		for (int i = 0; i < MAX_NoChainedDisplays; i++) {
			pixels = MAX_GetLine(i,j);
			cfg = ((j + 1) << 8) + pixels;
			SPI_Send((uint8_t* )&cfg);
		}
		CS(1);
	}
}

void MAX_SetPixel(uint8_t x, uint8_t y, uint8_t c) {

	if(x>=64)return;
	if(y>=16)return;
	uint8_t xd = x / 8;
	uint8_t xb = x % 8;
	if(c) MAX_DispData[y*8+ xd] |= 0x80>>xb;
	else MAX_DispData[y*8+ xd] &= ~(0x80>>xb);
}


