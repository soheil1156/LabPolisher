/****************************************************************************
 *   SOHEIL NAZARI                       $
 *   Project: STEPER CONTROL
 *
 *   Description:
 *   4 WIRE  STEPPER CONTROL FILE
 *
 ****************************************************************************
****************************************************************************/
	
	#ifndef _STEPPER
	#define _STEPPER
	
	//Control PIN SELECT
	
	
	#define A1_PIN (1 << 17)
	#define A2_PIN (1 << 18)
	#define B1_PIN (1 << 19)
	#define B2_PIN (1 << 20)
	
	
	//CONTROL PORT
	
	#define STEPER_PORT_DIR LPC_GPIO0->FIODIR
	#define STEPPER_PORT_SET LPC_GPIO0->FIOSET 
	#define STEPPER_PORT_CLR LPC_GPIO0->FIOCLR 
	
	#define STEPER_PORT_SET STEPER_PORT_DIR |=  (A1_PIN + A2_PIN + B1_PIN + B2_PIN); 
	#define STEPER_PORT_CLR STEPER_PORT_DIR &= ~(A1_PIN + A2_PIN + B1_PIN + B2_PIN);
	
	//PULSE DEFINE
	
	#define A1_POS STEPPER_PORT_SET |= A1_PIN
	#define A2_POS STEPPER_PORT_SET |= A2_PIN
	#define B1_POS STEPPER_PORT_SET |= B1_PIN
	#define B2_POS STEPPER_PORT_SET |= B2_PIN
	
	#define A1_NEG STEPPER_PORT_CLR |= A1_PIN
	#define A2_NEG STEPPER_PORT_CLR |= A2_PIN
	#define B1_NEG STEPPER_PORT_CLR |= B1_PIN
	#define B2_NEG STEPPER_PORT_CLR |= B2_PIN
	
	//DELAY FUNCTION
	
	void DelayMs( uint16_t ms );
	void Delay10us( uint32_t tenMicroSecondCounter );
	
	
	//STEPPER FUNCTION
	
	
	void STEPPER_INIT(void);
	void STEPRELEASE(void);
	
	void STEPERRUN(uint8_t Direction,uint16_t Angle,uint16_t SpeedStepPerSEC);
	void STEPERRUN_TORQUE(uint8_t Direction,uint16_t Angle,uint16_t SpeedStepPerSEC);
	
	#endif
	
