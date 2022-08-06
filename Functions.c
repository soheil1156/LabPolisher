/************GLOBAL FUNCTION****************************************
*				SOHEIL NAZARI
*				92 / 6 / 2
*******************************************************************/ 
#ifndef _Functions_c
#define _Functions_c

//#define in_line_debuge_F


#include "LPC17xx.h"
#include "type.h"
#include "string.h"
#include "stdint.h"
#include "UART.h"
#include "STEPMOTOR.h"
#include "KEYBOARD.h"

#define PWM_SET 1
#define PWM_NOTSET 0

#define TIME0_NOT_INIT 0
#define TIME0_STOP 1
#define TIME0_RUN 2

#define Motor_Nominal_Speed 940.F
#define Motor_Max_Speed 1200.0F
#define Motor_Min_Speed 150.0F

#define Motor_Max_Freq  7500 //( 60.0 / Motor_Nominal_Speed ) * Motor_Max_Speed
#define Motor_Min_Freq  250 //( 60.0 / Motor_Nominal_Speed ) * Motor_Min_Speed

#define MOTOR_STOP 0
#define MOTOR_RUN  1

#define MOTOR_DIR_FORWARD  1
#define MOTOR_DIR_REVERSE  0

#define debuge
#define in_line_debuge

#define FLAGDOWN 0
#define FLAGUP   1

#define Stepper_Max_Angle 90
#define Stepper_Mix_Angle 0
#define Min_Step_Angle    3.2
/**********************GDLOBAL VARIBLE********************************
*
*********************************************************************/ 

_Bool    Timer0Flag          = FLAGDOWN;
_Bool    ChengeFrequncyFlag  = FLAGDOWN;
_Bool    ChangeStateFlag     = FLAGDOWN;
_Bool    ChangeDirFlag       = FLAGDOWN;
_Bool    CheckCurrentFlag    = FLAGDOWN;
_Bool    CheckFlowFlag		 = FLAGDOWN;
_Bool    Motor_State         = MOTOR_STOP;
_Bool    Motor_Dir_State     = MOTOR_DIR_FORWARD;
_Bool    Stepper_Direction;
_Bool    Flow_State;
_Bool    Stop_Frame;
_Bool    Water_State = 0;

uint8_t  PWM_STATE = 0 ;
uint8_t  Timer_State = 0 ;
uint8_t  Sec,Min,Hour;
uint8_t  DATA[10] ={0};
uint8_t  Read_Motor_Interval = 0 ;
uint16_t NewFreq,OldFreq,Freq = 250;
uint8_t  Anim_Rotate_Interval,Anim_Rotate_Last;
uint32_t Newtime,Oldtime;
uint16_t Motor_Current_New,Motor_Current_Old;
int16_t  Step_Pos_New,Step_Pos_Old;
uint8_t  NewFlow,Flow = 0;
uint16_t BacklightIntensity = 5000;

/****************************CONVERT INT TO STRING********************/
char* itoa(int val, int base){
	
	static char buf[32] = {0};
	
	int i = 30;
	
	for(; val && i ; --i, val /= base)
	
		buf[i] = "0123456789ABCDEF"[val % base];
	
	return &buf[i+1];

}


/************CONVERT INT TO STRING CHARACHTER COUNT ********************/
char* itoa_e(int val, int base , uint8_t count){
	
	static char buf[32] = {0};

	int i = 30;
	int j;
	int k = 1;
	/*for (j = 0 ; j < count ; j++)
		k = k * 10;
	val = val % k;*/
	
#ifdef in_line_debuge_F
		UART0_Sendchar(10);
		UART0_Sendchar(10);
		UART0_PrintString(" !!!!  ITOE CALL  !!!! ");
		UART0_Sendchar(10);
		UART0_Sendchar(10);
	#endif


	for(; val && i ; --i, --count , val /= base)
	
		buf[i] = "0123456789ABCDEF"[val % base];
	if (count  == 0  ){

		#ifdef in_line_debuge_F
			UART0_Sendchar(10);
			UART0_Sendchar(10);
			UART0_PrintString("COUNT ITOE CHECK  = ");
			UART0_PrintString(&buf[i+1]);
			UART0_Sendchar(10);
			UART0_Sendchar(10);
		#endif
		return &buf[i+1];

	}

	while (count)
	{
		count--;
		--i;
		buf[i+1] = '0';
		#ifdef in_line_debuge_F
		UART0_Sendchar(10);
		UART0_PrintString("WHILE ITOE COUNT CALL  = ");
		UART0_PrintString(&buf[i+1]);
		UART0_Sendchar(10);
		UART0_Sendchar(10);
		#endif
	}
	return &buf[i+1];

}
/************************CONVERT FLOAT TO STRING NAZARI VERSION**********************/

#include "string.h"
/*
 *
*********************************************************************************************************/

void ftoa_e(unsigned char *buf, float f,uint8_t Decimal) {

	int i;
	int FTmp = f;
	int ITmp;
	ITmp = FTmp;

	ITmp = ITmp % 10;

	strcpy(buf,itoa_e(ITmp,10,1));
	strcat(buf,".");
	FTmp = f;
	for (i = 0; i < Decimal ; i++)
	{
	f = f * 10.0;
	FTmp = FTmp * 10;
	}
	ITmp = f - FTmp;
	strcat(buf,itoa_e(ITmp,10,1));
}








/************************CONVERT FLOAT TO STRING**********************/

#include "string.h"
/*
*********************************************************************************************************
*                        CONVERT A FLOATING POINT NUMBER TO STRING WITH 1 DECIMAL PLACE
*
* Description :	This function converts a floating point to a null terminated string 
*				with 1 decimal place.
*			
*				Examples:
*					
*				float f = 9.567;
*				ftoa(&s[0], f);		//s[]={'9','.','5', 0}
*				float f = -0.189;
*				ftoa(&s[0], f);  	//s[]={'-', '0', '.', '1', 0}
* Arguments   : 'unsigned char* buf' 	is the pointer to the string holding the conversion result
*				'float f'				is the input floating point
* Returns     : Returns the string with unsigned char* buf pointing to.
* Notes		  : This routine modified from itoa10() in ..\sample\misc folder of ht-picc
*				If more decimal places required, modify the last section of this code
*				Range of f in (-3,276.7, 3,276.7)
*				This function does print result like 0.0.
*********************************************************************************************************
*/
void ftoa(unsigned char *buf, float f) {
	unsigned int rem;
	unsigned char *s,length=0;
	int i;

	i = (int)((float)f*10);

	s = buf;
	if (i == 0){ 		//print 0.0 with null termination here
		*s++ = '0';
		*s++ = '.';
		*s++ = '0'; 
		*s=0; 			//null terminate the string
	} else {	
		if (i < 0) {
			*buf++ = '-';
			s = buf;
			i = -i;
		}
		//while-loop to "decode" the long integer to ASCII by append '0', string in reverse manner
		//If it is an integer of 124 -> string = {'4', '2', '1'}
		while (i) {
			++length;
			rem = i % 10;
			*s++ = rem + '0';
			i /= 10;
		}
		//reverse the string in this for-loop, string became {'1', '2', '4'} after this for-loop
		for(rem=0; ((unsigned char)rem)<length/2; rem++) {
			*(buf+length) = *(buf+((unsigned char)rem));
			*(buf+((unsigned char)rem)) = *(buf+(length-((unsigned char)rem)-1));
			*(buf+(length-((unsigned char)rem)-1)) = *(buf+length);
		}

		/* Take care of the special case of 0.x if length ==1*/	
		if(length==1) {
			*(buf+2) = *buf;
			*buf = '0';
			*(buf+1) = '.';
			*(s+2)=0; 			//null terminate
		} else {
			*(buf+length) = *(buf+length-1);
			*(buf+length-1)='.';
			*(s+1)=0; 			//null terminate
		}
	}
}

/************************LRC GENERATE**********************/
	uint8_t LRC_CALC(uint8_t databyte[10])
	{
		int i;
		uint8_t LRC_TMP =0;
		for (i = 0; i < 10 ; i++)
		{
			LRC_TMP += databyte[i] ;
		}
// 		LRC_TMP ^= 0xFF;
// 		LRC_TMP++;
		return (-LRC_TMP );
  }
  
/**********************LRC GENERATE STRING******************/
	uint8_t LRC_CALC_STRING(char *buf)
	{
		uint8_t databyte[10] = {0};
		uint8_t i;
		uint8_t LRC_TMP = 0;
		char tmpstr[2];
		
	  #ifdef in_line_debuge_F
		UART0_PrintString("LRC CHECK 1 CALL = ");
		UART0_PrintString(buf);
		UART0_Sendchar(10);
	  #endif
		
		if (*buf != ':')
			return 0;
		for (i = 0; i < 6; i++)
		{
			tmpstr[0] = *(buf+2 * i + 1);
			tmpstr[1] = *(buf+2 * i + 2);
			databyte[i] =(uint8_t)strtol(tmpstr,NULL,16);
			#ifdef in_line_debuge_F
				UART0_PrintString("LRC CHECK DATABYTE = ");
				UART0_PrintString(itoa(databyte[i],10));
				UART0_Sendchar(10);
			#endif
		}
		for (i = 0; i < 6; i++)
		{
			LRC_TMP += databyte[i];
		}
		 		LRC_TMP ^= 0xFF;
		 		LRC_TMP++;
			#ifdef in_line_debuge_F
				UART0_PrintString("LRC VALUE = ");
				UART0_PrintString(itoa((uint8_t)LRC_TMP,16));
				UART0_Sendchar(10);
			#endif

		return (LRC_TMP);

	}

/***********************Draw TABLE***************************/
void draw_table()
{
	    DrawLine(0,0,128,0,0x01);
			DrawLine(0,21,128,21,0x01);
			DrawLine(0,42,43,42,0x01);
			DrawLine(0,0,0,64,0x01);
			DrawLine(43,0,43,64,0x01);
			DrawLine(0,63,127,63,0x01);
			DrawLine(127,0,127,64,0x01);
}

void Dis_init()
{
		GLCD_ClearScreen();
		GLCD_ClearScreen();
		drawCharBitmap(0, 7, 1, (uint8_t *)tMCBitmaps, 8, 127);
		DelayMs(2500);
}
void Set_backLight(uint16_t Backlight)
{
			int i;
	if (Backlight > 8000)
		Backlight= 8000;
	//PWM SET
	if (PWM_STATE == PWM_NOTSET){
		PWM_Init(1,6,8000);
	PWM_STATE = PWM_SET;
	}
	  PWM_Set(1,6,Backlight);
		PWM_Start(1);
		
		/*for (i = 0 ; i <= Backlight;  i= i+20)
		{
			PWM_Set(1,2,i);
			DelayMs(2);
		}*/


#ifdef debuge

		 UART0_PrintString("BACKLIGHT = ");
		 UART0_PrintString(itoa(Backlight,10));
		 UART0_Sendchar(10);
#endif
}
void Timer_update()
{
	if (Timer_State == TIME0_NOT_INIT )
	{
		
		drawString(66, 6, 1, &arial_10ptFontInfo,"00 : 00 : 00");
		Timer_State = TIME0_STOP;
	}
	if (Timer_State == TIME0_RUN ){
	Sec++;
	if (Sec > 59)
	{
		GLCD_FILLRECT(112,5,14,11,0x00);
		GLCD_FILLRECT(89,5,14,11,0x00);
		drawString(112, 6, 1, &arial_10ptFontInfo,"00");
		Min++;
		Sec = 0;
				if (Min > 59){
					Hour++;
					Min = 0;
					drawString(89, 6, 1, &arial_10ptFontInfo,"00");
				}
				else
				{
					drawString(89, 6, 1, &arial_10ptFontInfo,itoa_e(Min,10,2));
					GLCD_GoTo(0,0);	
				}
			}
	else{
		GLCD_FILLRECT(112,5,14,11,0x00);

			drawString(112, 6, 1, &arial_10ptFontInfo,itoa_e(Sec,10,2));
		} 
	

DrawLine(42,0,128,0,0x01);
}
	
}

/****************REFRESH BACKGROUND***********************
*
**********************************************************/ 

void RefreshBackground()
{
		   draw_table();
		   GLCD_GoTo(0,0);
			drawString(66, 6, 1, &arial_10ptFontInfo,"00 : 00 : 00");
			drawCharBitmap(45, 10, 1, (uint8_t *)CLOCKBitmaps, 2, 17);
			GLCD_GoTo(0,0);
			drawCharBitmap(2, 10, 1, (uint8_t *)amperBitmaps, 2, 16);
			drawString(34, 27, 1, &arial_10ptFontInfo, "%");
			GLCD_GoTo(0,0);			
		  drawString(52, 51, 1, &arial_10ptFontInfo, "RPM");
}

/********************Graphic RpmUpdate*********************
*
**********************************************************/ 

void Rpm_update(uint16_t Motor_Frequency)
{
		uint16_t Rpm_Speed = 0; 
		Rpm_Speed = (int)(Motor_Nominal_Speed / 100.0 * ((float)Motor_Frequency) / 60.0);
		GLCD_FILLRECT(52,30,50,24,0x00);
		drawString(52, 30, 1, &arial_18ptFontInfo, itoa_e(Rpm_Speed,10,4));

}

/************Graphic STOP SHOW ANIMATION******************
*
*********************************************************/
void Stop_Show_Update(uint8_t Frame)
{
	GLCD_FILLRECT(105,30,22,32,0x00);
	if (Frame)
		drawCharBitmap(108, 40, 1, (uint8_t *)sTOPBitmaps, 2, 14);
	return;
}



/************Graphic CCW ARROW ANIMATION******************
*
*********************************************************/
void Anim_Rotate0_update(uint8_t FrameNumber)
{
		    GLCD_FILLRECT(105,30,22,32,0x00);
			switch(FrameNumber%8){
				
		    case 0 : 
				
				drawCharBitmap(108, 40, 1, (uint8_t *)rotate_0Bitmaps, 2, 14);
				
				break;
				case 1 : 
				drawCharBitmap(108, 32, 1, (uint8_t *)rotate_1Bitmaps, 3, 14);
				break;
				
				case 2 : 
				drawCharBitmap(108, 40, 1, (uint8_t *)rotate_2Bitmaps, 2, 14);
				break;
				case 3 :
				
				drawCharBitmap(108, 40, 1, (uint8_t *)rotate_3Bitmaps, 2, 17);
				
				break;
				case 4 :
				
				drawCharBitmap(108, 40, 1, (uint8_t *)rotate_4Bitmaps, 2, 14);
				
				
				break;
				case 5 :

				drawCharBitmap(108, 35, 1, (uint8_t *)rotate_5Bitmaps, 3, 14);
				
				break;
				case 6 :
				
				drawCharBitmap(108, 40, 1, (uint8_t *)rotate_6Bitmaps, 2, 14);
				
				break;
				
				case 7 :
				
				drawCharBitmap(105, 40, 1, (uint8_t *)rotate_7Bitmaps, 2, 17);
				
				break;
				
				default : 
				break;
			}
			
}
void Water_Flow_Update(uint8_t NewFlow)
{
	uint8_t AnimFrame;
	if (NewFlow > 100)
		NewFlow = 100;
	AnimFrame = NewFlow / 20;
	GLCD_FILLRECT(2,30,32,11,0x00);
	switch (AnimFrame)
		{
		case 0  :		 /* Frame0 */
						drawCharBitmap(2, 31, 1, (uint8_t *)wATER_0Bitmaps, 2, 11);
						break;
		case 1  :		 /* Frame1 */
						drawCharBitmap(2, 31, 1, (uint8_t *)wATER_20Bitmaps, 2, 11);
						break;
		case 2  :		 /* Frame2 */
						drawCharBitmap(2, 31, 1, (uint8_t *)wATER_40Bitmaps, 2, 11);
						break;				
		case 3  :		 /* Frame3 */
						drawCharBitmap(2, 31, 1, (uint8_t *)wATER_60Bitmaps, 2, 11);
						break;
		case 4 :		 /* Frame4 */
						drawCharBitmap(2, 31, 1, (uint8_t *)wATER_80Bitmaps, 2, 11);
						break;
		case 5 :		 /* Frame5 */
						drawCharBitmap(2, 31, 1, (uint8_t *)wATER_100Bitmaps, 2, 11);
						break;
		//case default :
					//	break;
						}
	if (NewFlow < 10 )
		{
			drawString(18, 27, 1, &arial_10ptFontInfo, "0");
			drawString(26, 27, 1, &arial_10ptFontInfo, itoa(NewFlow , 10));
		//	drawString(34, 27, 1, &arial_10ptFontInfo, "%");
		}
	else if(NewFlow < 100)
		{
			drawString(18, 27, 1, &arial_10ptFontInfo, itoa(NewFlow , 10));
			//drawString(34, 27, 1, &arial_10ptFontInfo, "%");
		}
			else {drawString(14, 27, 1, &arial_10ptFontInfo, "100");
				//	drawString(34, 27, 1, &arial_10ptFontInfo, "%");
				}
		
}


void Current_Show_Update(float NowCurrent)
{
	
	unsigned char str2[5]={0};
	//ftoa(((float)NowCurrent) / 10.0,1, *str);
	#ifdef in_line_debuge_F
			
				 UART0_PrintString("FTOA = ");
				 ftoa(&str2[0],NowCurrent);
			  UART0_PrintString(str2);
				UART0_Sendchar(10); 
			
			#endif
	GLCD_FILLRECT(19,1,20,16,0x00);
	ftoa_e(&str2[0],NowCurrent,1);
	drawString(21, 6, 1, &arial_10ptFontInfo, str2);
	DrawLine(0,0,42,0,0x01);
}
void Motor_Direction_update(_Bool Direction)
{
	GLCD_FILLRECT(1,52,41,16,0x00);
	DrawLine(0,63,42,63,0x01);
	if (Direction)
		{
			
			drawCharBitmap(2, 57, 1, (uint8_t *)cIRCLE_CWBitmaps, 1, 8);
			drawString(11, 48, 1, &arial_10ptFontInfo, "CW");	
		}
	else
		{
			drawCharBitmap(2, 57, 1, (uint8_t *)cIRCLE_CCWBitmaps, 1, 8);
			drawString(11, 48, 1, &arial_10ptFontInfo, "CCW");	
		}
}

/* ********************************************************
 *
 * 	Function : READ MOTOR SPEED
 *
 * ********************************************************/
uint16_t Read_Motor_Speed(uint8_t Address,_Bool SetOrCurrent)
{
	char  str[32] = {0};
	char tmpstr[10] = {0};
	uint8_t Operation_Data_Byte = 0x00;
	uint8_t LRC_TMP = 0x00 ;
	LPC_GPIO0->FIODIR |= (1 << 0 );
	LPC_GPIO0->FIOSET |= (1 << 0 );



	str[0] = ':';
	strcat(str,itoa_e(Address%10,16,2));
	
	strcat(str,"03");   /*Read Data Function Code*/
	if (SetOrCurrent == 0 )
	strcat(str,"2523"); /*Read FREQUENCY Code*/
	else
	strcat(str,"2524"); /*Read SET Code*/

	strcat(str,"0001"); /*Read SART Code*/

	LRC_TMP = LRC_CALC_STRING(&str[0]);

	strcat(str ,itoa_e(LRC_TMP,16,2));

	tmpstr[0] = 0x0D;
	tmpstr[1] = 0x0A;
	strcat(str,tmpstr);
	/*END OPERATION Code*/

		#ifdef in_line_debuge_F
			
				UART0_PrintString("MOTOR FIRST PHARSE = ");
			  UART0_PrintString(str);
				UART0_Sendchar(10); 
			
		#endif

	/***********************Read Data Start ********************/
				UART1_PrintString(str);
				LPC_GPIO0->FIODIR |= (1 << 0 );
				DelayMs(2);
				LPC_GPIO0->FIOCLR |= (1 << 0 );

				strcpy(str,"");
				Read_Motor_Interval = 0;
		{
				char i = 0;
				char Read_Data[5] = {0};
				while(1)
					{
						if (Read_Motor_Interval == 1)
							break;
						if(LPC_UART1->LSR & 0x01)  // Nothing received so just block
							{
								str[i] = LPC_UART1->RBR;
								i++;
							#ifdef in_line_debuge_F
								UART0_PrintString("CHARECTER RECIEVED = ");
								UART0_PrintString(str);
								UART0_Sendchar(10);
							#endif
							}
					}
				if (str[0] != ':')
					{
						#ifdef in_line_debuge_F
							UART0_PrintString("Motor Read ERROR = ");
							UART0_PrintString(str);
							UART0_Sendchar(10);
						#endif
						return 0;
					}
				Read_Data[0] = str[7];
				Read_Data[1] = str[8];
				Read_Data[2] = str[9];
				Read_Data[3] = str[10];
				Read_Data[4] = 0;
						#ifdef in_line_debuge_F
							UART0_PrintString("Motor Read SPEED = ");
							UART0_PrintString(Read_Data);
							UART0_Sendchar(10);
							UART0_PrintString("PRINT DATA STRTOL = ");
							UART0_PrintString(itoa((uint16_t)strtol(Read_Data,NULL,16),10));
							UART0_Sendchar(10);
						#endif
				return ((uint16_t)strtol(Read_Data,NULL,16));
				}

}

/* **************************************************************
 *
 * 							SET MOTOR SPEED
 *
****************************************************************/

uint16_t Set_Motor_Speed(uint8_t Address,uint16_t Speed)
{
		char  str[32] = {0};
		char tmpstr[10] = {0};
		uint8_t LRC_TMP;
		LPC_GPIO0->FIODIR |= (1 << 0 );
	    LPC_GPIO0->FIOSET |= (1 << 0 );

/* **********************************************
************** CHECK SPEED MAX MIN **************
************************************************/
	    if ( Speed   > 7500)   /*Motor_Nominal_Speed * Speed / 50*/
	    	Speed =  7500;//Motor_Max_Speed * 50 * 100/ Motor_Nominal_Speed ;
	    if ( Speed  < 10)//  Motor_Nominal_Speed * Speed / 50 )
	    	Speed = 10; //Motor_Min_Speed * 100 * 50 / Motor_Nominal_Speed ;


	#ifdef in_line_debuge_F
		UART0_PrintString("MOTOR SET INIT PHARSE CKECK = ");
		UART0_PrintString(str);
		UART0_Sendchar(10);
	#endif

		str[0] = ':';
		strcpy(tmpstr,itoa(Address%10,10));

		if (Address < 10)
		{
			str[1]= '0';
			str[2] = tmpstr[0];
		}
		else
		{
			strcat(str,tmpstr);
		}

		strcat(str,"06");   /*WRITE Data Function Code*/

		strcat(str,"2502"); /*ADRESS  FREQUENCY Code*/
		
		#ifdef in_line_debuge_F
		UART0_PrintString("MOTOR SET PRE PHARSE CKECK = ");
		UART0_PrintString(str);
		UART0_Sendchar(10);
	   #endif

		strcpy(tmpstr,itoa_e(Speed,16,4));	/*SPEED FREQUENCY Code*/

		strcat(str,tmpstr);

	#ifdef in_line_debuge_F
		UART0_PrintString("MOTOR SET FIRST PHARSE CKECK = ");
		UART0_PrintString(str);
		UART0_Sendchar(10);
	#endif
		LRC_TMP = LRC_CALC_STRING(&str[0]);
		strcpy(tmpstr ,itoa(LRC_TMP,16));

		if (LRC_TMP < 17)
		{
			char tmp2;
			tmp2  = tmpstr[0];
			tmpstr[0]= '0';
			tmpstr[1]= tmp2;
		}
		strcat(str,tmpstr);
		tmpstr[0] = 0x0D;
		tmpstr[1] = 0x0A;
		strcat(str,tmpstr); 	/*END FREQUENCY Code*/
	#ifdef in_line_debuge_F
		UART0_PrintString("MOTOR SET END PHARSE CKECK = ");
		UART0_PrintString(str);
		UART0_Sendchar(10);
	#endif
		UART1_PrintString(str);
		LPC_GPIO0->FIODIR |= (1 << 0 );
	   // LPC_GPIO0->FIOCLR |= (1 << 0 );

}
/* ********************************************************
 *
 * 	Function : MOTOR RUN,STOP,DIRECTION
 *
 * ********************************************************/

uint8_t Motor_Operation_Set(uint8_t Address,_Bool Start , _Bool Direction)
{
			char  str[32] = {0};
			char tmpstr[10] = {0};
			uint8_t Operation_Data_Byte = 0x00;
			uint8_t LRC_TMP = 0x00 ;
			LPC_GPIO0->FIODIR |= (1 << 0 );
		    LPC_GPIO0->FIOSET |= (1 << 0 );

			str[0] = ':';
			strcpy(tmpstr,itoa(Address%10,10));

			if (Address < 10)
			{
				str[1]= '0';
				str[2] = tmpstr[0];
			}
			else
			{
				strcat(str,tmpstr);
			}

			strcat(str,"06");   /*WRITE Data Function Code*/

			strcat(str,"2501"); /*ADRESS  START/STOP  Code*/

			/*START Data Function Code*/

			if (Start)
				Operation_Data_Byte |=  (1 << 0) ;
			else
				Operation_Data_Byte &= ~(1 << 0) ;
			/*STOP Data Function Code*/
			if (Direction)
				Operation_Data_Byte |=  (1 << 1) ;
			else
				Operation_Data_Byte &= ~(1 << 1) ;

			/*DATA SET Code*/
			strcpy(tmpstr,itoa_e(Operation_Data_Byte,16,4));

			strcat(str,tmpstr);

			LRC_TMP = LRC_CALC_STRING(&str[0]);
					strcpy(tmpstr ,itoa(LRC_TMP,16));

					if (LRC_TMP < 16)
					{
						char tmp2;
						tmp2  = tmpstr[0];
						tmpstr[0]= '0';
						tmpstr[1]= tmp2;
					}
					strcat(str,tmpstr);
					tmpstr[0] = 0x0D;
					tmpstr[1] = 0x0A;
					strcat(str,tmpstr); 	/*END OPERATION Code*/
				#ifdef in_line_debuge_F
					UART0_PrintString("MOTOR OPERATION END PHARSE CKECK = ");
					UART0_PrintString(str);
					UART0_Sendchar(10);
				#endif

						UART1_PrintString(str);
						LPC_GPIO0->FIODIR |= (1 << 0 );
						//LPC_GPIO0->FIOCLR |= (1 << 0 );




}


/* ********************************************************
 *
 * 	Function : Water On/Off
 *
 * ********************************************************/

void T_Water()
{
	GPIOSetDir(0,5,1);
	if (Water_State == 1)
		GPIOSetValue(0,5,1);
	else
		GPIOSetValue(0,5,0);

		
}

void Uart_Command_Init()
{
					UART0_Sendchar(10);
					UART0_Sendchar(10);
					UART0_PrintString("\***************** << POLISHER CONSOLE >> ********************/");
					UART0_Sendchar(10);
					UART0_PrintString("\*                                                           */");
					UART0_Sendchar(10);
					UART0_PrintString("\*                                                           */");
					UART0_Sendchar(10);
					UART0_PrintString("\*       Please Push Following Button For Operation          */");
					UART0_Sendchar(10);
					UART0_PrintString("\*                                                           */");
					UART0_Sendchar(10);
					UART0_PrintString("\* Start = 'S'   Stop = 'C'   ForWard = 'F'    Reverse = 'R' */");
					UART0_Sendchar(10);
					UART0_PrintString("\*                                                           */");
					UART0_Sendchar(10);
					UART0_PrintString("\*       Increase Speed = '+'       Decrease Speed = '-'     */");
					UART0_Sendchar(10);
					UART0_PrintString("\*                                                           */");
					UART0_Sendchar(10);
					UART0_PrintString("\*                                                           */");
					UART0_Sendchar(10);
					UART0_PrintString("\* Flow On/Off = 'Q'       INC_Flow = '<'     DEC_Flow = '>' */");
					UART0_Sendchar(10);
					UART0_PrintString("\*                                                           */");
					UART0_Sendchar(10);
					UART0_PrintString("\*                                                           */");
					UART0_Sendchar(10);
					UART0_PrintString("\******************** << POLISHER END >> *********************/");
					UART0_Sendchar(10);
					UART0_Sendchar(10);
					UART0_Sendchar(10);
}

#define NO_BUTTON_PRESSED 	 0
#define START_PRESSED 		's'
#define STOP_PRESSED 		'c'
#define FORWARD_PRESSED	    'f'
#define REVERSE_PRESSED     'r'
#define INC_SPEED_PRESSED   '+'
#define DEC_SPEED_PRESSED   '-'
#define INC_FLOW_PRESSED    ','
#define DEC_FLOW_PRESSED    '.'
#define HELP_PRESSED		'?'
#define LIGHT_UP		    '\''
#define LIGHT_DOWN		    ';'
#define Toggle_Water			'w'
char Check_uart0()
{
	if (LPC_UART0->LSR  & 0x20)  // Nothing received so just block
	{

		char uart0tmp =  LPC_UART0->RBR;
		switch (uart0tmp)
		{
		case NO_BUTTON_PRESSED :
			break;
		case START_PRESSED :
			Motor_State = MOTOR_RUN;
			ChengeFrequncyFlag = FLAGUP;
			ChangeStateFlag = FLAGUP;
			Timer_State = TIME0_RUN;
			Motor_Operation_Set(0x01,Motor_State,Motor_Dir_State);
			UART0_PrintString("START PRESS");
			UART0_Sendchar(10);
					break;
		case STOP_PRESSED :
			Motor_State = MOTOR_STOP;
			ChengeFrequncyFlag = FLAGUP;
			ChangeStateFlag = FLAGUP;
			Timer_State = TIME0_STOP;
			Motor_Operation_Set(0x01,Motor_State,Motor_Dir_State);
			UART0_PrintString("STOP PRESS");
			UART0_Sendchar(10);
			break;
		case FORWARD_PRESSED :
			Motor_Dir_State = MOTOR_DIR_FORWARD ;
			Motor_Operation_Set(0x01,Motor_State,Motor_Dir_State);
			ChangeDirFlag = FLAGUP;
			ChengeFrequncyFlag = FLAGUP;




			UART0_PrintString("FORWARD");
			UART0_Sendchar(10);
				break;
		case REVERSE_PRESSED :
			Motor_Dir_State = MOTOR_DIR_REVERSE ;
			Motor_Operation_Set(0x01,Motor_State,Motor_Dir_State);
			ChangeDirFlag = FLAGUP;
			ChengeFrequncyFlag = FLAGUP;



			UART0_PrintString("REVERSED");
			UART0_Sendchar(10);
			break;
		case INC_SPEED_PRESSED :
			
			ChengeFrequncyFlag = FLAGUP;
			//Freq = Read_Motor_Speed(0x01,0) + 5;
			Freq = Freq + 15;
			if (Freq > 7500)
				Freq = 7500;
		    Set_Motor_Speed(0x01,Freq);
		    UART0_PrintString("SPEED INC =");
		    UART0_PrintString(itoa_e(Freq , 10 ,4));
		    UART0_Sendchar(10);
			break;
		case DEC_SPEED_PRESSED :
			
			ChengeFrequncyFlag = FLAGUP;
			//Freq = Read_Motor_Speed(0x01,0) - 5;
			Freq = Freq - 15;
			if (Freq < 250)
					Freq = 250;
		    Set_Motor_Speed(0x01,Freq);
		    UART0_PrintString("SPEED DEC = ");
		    UART0_PrintString(itoa_e(Freq , 10 ,4));
		    UART0_Sendchar(10);
			break;
		case INC_FLOW_PRESSED :
			            CheckFlowFlag		 = FLAGUP;
			            NewFlow ++;
			            if (NewFlow > 99)
			            	NewFlow = 99;
			            UART0_PrintString("FLOW INC = ");
					    UART0_PrintString(itoa_e(NewFlow , 10 ,2));
					    UART0_PrintString(" %");
					    UART0_Sendchar(10);
			break;
		case DEC_FLOW_PRESSED :

						CheckFlowFlag		 = FLAGUP;
						 if (NewFlow > 0)
							 NewFlow --;

			            UART0_PrintString("FLOW DEC = ");
						UART0_PrintString(itoa_e(NewFlow , 10 ,2));
						UART0_PrintString(" %");
						UART0_Sendchar(10);
			break;
		case HELP_PRESSED:
			Uart_Command_Init();
			break;

		case LIGHT_UP :
			BacklightIntensity = BacklightIntensity + 50;
			if (BacklightIntensity > 8000)
				BacklightIntensity = 8000;
			Set_backLight(BacklightIntensity);
			break;

		case LIGHT_DOWN :
			if (BacklightIntensity < 51)
				BacklightIntensity = 0;
			else
				BacklightIntensity = BacklightIntensity - 50;
			Set_backLight(BacklightIntensity);
					break;
		case  Toggle_Water :
			        Water_State = !Water_State;
					T_Water();
			break;
		default :
			break;
		}
		return ( uart0tmp );
	}
	return 0;
}

/* ********************************************************
 *
 * 	Function : READ MOTOR Current
 *
 * ********************************************************/
uint16_t Read_Motor_Current(uint8_t Address)
{
	char  str[32] = {0};
	char tmpstr[10] = {0};
	uint8_t Operation_Data_Byte = 0x00;
	uint8_t LRC_TMP = 0x00 ;
	LPC_GPIO0->FIODIR |= (1 << 0 );
	LPC_GPIO0->FIOSET |= (1 << 0 );



	str[0] = ':';
	strcat(str,itoa_e(Address%10,16,2));

	strcat(str,"03");   /*Read Data Function Code*/

	strcat(str,"2527"); /*Read FREQUENCY Code*/

	strcat(str,"0001"); /*Read START Code*/

	LRC_TMP = LRC_CALC_STRING(&str[0]);

	strcat(str ,itoa_e(LRC_TMP,16,2));

	tmpstr[0] = 0x0D;
	tmpstr[1] = 0x0A;
	strcat(str,tmpstr);
	/*END OPERATION Code*/

		#ifdef in_line_debuge_F

				UART0_PrintString("MOTOR FIRST PHARSE = ");
			  UART0_PrintString(str);
				UART0_Sendchar(10);

		#endif

	/***********************Read Data Start ********************/
				UART1_PrintString(str);
				LPC_GPIO0->FIODIR |= (1 << 0 );
				DelayMs(2);
				LPC_GPIO0->FIOCLR |= (1 << 0 );

				strcpy(str,"");
				Read_Motor_Interval = 0;
		{
				char i = 0;
				char Read_Data[5] = {0};
				while(1)
					{
						if (Read_Motor_Interval == 1)
							break;
						if(LPC_UART1->LSR & 0x01)  // Nothing received so just block
							{
								str[i] = LPC_UART1->RBR;
								i++;
							#ifdef in_line_debuge_F
								UART0_PrintString("CHARECTER RECIEVED = ");
								UART0_PrintString(str);
								UART0_Sendchar(10);
							#endif
							}
					}
				if (str[0] != ':')
					{
						#ifdef in_line_debuge_F
							UART0_PrintString("Motor Read ERROR = ");
							UART0_PrintString(str);
							UART0_Sendchar(10);
						#endif
						return 500;
					}
				Read_Data[0] = str[7];
				Read_Data[1] = str[8];
				Read_Data[2] = str[9];
				Read_Data[3] = str[10];
				Read_Data[4] = 0;
						#ifdef in_line_debuge_F
							UART0_PrintString("Motor Read SPEED = ");
							UART0_PrintString(Read_Data);
							UART0_Sendchar(10);
							UART0_PrintString("PRINT DATA STRTOL = ");
							UART0_PrintString(itoa((uint16_t)strtol(Read_Data,NULL,16),10));
							UART0_Sendchar(10);
						#endif
				return ((uint16_t)strtol(Read_Data,NULL,16));
				}

}
uint8_t Read_Current_Flow()
{

}

uint8_t Set_Flow(uint8_t Flow_F)
{
	int8_t Diff_Flow  ;
	Diff_Flow = Flow_F - Flow ;
	Flow = Flow_F;
	if (Diff_Flow > 0)
	{
		STEPERRUN(0,Diff_Flow * 100,100);
	}
	else
	{
		STEPERRUN(1,abs(Diff_Flow)*100 , 100);
	}
	STEPRELEASE();
}


#define KNO_BUTTON_PRESSED 	     0x00
#define KSTART_PRESSED 		     0x14
#define KSTOP_PRESSED 		     0x24
#define KFORWARD_PRESSED	     0x34
#define KREVERSE_PRESSED         0x44
#define KINC_SPEED_PRESSED       0x13
#define KINC_HIGH_SPEED_PRESSED  0x33
#define KDEC_SPEED_PRESSED       0x31
#define KDEC_HIGH_SPEED_PRESSED  0x12
#define KINC_FLOW_PRESSED        0x11
#define KDEC_FLOW_PRESSED        0x22
#define KRPM_000				 0x21
#define KRPM_100				 0x41
#define KRPM_250				 0x23
#define KRPM_500				 0x43
#define KWATER_OPEN				 0x32
#define KWATER_CLOSE			 0x42
/*
#define KHELP_PRESSED		     0x56
#define KLIGHT_UP		         0x55
#define KLIGHT_DOWN		         0x22
#define KToggle_Water		     0x57
*/

void KEY_CHECK()
{
	uint8_t i,j;
	for (i = 1 ; i <= 4 ; i++ )
		for (j = 1 ; j <= 4 ; j++)
			if (KEY_CHECK_POS(i,j))
			{
	uint16_t sum = 0;
	sum = j + (i << 4);
	i = 5;
	j = 5;
		switch (sum)
		{
					case KNO_BUTTON_PRESSED :
						break;
					case KSTART_PRESSED :
						Motor_State = MOTOR_RUN;
						ChengeFrequncyFlag = FLAGUP;
						ChangeStateFlag = FLAGUP;
						Timer_State = TIME0_RUN;
						Motor_Operation_Set(0x01,Motor_State,Motor_Dir_State);
						UART0_PrintString("K_START PRESS");
						UART0_Sendchar(10);
								break;
					case KSTOP_PRESSED :
						Motor_State = MOTOR_STOP;
						ChengeFrequncyFlag = FLAGUP;
						ChangeStateFlag = FLAGUP;
						Timer_State = TIME0_STOP;
						Motor_Operation_Set(0x01,Motor_State,Motor_Dir_State);
						UART0_PrintString("K_STOP PRESS");
						UART0_Sendchar(10);
						break;
					case KFORWARD_PRESSED :
						Motor_Dir_State = MOTOR_DIR_FORWARD ;
						Motor_Operation_Set(0x01,Motor_State,Motor_Dir_State);
						ChangeDirFlag = FLAGUP;
						ChengeFrequncyFlag = FLAGUP;




						UART0_PrintString("K_FORWARD");
						UART0_Sendchar(10);
							break;
					case KREVERSE_PRESSED :
						Motor_Dir_State = MOTOR_DIR_REVERSE ;
						Motor_Operation_Set(0x01,Motor_State,Motor_Dir_State);
						ChangeDirFlag = FLAGUP;
						ChengeFrequncyFlag = FLAGUP;



						UART0_PrintString("K_REVERSED");
						UART0_Sendchar(10);
						break;
					case KINC_SPEED_PRESSED :

						ChengeFrequncyFlag = FLAGUP;
						//Freq = Read_Motor_Speed(0x01,0) + 5;
						Freq = Freq + 5;
						if (Freq > 5000)
							Freq = 5000;
					    Set_Motor_Speed(0x01,Freq);
					    UART0_PrintString("K_SPEED INC =");
					    UART0_PrintString(itoa_e(Freq , 10 ,4));
					    UART0_Sendchar(10);
						break;
					case KDEC_SPEED_PRESSED :

						ChengeFrequncyFlag = FLAGUP;
						//Freq = Read_Motor_Speed(0x01,0) - 5;
						Freq = Freq - 5;
						if (Freq < 250)
								Freq = 250;
					    Set_Motor_Speed(0x01,Freq);
					    UART0_PrintString("K_SPEED DEC = ");
					    UART0_PrintString(itoa_e(Freq , 10 ,4));
					    UART0_Sendchar(10);
						break;

					case KINC_HIGH_SPEED_PRESSED :

											ChengeFrequncyFlag = FLAGUP;
											//Freq = Read_Motor_Speed(0x01,0) + 5;
											Freq = Freq + 100;
											if (Freq > 5000)
												Freq = 5000;
										    Set_Motor_Speed(0x01,Freq);
										    UART0_PrintString("K_SPEED INC =");
										    UART0_PrintString(itoa_e(Freq , 10 ,4));
										    UART0_Sendchar(10);
											break;
										case KDEC_HIGH_SPEED_PRESSED :

											ChengeFrequncyFlag = FLAGUP;
											//Freq = Read_Motor_Speed(0x01,0) - 5;
											Freq = Freq - 100;
											if (Freq < 250)
													Freq = 250;
										    Set_Motor_Speed(0x01,Freq);
										    UART0_PrintString("K_SPEED DEC = ");
										    UART0_PrintString(itoa_e(Freq , 10 ,4));
										    UART0_Sendchar(10);
											break;

					case KINC_FLOW_PRESSED :
						            CheckFlowFlag		 = FLAGUP;
						            NewFlow ++;
						            if (NewFlow > 99)
						            	NewFlow = 99;
						            UART0_PrintString("K_FLOW INC = ");
								    UART0_PrintString(itoa_e(NewFlow , 10 ,2));
								    UART0_PrintString(" %");
								    UART0_Sendchar(10);
						break;
					case KDEC_FLOW_PRESSED :

									CheckFlowFlag		 = FLAGUP;
									 if (NewFlow > 0)
										 NewFlow --;

						            UART0_PrintString("K_FLOW DEC = ");
									UART0_PrintString(itoa_e(NewFlow , 10 ,2));
									UART0_PrintString(" %");
									UART0_Sendchar(10);
						break;

					case KRPM_000 :
												ChengeFrequncyFlag = FLAGUP;
												//Freq = Read_Motor_Speed(0x01,0) + 5;
												Freq = 0;
											    Set_Motor_Speed(0x01,Freq);
											    UART0_PrintString("Speed set to =");
											    UART0_PrintString(itoa_e(Freq , 10 ,4));
											    UART0_Sendchar(10);

						break;
					case KRPM_100 :
																	ChengeFrequncyFlag = FLAGUP;
																	//Freq = Read_Motor_Speed(0x01,0) + 5;
																	Freq = 531;
																    Set_Motor_Speed(0x01,Freq);
																    UART0_PrintString("Speed set to =");
																    UART0_PrintString(itoa_e(Freq , 10 ,4));
																    UART0_Sendchar(10);

											break;
					case KRPM_250 :
																	ChengeFrequncyFlag = FLAGUP;
																	//Freq = Read_Motor_Speed(0x01,0) + 5;
																	Freq = 1329;
																    Set_Motor_Speed(0x01,Freq);
																    UART0_PrintString("Speed set to =");
																    UART0_PrintString(itoa_e(Freq , 10 ,4));
																    UART0_Sendchar(10);

											break;
					case KRPM_500 :
																	ChengeFrequncyFlag = FLAGUP;
																	//Freq = Read_Motor_Speed(0x01,0) + 5;
																	Freq = 2659;
																    Set_Motor_Speed(0x01,Freq);
																    UART0_PrintString("Speed set to =");
																    UART0_PrintString(itoa_e(Freq , 10 ,4));
																    UART0_Sendchar(10);

											break;

					case KWATER_OPEN :
						if (Water_State != 1)
						{
						 Water_State = 1;
						T_Water();
						}
						break;
					case KWATER_CLOSE :
											if (Water_State != 0)
											{
											 Water_State = 0;
											T_Water();
											}
											break;
					/*case KHELP_PRESSED:
						Uart_Command_Init();
						break;

					case KLIGHT_UP :
						BacklightIntensity = BacklightIntensity + 50;
						if (BacklightIntensity > 8000)
							BacklightIntensity = 8000;
						Set_backLight(BacklightIntensity);
						break;

					case KLIGHT_DOWN :
						if (BacklightIntensity < 51)
							BacklightIntensity = 0;
						else
							BacklightIntensity = BacklightIntensity - 50;
						Set_backLight(BacklightIntensity);
								break;
					case  KToggle_Water :
						        Water_State = !Water_State;
								T_Water();
						break;*/
					default :
						break;

}
			}
				}

#endif
