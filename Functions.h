/************GLOBAL FUNCTION******************
*				SOHEIL NAZARI
*				92 / 6 / 2
**********************************************/
#ifndef _Functions
#define _Functions

#include "type.h"



/****************FUNCTIONS********************
*
*********************************************/ 

char* itoa(int val, int base);
uint8_t LRC_CALC(uint8_t databyte[10]);

void draw_table(void);
void Dis_init(void);
void Set_backLight(uint16_t Backlight);


void Timer_update(void);



#endif