#pragma once

#define MAX_NoChainedDisplays	(16)

void MAX_Init(void);
void MAX_Test(void);
void MAX_UpdateDisp(void);
void MAX_SetPixel(uint8_t x, uint8_t y, uint8_t c);

/* register map
0 noop
1-8  digits (led lines)
9 decode mode (0 = no decode)
a intensity (0-f)
b scan limit (0-7)
c shutdown (1 = normal operation)
f test

send 16bit packets

*/
