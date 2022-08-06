
#include "graphic.h"

extern void GLCD_SetPixel(unsigned char x, unsigned char y, unsigned char color);

const unsigned char color = 1;

void GLCD_Rectangle(unsigned char x, unsigned char y, unsigned char b, unsigned char a)
{
  unsigned char j; // zmienna pomocnicza
  // rysowanie linii pionowych (boki)
  for (j = 0; j < a; j++) {
		GLCD_SetPixel(x, y + j, color);
		GLCD_SetPixel(x + b - 1, y + j, color);
	}
  // rysowanie linii poziomych (podstawy)
  for (j = 0; j < b; j++)	{
		GLCD_SetPixel(x + j, y, color);
		GLCD_SetPixel(x + j, y + a - 1, color);
	}
}
//

void GLCD_Circle(unsigned char cx, unsigned char cy ,unsigned char radius)
{
int x, y, xchange, ychange, radiusError;
x = radius;
y = 0;
xchange = 1 - 2 * radius;
ychange = 1;
radiusError = 0;
while(x >= y)
  {
  GLCD_SetPixel(cx+x, cy+y, color); 
  GLCD_SetPixel(cx-x, cy+y, color); 
  GLCD_SetPixel(cx-x, cy-y, color);
  GLCD_SetPixel(cx+x, cy-y, color); 
  GLCD_SetPixel(cx+y, cy+x, color); 
  GLCD_SetPixel(cx-y, cy+x, color); 
  GLCD_SetPixel(cx-y, cy-x, color); 
  GLCD_SetPixel(cx+y, cy-x, color); 
  y++;
  radiusError += ychange;
  ychange += 2;
  if ( 2*radiusError + xchange > 0 )
    {
    x--;
	radiusError += xchange;
	xchange += 2;
	}
  }
}
//

void GLCD_Line(unsigned int X1,unsigned int Y1,unsigned int X2,unsigned int Y2)
{
int CurrentX, CurrentY, Xinc, Yinc, 
    Dx, Dy, TwoDx, TwoDy, 
	TwoDxAccumulatedError, TwoDyAccumulatedError;

Dx = (X2-X1); // obliczenie sk³adowej poziomej
Dy = (Y2-Y1); // obliczenie sk³adowej pionowej

TwoDx = Dx + Dx; // podwojona sk³adowa pozioma
TwoDy = Dy + Dy; // podwojona sk³adowa pionowa

CurrentX = X1; // zaczynamy od X1
CurrentY = Y1; // oraz Y1

Xinc = 1; // ustalamy krok zwiêkszania pozycji w poziomie 
Yinc = 1; // ustalamy krok zwiêkszania pozycji w pionie

if(Dx < 0) // jesli sk³adowa pozioma jest ujemna 
  {
  Xinc = -1; // to bêdziemy siê "cofaæ" (krok ujemny)
  Dx = -Dx;  // zmieniamy znak sk³adowej na dodatni
  TwoDx = -TwoDx; // jak równie¿ podwojonej sk³adowej
  }

if (Dy < 0) // jeœli sk³adowa pionowa jest ujemna
  {
  Yinc = -1; // to bêdziemy siê "cofaæ" (krok ujemny)
  Dy = -Dy; // zmieniamy znak sk³adowej na dodatki
  TwoDy = -TwoDy; // jak równiez podwojonej sk³adowej
  }

GLCD_SetPixel(X1,Y1, color); // stawiamy pierwszy krok (zapalamy pierwszy piksel)

if ((Dx != 0) || (Dy != 0)) // sprawdzamy czy linia sk³ada siê z wiêcej ni¿ jednego punktu ;)
  {
  // sprawdzamy czy sk³adowa pionowa jest mniejsza lub równa sk³adowej poziomej
  if (Dy <= Dx) // jeœli tak, to idziemy "po iksach"
    { 
    TwoDxAccumulatedError = 0; // zerujemy zmienn¹ 
    do // ruszamy w drogê
	  {
      CurrentX += Xinc; // do aktualnej pozycji dodajemy krok 
      TwoDxAccumulatedError += TwoDy; // a tu dodajemy podwojon¹ sk³adow¹ pionow¹
      if(TwoDxAccumulatedError > Dx)  // jeœli TwoDxAccumulatedError jest wiêkszy od Dx
        {
        CurrentY += Yinc; // zwiêkszamy aktualn¹ pozycjê w pionie
        TwoDxAccumulatedError -= TwoDx; // i odejmujemy TwoDx
        }
       GLCD_SetPixel(CurrentX,CurrentY, color);// stawiamy nastêpny krok (zapalamy piksel)
       }while (CurrentX != X2); // idziemy tak d³ugo, a¿ osi¹gniemy punkt docelowy
     }
   else // w przeciwnym razie idziemy "po igrekach" 
      {
      TwoDyAccumulatedError = 0; 
      do 
	    {
        CurrentY += Yinc; 
        TwoDyAccumulatedError += TwoDx;
        if(TwoDyAccumulatedError>Dy) 
          {
          CurrentX += Xinc;
          TwoDyAccumulatedError -= TwoDy;
          }
         GLCD_SetPixel(CurrentX,CurrentY, color); 
         }while (CurrentY != Y2);
    }
  }
}
////////////////////////////////////////////////////////////////


void DrawLine(int x1, int y1, int x2, int y2, int color)
{
	int i, deltax, deltay, numpixels;
	int d, dinc1, dinc2;
	int x, xinc1, xinc2;
	int y, yinc1, yinc2;
	//calculate deltaX and deltaY
	deltax = fabs(x2 - x1);
	deltay = fabs(y2 - y1);

	//initialize
	if(deltax >= deltay)
	{
	//If x is independent variable
	numpixels = deltax + 1;
	d = (2 * deltay) - deltax;
	dinc1 = deltay << 1;
	dinc2 = (deltay - deltax) << 1;
	xinc1 = 1;
	xinc2 = 1;
	yinc1 = 0;
	yinc2 = 1;
	}
	else
	{
	//if y is independent variable
	numpixels = deltay + 1;
	d = (2 * deltax) - deltay;
	dinc1 = deltax << 1;
	dinc2 = (deltax - deltay) << 1;
	xinc1 = 0;
	xinc2 = 1;
	yinc1 = 1;
	yinc2 = 1;
	}

	//move the right direction
	if(x1 > x2)
	{
	xinc1 = -xinc1;
	xinc2 = -xinc2;
	}
	if(y1 > y2)
	{
	yinc1 = -yinc1;
	yinc2 = -yinc2;
	}

	x = x1;
	y = y1;
	//draw the pixels
	//_color = color;
	for(i = 1; i < numpixels; i++)
	{
	GLCD_SetPixel(x, y,color);
	if(d < 0)
	{
		d = d + dinc1;
		x = x + xinc1;
		y = y + yinc1;
	}
	else
	{
		d = d + dinc2;
		x = x + xinc2;
		y = y + yinc2;
	}
	}
}


/////////////////////////////////////////////////////////////

void GLCD_FILLRECT(char x,char y, char b, char a,char color)
{
unsigned char i, j;
for(j = y/8; j < (y + a)/8; j++)
  {
  GLCD_GoTo(x,j);
  for(i = x; i < (x+b); i++)
    GLCD_WriteData(color);
}
}
//
void drawCharBitmap(const uint16_t xPixel, const uint16_t yPixel, uint16_t color, const uint8_t *glyph, uint8_t glyphHeightPages, uint8_t glyphWidthBits)
{
 uint16_t verticalPage, horizBit, currentY, currentX;
  uint16_t indexIntoGlyph;

  // set initial current y
  currentY = yPixel;
  currentX = xPixel;

  // for each page of the glyph
  for (verticalPage = glyphHeightPages; verticalPage > 0; --verticalPage)
  {
    // for each horizontol bit
    for (horizBit = 0; horizBit < glyphWidthBits; ++horizBit)
    {
      // next byte
      indexIntoGlyph = (glyphHeightPages * horizBit) + verticalPage - 1;

      currentX = xPixel + (horizBit);
      // send the data byte
      if (glyph[indexIntoGlyph] & (0X80)) GLCD_SetPixel(currentX, currentY, color);
      if (glyph[indexIntoGlyph] & (0X40)) GLCD_SetPixel(currentX, currentY - 1, color);
      if (glyph[indexIntoGlyph] & (0X20)) GLCD_SetPixel(currentX, currentY - 2, color);
      if (glyph[indexIntoGlyph] & (0X10)) GLCD_SetPixel(currentX, currentY - 3, color);
      if (glyph[indexIntoGlyph] & (0X08)) GLCD_SetPixel(currentX, currentY - 4, color);
      if (glyph[indexIntoGlyph] & (0X04)) GLCD_SetPixel(currentX, currentY - 5, color);
      if (glyph[indexIntoGlyph] & (0X02)) GLCD_SetPixel(currentX, currentY - 6, color);
      if (glyph[indexIntoGlyph] & (0X01)) GLCD_SetPixel(currentX, currentY - 7, color);
    }
    // next line of pages
    currentY += 8;
  }
}
void drawString(uint16_t x, uint16_t y, uint16_t color, const FONT_INFO *fontInfo, char *str)
{
  uint16_t currentX, charWidth, characterToOutput;
  const FONT_CHAR_INFO *charInfo;
  uint16_t charOffset;

  // set current x, y to that of requested
  currentX = x;

  // while not NULL
  while (*str != '\0')
  {
    // get character to output
    characterToOutput = *str;

    // get char info
    charInfo = fontInfo->charInfo;

    // some fonts have character descriptors, some don't
    if (charInfo != NULL)
    {
      // get correct char offset
      charInfo += (characterToOutput - fontInfo->startChar);

      // get width from char info
      charWidth = charInfo->widthBits;

      // get offset from char info
      charOffset = charInfo->offset;
    }
    else
    {
      // if no char info, char width is always 5
      charWidth = 5;

      // char offset - assume 5 * letter offset
      charOffset = (characterToOutput - fontInfo->startChar) * 5;
    }

    // Send individual characters
    drawCharBitmap(currentX, y + 1, color, &fontInfo->data[charOffset], fontInfo->heightPages, charWidth);

    // next char X
    currentX += charWidth + 1;

    // next char
    str++;
  }
}
