#include <stdint.h>
#include "max7219.h"
#include "font5x7.h"

//void MAX_SetPixel(uint8_t x, uint8_t y, uint8_t c)

void drawline(int x0, int y0, int x1, int y1) {
	int dx, dy, p, x, y;

	dx = x1 - x0;
	dy = y1 - y0;

	x = x0;
	y = y0;

	p = 2 * dy - dx;

	while (x < x1) {
		if (p >= 0) {
			MAX_SetPixel(x, y, 1);
			y = y + 1;
			p = p + 2 * dy - 2 * dx;
		} else {
			MAX_SetPixel(x, y, 1);
			p = p + 2 * dy;
		}
		x = x + 1;
	}
}

void plot_line(int x0, int y0, int x1, int y1) {
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2; /* error value e_xy */

	for (;;) { /* loop */
		MAX_SetPixel(x0, y0,1);
		if (x0 == x1 && y0 == y1)
			break;
		e2 = 2 * err;
		if (e2 >= dy) {
			err += dy;
			x0 += sx;
		} /* e_xy+e_x > 0 */
		if (e2 <= dx) {
			err += dx;
			y0 += sy;
		} /* e_xy+e_y < 0 */
	}
}
void plot_circle(int xm, int ym, int r) {
	int x = -r, y = 0, err = 2 - 2 * r; /* II. Quadrant */
	do {
		MAX_SetPixel(xm - x, ym + y, 1); /*   I. Quadrant */
		MAX_SetPixel(xm - y, ym - x, 1); /*  II. Quadrant */
		MAX_SetPixel(xm + x, ym - y, 1); /* III. Quadrant */
		MAX_SetPixel(xm + y, ym + x, 1); /*  IV. Quadrant */
		r = err;
		if (r > x)
			err += ++x * 2 + 1; /* e_xy+e_x > 0 */
		if (r <= y)
			err += ++y * 2 + 1; /* e_xy+e_y < 0 */
	} while (x < 0);
}



void draw_char(int8_t x, int8_t y, uint8_t c) {
	uint8_t pix;
	for (uint8_t yy = 0; yy < FONT_CHAR_HEIGHT; yy++) {
		for (uint8_t xx = 0; xx < FONT_CHAR_WIDTH; xx++) {
			pix = FONT_BIT(c,xx,7-yy);
			MAX_SetPixel(xx+x, yy+y, pix);
		}
	}
	for (uint8_t yy = 0; yy < FONT_CHAR_HEIGHT; yy++) {
		MAX_SetPixel(5+x, yy+y, 0);
	}
}

void draw_text(int16_t x, int16_t y, uint8_t *t) {
	while (*t) {
		if(x>=64)return;
		if(x>-4)draw_char(x, y, *t);
		t++;
		x += 6;
	}
}



