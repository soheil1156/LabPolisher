#ifndef _graphic
#define _graphic

#include "type.h"
#include "GFONTS_S.h"

void GLCD_Rectangle(unsigned char x, unsigned char y, unsigned char b, unsigned char a);
void GLCD_Circle(unsigned char cx, unsigned char cy ,unsigned char radius);
void GLCD_Line(unsigned int X1,unsigned int Y1,unsigned int X2,unsigned int Y2);
void DrawLine(int x1, int y1, int x2, int y2, int color);
void GLCD_FILLRECT(char x,char y, char b, char a,char color);
void drawCharBitmap(const uint16_t xPixel, const uint16_t yPixel, uint16_t color, const uint8_t *glyph, uint8_t glyphHeightPages, uint8_t glyphWidthBits);
void drawString(uint16_t x, uint16_t y, uint16_t color, const FONT_INFO *fontInfo, char *str);

#endif 