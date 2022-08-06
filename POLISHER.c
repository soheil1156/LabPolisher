/* * * * * SOHEIL * * * * *
 * * * * * 1392,6,2 * * * * */
 
 #include "LPC17xx.h"
 #include <stdlib.h>     /* atoi */
 #include "type.h" 
 #include "system_LPC17xx.h"
 #include "UART.h"
 #include "GPIO.h"
 #include "KS0108.h"

 #include "ADC.h"
 #include "graphic.c"
 #include "image.h"
 #include "pwm.h"
 #include "timer.h"
 #include "KEYBOARD.h"

 #include "Functions.c"
 
//#define debuge

 int main()
 {
	 
	 
/************INITIALIZE********************
*
*********************************************/ 
	 SystemInit();
	

	Set_backLight(BacklightIntensity);
   
 UART0_Init(57600);
 	 UART1_Init(19200);
	
	 UART0_PrintString("Begine1");
		 UART0_Sendchar(10);


	
	 TimerInit(0, 72000000); //1SEC INTERVAL FOR CLOCK
	 TimerInit(1, 72000000/10);  // ~ 0.1SEC INTERVAL FOR CLOCK
	 STEPPER_INIT();
	 ADCInit(ADC_CLK);
	 KEY_Init();
	 GLCD_Initalize();

	 //LPC_PINCON->PINMODE4 &= 0;

	 #ifdef debuge
	 
		 UART0_PrintString("Begine1");
		 UART0_Sendchar(10);

		 UART0_PrintString("SYSTEM INIT");
		 UART0_Sendchar(10);

		 UART0_PrintString("UART 0 INIT");
		 UART0_Sendchar(10);

		 UART0_PrintString("RS485 _ON UART1 _ INIT");
		 UART0_Sendchar(10);

		 UART0_PrintString("GLCD INIT");
		 UART0_Sendchar(10);

		 UART0_PrintString("Stepper INIT");
		 UART0_Sendchar(10);

		 UART0_PrintString("PWM_CHANNEL1_PORT1.18 INIT");
	     UART0_Sendchar(10);
	 
	 #endif
/**********************************************
*
********************END OF INIT****************/
	 
	 Dis_init();
	 GLCD_ClearScreen();
	
	 enable_timer(0);
	 enable_timer(1);

	 RefreshBackground();


	#ifdef debuge
	 UART0_PrintString("MOTOR SPEED IS");
	 UART0_PrintString(itoa((uint16_t)((float)Read_Motor_Speed(0x01,1)) * 0.188,10));
	 UART0_Sendchar(10);
	#endif

/*
	 Set_Motor_Speed(0x01,1000);
	 DelayMs(15);

	 Motor_Operation_Set(0x01,1,0);
	 DelayMs(75000);
	 UART0_Sendchar(10);
	 UART0_PrintString("MOTOR SPEED IS ::::::");
	 UART0_PrintString(itoa((uint16_t)((float)Read_Motor_Speed(0x01)) * 0.188,10));
	  */
	 DelayMs(15);
	 NewFreq = OldFreq = 0;
	 Set_Motor_Speed(0x01,250);
	 ChengeFrequncyFlag = FLAGUP;
	Rpm_update(NewFreq);
	Water_Flow_Update(0);
	Current_Show_Update(0.0);
	Motor_Direction_update(1);
	 DelayMs(25);
	 Motor_Operation_Set(0x01,Motor_State,Motor_Dir_State);
	 DelayMs(25);
	 Uart_Command_Init();
	 while(1)
	 {



		 Check_uart0();
		 KEY_CHECK();
			/* Timer Update */
			if(Newtime > Oldtime)
			{
				for(;Oldtime < Newtime ;Oldtime++)
				Timer_update();
				CheckCurrentFlag    = FLAGUP;
			}
			if (CheckCurrentFlag    == FLAGUP)
			{
				Motor_Current_New = Read_Motor_Current(0x01);
				if (Motor_Current_New == 500)
					Motor_Current_New = Motor_Current_Old;
				if(Motor_Current_New!=Motor_Current_Old)
				{
				Current_Show_Update(((float)Motor_Current_New)/10.0F);
				Motor_Current_Old = Motor_Current_New;
				}
				CheckCurrentFlag    = FLAGDOWN;
			}
			/* CCW UPDATE */
			if (ChangeDirFlag)
			{
					Motor_Direction_update(Motor_Dir_State);
					ChangeDirFlag = FLAGDOWN;
			}
			
			if (CheckFlowFlag)
			{

				Set_Flow(NewFlow);
				Water_Flow_Update(Flow);
				CheckFlowFlag = FLAGDOWN;
			}
			/* RPM UPDATE */
			if (ChengeFrequncyFlag)
			{
				NewFreq = Freq;
				//NewFreq = Read_Motor_Speed(0x01,1);
				if(NewFreq != OldFreq)
				{
					OldFreq = NewFreq;
					Rpm_update(NewFreq);
					//Water_Flow_Update(NewFreq);
					//Current_Show_Update((float)NewFreq/100.0F);
					//Motor_Direction_update((NewFreq / 5)%2);
					/*
					UART0_PrintString("MOTOR SPEED IS");
					UART0_PrintString(itoa((uint16_t)((float)Read_Current_Motor_Speed(0x01)) * 0.1566,10));
					UART0_Sendchar(10);*/


				}
				else if (Read_Motor_Speed(0x01,1) == Read_Motor_Speed(0x01,0)) //IF CURRENT SPEED = SET SPEED
					ChengeFrequncyFlag = FLAGDOWN;
			 }
			if (Anim_Rotate_Interval != Anim_Rotate_Last)
			{
				if ( Motor_State == MOTOR_RUN){
				Anim_Rotate0_update(Anim_Rotate_Interval);
				Anim_Rotate_Last = Anim_Rotate_Interval  ;
				}
				else {
					 Stop_Show_Update ((Anim_Rotate_Interval >> 3) & 0x01);
				}
			}
			}
		
	 }
 
 
 /************IRQ HANDELER*******************
*
*********************************************/ 
 
 void TIMER0_IRQHandler (void)
{

	TIMER0_interrupt();
	if(Timer_State == TIME0_RUN)
	{
	Newtime ++;
	if (Newtime % 5 == 0) // 5 Sec Motor check For Asurance
	ChengeFrequncyFlag = FLAGUP;
	}
}
 
 void TIMER1_IRQHandler (void)
{

	TIMER1_interrupt();
	Read_Motor_Interval++;
	Anim_Rotate_Interval ++ ;
}
