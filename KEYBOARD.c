/****************************************************************************
 *   SOHEIL NAZARI                       $
 *   Project: KEYBOARD CONTROL SOURCE
 *
 *   Description:
 *   KEYBOARD CONTROL 16 MATRIX
 *
 ****************************************************************************
****************************************************************************/


	#include "type.h"
	#include "KEYBOARD.h"
	#include "GPIO.h"
	void KEY_Init()
	{
		GPIOSetPull(0,21,2);
		GPIOSetPull(0,22,2);
		GPIOSetPull(0,23,2);
		GPIOSetPull(0,24,2);

		GPIOSetPull(1,0,2);
		GPIOSetPull(1,1,2);
		GPIOSetPull(1,4,2);
		GPIOSetPull(1,8,2);

	}


	uint8_t KEY_CHECK_POS(uint8_t row , uint8_t col)

	{
		uint8_t s_row, s_col;
		uint8_t val_k = 0;
		char str[32];

		switch (col)
				{
				case 1 : s_col = KEY_COL_POS_1;
					break;

				case 2 : s_col = KEY_COL_POS_2;
						break;

				case 3 : s_col = KEY_COL_POS_3;
						break;

				case 4 : s_col = KEY_COL_POS_4;
						break;

				};
				switch (row)
					{
					case 1 : s_row = KEY_ROW_POS_1;
						break;

					case 2 : s_row = KEY_ROW_POS_2;
							break;

					case 3 : s_row = KEY_ROW_POS_3;
							break;

					case 4 : s_row = KEY_ROW_POS_4;
							break;

					};


			//FIND COLUMN PORT IS 0 - MAKE ROW INNPUT AND CHECK

				COL_ALL_CLR;
				// Delay for Line UP
					__asm("nop");
					__asm("nop");
					__asm("nop");
					__asm("nop");
					__asm("nop");
					__asm("nop");
					__asm("nop");
					__asm("nop");
					__asm("nop");

		KEY_ROW_IN;
		KEY_COL_OUT;

		//COL_ALL_SET;

		KEY_COL_PORT -> FIOSET |= (1<<s_col);

		// Delay for Line UP
		__asm("nop");
		__asm("nop");
		__asm("nop");
		__asm("nop");
		__asm("nop");
		__asm("nop");
		__asm("nop");
		__asm("nop");
		__asm("nop");

		    /*KEY_ROW_PORT->FIOMASK = ~(1<<s_row);
		    val_k = (KEY_ROW_PORT->FIOPIN >> s_row) ;

		    UART0_PrintString("RPL = ");
		    strcpy(str,itoa((KEY_ROW_PORT->FIOPIN >> s_row),2));
		    UART0_PrintString(str);
		    UART0_Sendchar(10);
		    UART0_PrintString("VAL1 = ");
		    UART0_PrintString(itoa(val_k,2));
		    UART0_Sendchar(10);*/


		    if ((GPIOGetValue(0,s_row) & 0x1) == 0)
		    {
		    	/*UART0_PrintString("RETURN 0");
		    	 UART0_Sendchar(10);*/
		    	RAW_ALL_CLR;
		    	COL_ALL_CLR;
		    		return 0;
		    }
		    else
		    {
		    	RAW_ALL_CLR;
		    	COL_ALL_CLR;
				__asm("nop");
				__asm("nop");
				__asm("nop");
				__asm("nop");
				__asm("nop");
				__asm("nop");
				__asm("nop");
				__asm("nop");
				__asm("nop");

		    	KEY_ROW_OUT;
		    	KEY_COL_IN;


		    	KEY_ROW_PORT -> FIOSET |= (1<<s_row);

		    	__asm("nop");
		    	__asm("nop");
		    	__asm("nop");
		    	__asm("nop");
		    	__asm("nop");
		    	__asm("nop");
		    	__asm("nop");
		    	__asm("nop");
		    	__asm("nop");

		    	/*val_k = 0;

		    	KEY_COL_PORT->FIOMASK = ~(1<<s_col);
		    	val_k = (KEY_COL_PORT->FIOPIN >> s_col);

		    	UART0_PrintString("CPL = ");
		    	strcpy(str, itoa((KEY_COL_PORT->FIOPIN >> s_col),2));
		    	UART0_PrintString(str);
		    	 UART0_Sendchar(10);
		    	UART0_PrintString("VAL2 = ");
		    	UART0_PrintString(itoa(val_k,2));
		        UART0_Sendchar(10);*/



		    
		    	if ((GPIOGetValue(1,s_col)& 0x1) == 1){

		    	/*	UART0_PrintString("RETURN 1");
		    	 UART0_Sendchar(10);*/
					RAW_ALL_CLR;
		    	COL_ALL_CLR;
		    	return 1 ; //key is pressed}
		    	}
		    	else{
				/*	UART0_PrintString("RETURN 2");
		    	 UART0_Sendchar(10);*/
						RAW_ALL_CLR;
		    	COL_ALL_CLR;
		    		return 0 ;

		    	}

		    	}

					
					




	}


