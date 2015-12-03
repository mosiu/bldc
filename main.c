/*
 * main.c
 *
 *  Created on: 2015 Nov 18 14:07:11
 *  Author: mosiu
 */


#include <DAVE.h>                 //Declarations from DAVE Code Generation (includes SFR declaration)

#define SYSTICK_PERIOD_US 	1000u
#define PWM_FREQ_HZ			10000u
#define PWM_PERIOD_SEC		(1.0/PWM_FREQ_HZ)
#define TIME_PER_SYSTIC_SEC (SYSTICK_PERIOD_US)/1000000.0

typedef enum {
	PHASE_A,
	PHASE_B,
	PHASE_C
}phase_t;


/**

 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. It is responsible for
 * invoking the APP initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
 * code.
 */

void PWM_set_duty (float duty_A, float duty_B, float duty_C);
void PWM_set_freq (uint32_t freq_hz);
void ZSM_update(float amplitude, float phase, uint32_t frequency);
void PWM_set_interrupt(void);

void PWM_systick_setting(void);

int main(void)
{
	uint32_t Systick_Timer_ID;
  DAVE_STATUS_t status;
  status = DAVE_Init();           /* Initialization of DAVE APPs  */
  PWM_set_interrupt();

  // Create software timer - 1ms
//  Systick_Timer_ID = (uint32_t)SYSTIMER_CreateTimer(SYSTICK_PERIOD_US,SYSTIMER_MODE_PERIODIC,(void*)PWM_systick_setting,NULL);
//  SYSTIMER_StartTimer(Systick_Timer_ID);


  if(status == DAVE_STATUS_FAILURE)
  {
    /* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
    XMC_DEBUG("DAVE APPs initialization failed\n");

    while(1U)
    {

    }
  }

  PWM_CCU8_Start(&PWM_CCU8_0);
  PWM_CCU8_Start(&PWM_CCU8_1);
  PWM_CCU8_Start(&PWM_CCU8_2);


  // freq to 10khz
  // TODO:: set it here
  PWM_set_freq(PWM_FREQ_HZ);
  //kernel_number = PWM_CCU8_0.kernel_number;
  //ccu_modules = 1U << (SCU_GENERAL_CCUCON_GSC80_Pos + kernel_number);
  //GLOBAL_CCU8_SyncStartTriggerHigh(GLOBAL_CCU8_CCUCON_Msk);

  /* Placeholder for user application code. The while loop below can be replaced with user application code. */
  while(1U)
  {
  }
}

//void PWM_systick_setting(void)
//{
//	ZSM_update(0.7, 0, 200);
//}

void ZSM_update(float amplitude, float phase, uint32_t frequency)
{
	if (amplitude > 1.0)
		amplitude = 1.0;
	if (amplitude < 0)
		amplitude = 0;

	float duty_cycle_result_A, duty_cycle_result_B, duty_cycle_result_C;

	static float time = 0;

	duty_cycle_result_A = amplitude*(0.5*sinf(6.2832*frequency*time + phase)+0.5);
	duty_cycle_result_B = amplitude*(0.5*sinf(6.2832*frequency*time + phase + 2.0944)+0.5);
	duty_cycle_result_C = amplitude*(0.5*sinf(6.2832*frequency*time + phase - 2.0944)+0.5);
	PWM_set_duty(duty_cycle_result_A, duty_cycle_result_B, duty_cycle_result_C);

	time += PWM_PERIOD_SEC;
}



void PWM_set_freq(uint32_t freq_hz)
{
	  PWM_CCU8_SetFreqSymmetric(&PWM_CCU8_0, freq_hz);
	  PWM_CCU8_SetFreqSymmetric(&PWM_CCU8_1, freq_hz);
	  PWM_CCU8_SetFreqSymmetric(&PWM_CCU8_2, freq_hz);
}

void PWM_set_duty (float duty_A, float duty_B, float duty_C)
{
	PWM_CCU8_SetDutyCycleSymmetric(&PWM_CCU8_0, XMC_CCU8_SLICE_COMPARE_CHANNEL_1, (uint32_t)10000*duty_A);

	PWM_CCU8_SetDutyCycleSymmetric(&PWM_CCU8_1, XMC_CCU8_SLICE_COMPARE_CHANNEL_1, (uint32_t)10000*duty_B);

	PWM_CCU8_SetDutyCycleSymmetric(&PWM_CCU8_2, XMC_CCU8_SLICE_COMPARE_CHANNEL_1, (uint32_t)10000*duty_C);
}

void PWM_set_interrupt (void)
{
	XMC_CCU8_SLICE_EnableEvent(CCU80_CC80, XMC_CCU8_SLICE_IRQ_ID_PERIOD_MATCH);
	XMC_CCU8_SLICE_SetInterruptNode(CCU80_CC80, XMC_CCU8_SLICE_IRQ_ID_PERIOD_MATCH, XMC_CCU8_SLICE_SR_ID_0);
	NVIC_EnableIRQ(CCU80_0_IRQn);
}

void
CCU80_0_IRQHandler()
{
	ZSM_update(0.7, 0, 1);
}


