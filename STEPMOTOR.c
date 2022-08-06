/****************************************************************************
 *   SOHEIL NAZARI                       $
 *   Project: STEPER CONTROL
 *
 *   Description:
 *   4 WIRE  STEPPER CONTROL FILE
 *
 ****************************************************************************
****************************************************************************/
#include "lpc17xx.h"
#include "type.h"
#include "STEPMOTOR.h"

	void DelayMs( uint16_t ms ){
const int ten = 10800; // must be callibrate!!
	int i;
	for(  i=0 ; i <=ten * ms ; i++);
	
}
void Delay10us( uint32_t tenMicroSecondCounter )
{
	const int ten = 1080;
	int i;
	for( i=0 ; i <=ten * tenMicroSecondCounter ; i++);
}

 void STEPPER_INIT()
	{
		STEPER_PORT_SET;
	}
	void STEPRELEASE()
	{
		A1_NEG;
		A2_NEG;
		B1_NEG;
		B2_NEG;
	}
 void STEPERRUN(uint8_t Direction,uint16_t Angle,uint16_t SpeedStepPerSEC)
	
	{
		// DIRECTION = 0 CW
		// DIRECTION = 1 CCW
		int i;
	  int delay = (int)((1.0/(float)SpeedStepPerSEC) * 10000.0);
		int StepNumber = (int)Angle / 3.2;
		if (Direction ==0)
		{
						i = 0 ;
			while(1)
			{
				if ( i == 	StepNumber)
					break;
				B2_POS;
				A1_NEG;
				Delay10us(delay);
				i++;
						if ( i == 	StepNumber)
					break;
				A2_POS;
				B2_NEG;
				Delay10us(delay);
				i++;
								if ( i == 	StepNumber)
					break;
				B1_POS;
				A2_NEG;
				Delay10us(delay);
				i++;
										if ( i == 	StepNumber)
					break;
				A1_POS;
				B1_NEG;
				Delay10us(delay);
				i++;
			}
		}
		else
		{
			i = 0 ;
			while(1)
			{
				if ( i == 	StepNumber)
					break;
				B2_NEG;
				A1_POS;
				Delay10us(delay);
				i++;
						if ( i == 	StepNumber)
					break;
				A1_NEG;
				B1_POS;
				Delay10us(delay);
				i++;
								if ( i == 	StepNumber)
					break;
				B1_NEG;
				A2_POS;
				Delay10us(delay);
				i++;
										if ( i == 	StepNumber)
					break;
				A2_NEG;
				B2_POS;
				Delay10us(delay);
				i++;
			}
			
		}
	}
	void STEPERRUN_TORQUE(uint8_t Direction,uint16_t Angle,uint16_t SpeedStepPerSEC)
	
	{
		// DIRECTION = 0 CW
		// DIRECTION = 1 CCW
		int i;
	  int delay = (int)((1.0/(float)SpeedStepPerSEC) * 10000.0);
		int StepNumber = (int)Angle / 3.2;
		if (Direction ==0)
		{
								i = 0 ;
			while(1)
			{
				if ( i == 	StepNumber)
					break;
				B2_POS;
				A2_POS;
				B2_NEG;
				A1_NEG;
				Delay10us(delay);
				i++;
						if ( i == 	StepNumber)
					break;
				A2_POS;
				B2_NEG;
				B1_POS;
				A2_NEG;
				Delay10us(delay);
				i++;
								if ( i == 	StepNumber)
					break;
				B1_POS;
				A2_NEG;
				A1_POS;
				B1_NEG;
				Delay10us(delay);
				i++;
										if ( i == 	StepNumber)
					break;
				A1_POS;
				B1_NEG;
				B2_POS;
				A2_NEG;
				Delay10us(delay);
				i++;
			}
		}
		else
		{
			i = 0 ;
			while(1)
			{
				if ( i == 	StepNumber)
					break;
				B2_NEG;
				A1_POS;
				Delay10us(delay);
				i++;
						if ( i == 	StepNumber)
					break;
				A1_NEG;
				B1_POS;
				Delay10us(delay);
				i++;
								if ( i == 	StepNumber)
					break;
				B1_NEG;
				A2_POS;
				Delay10us(delay);
				i++;
										if ( i == 	StepNumber)
					break;
				A2_NEG;
				B2_POS;
				Delay10us(delay);
				i++;
			}
			
		}
	}