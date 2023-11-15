#ifndef FONT5X7_H
#define FONT5X7_H

/*
 * 5 x 7 ASCII font.
 *
 */

#define FONT_CHAR_HEIGHT 7
#define FONT_CHAR_WIDTH 5

/* Generic to compute font bit on/off for this font */
#if defined(VECT_TAB_SRAM)
	#define font_data(x)   (*(volatile unsigned long *)(0x0800f800+x))
	#define FONT_BIT(c, column, row) ((font_data((c*5)+(column)) & 1<<(7-(row))) != 0)


#else
	#define FONT_BIT(c, column, row) ((font_data[(c*5)+(column)] & 1<<(7-(row))) != 0)
#endif




extern const unsigned char font_data[];

#endif
