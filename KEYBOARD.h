/****************************************************************************
 *   SOHEIL NAZARI                       $
 *   Project: KEYBOARD CONTROL
 *
 *   Description:
 *   KEYBOARD CONTROL 16 MATRIX
 *
 ****************************************************************************
****************************************************************************/

	#ifndef _KEYBOARD
	#define _KEYBOARD

	#include "type.h"
	#include <lpc17xx.h>
	//Control PIN SELECT

	#define KEY_ROW_POS_1 21
	#define KEY_ROW_POS_2 22
	#define KEY_ROW_POS_3 23
	#define KEY_ROW_POS_4 24

	#define KEY_ROW_1 (1 << 21)
	#define KEY_ROW_2 (1 << 22)
	#define KEY_ROW_3 (1 << 23)
	#define KEY_ROW_4 (1 << 24)


	#define KEY_COL_POS_1 0
	#define KEY_COL_POS_2 1
	#define KEY_COL_POS_3 4
	#define KEY_COL_POS_4 8

	#define KEY_COL_1 (1 << 0)
	#define KEY_COL_2 (1 << 1)
	#define KEY_COL_3 (1 << 4)
	#define KEY_COL_4 (1 << 8)


	//CONTROL PORT

	#define KEY_ROW_DIR  LPC_GPIO0->FIODIR
	#define KEY_ROW_PORT LPC_GPIO0

	#define KEY_ROW_IN  KEY_ROW_DIR &=  ~(KEY_ROW_1 + KEY_ROW_2 + KEY_ROW_3 + KEY_ROW_4);
	#define KEY_ROW_OUT KEY_ROW_DIR |=   (KEY_ROW_1 + KEY_ROW_2 + KEY_ROW_3 + KEY_ROW_4);

    #define ROW1_SET    KEY_ROW_PORT -> FIOSET = KEY_ROW_1;
	#define ROW2_SET    KEY_ROW_PORT -> FIOSET = KEY_ROW_2;
	#define ROW3_SET    KEY_ROW_PORT -> FIOSET = KEY_ROW_3;
	#define ROW4_SET    KEY_ROW_PORT -> FIOSET = KEY_ROW_4;

    #define ROW1_CLR    KEY_ROW_PORT -> FIOCLR = KEY_ROW_1;
	#define ROW2_CLR    KEY_ROW_PORT -> FIOCLR = KEY_ROW_2;
	#define ROW3_CLR    KEY_ROW_PORT -> FIOCLR = KEY_ROW_3;
	#define ROW4_CLR    KEY_ROW_PORT -> FIOCLR = KEY_ROW_4;

	#define RAW_ALL_SET {ROW1_SET; ROW2_SET; ROW3_SET; ROW4_SET;}
	#define RAW_ALL_CLR {ROW1_CLR; ROW2_CLR; ROW3_CLR; ROW4_CLR;}

	#define KEY_COL_DIR  LPC_GPIO1->FIODIR
	#define KEY_COL_PORT LPC_GPIO1

	#define KEY_COL_IN  KEY_COL_DIR &=  ~(KEY_COL_1 + KEY_COL_2 + KEY_COL_3 + KEY_COL_4);
	#define KEY_COL_OUT KEY_COL_DIR |=   (KEY_COL_1 + KEY_COL_2 + KEY_COL_3 + KEY_COL_4);

    #define COL1_SET    KEY_COL_PORT -> FIOSET = KEY_COL_1;
	#define COL2_SET    KEY_COL_PORT -> FIOSET = KEY_COL_2;
	#define COL3_SET    KEY_COL_PORT -> FIOSET = KEY_COL_3;
	#define COL4_SET    KEY_COL_PORT -> FIOSET = KEY_COL_4;

    #define COL1_CLR    KEY_COL_PORT -> FIOCLR = KEY_COL_1;
	#define COL2_CLR    KEY_COL_PORT -> FIOCLR = KEY_COL_2;
	#define COL3_CLR    KEY_COL_PORT -> FIOCLR = KEY_COL_3;
	#define COL4_CLR    KEY_COL_PORT -> FIOCLR = KEY_COL_4;

	#define COL_ALL_SET { COL1_SET; COL2_SET; COL3_SET; COL4_SET }
	#define COL_ALL_CLR { COL1_CLR; COL2_CLR; COL3_CLR; COL4_CLR }


	void KEY_Init(void);
	uint8_t KEY_CHECK_POS(uint8_t row , uint8_t col);
	#endif
