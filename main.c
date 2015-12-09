/*
 * main.c
 *
 *  Created on: 2015 Nov 18 14:07:11
 *  Author: mosiu
 */


#include <DAVE.h>                 //Declarations from DAVE Code Generation (includes SFR declaration)
#include <math.h>

#define SYSTICK_PERIOD_US 	1000u
#define PWM_FREQ_HZ			10000u
#define PWM_PERIOD_SEC		(1.0/PWM_FREQ_HZ)
#define TIME_PER_SYSTIC_SEC (SYSTICK_PERIOD_US)/1000000.0
#define SQRT_3				1.73205

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
void SYSTICK_Init();

void SYSTIMER_Irq(void);

int main(void)
{
  DAVE_STATUS_t status;
  status = DAVE_Init();           /* Initialization of DAVE APPs  */
  PWM_set_interrupt();




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



void ZSM_update(float amplitude, float phase, uint32_t frequency)
{
	// this function is documented by MATLAB "plot_3f.m" file
	if (amplitude > 1.0)
		amplitude = 1.0;
	if (amplitude < 0)
		amplitude = 0;

	float duty_A, duty_B, duty_C;
	float z0;
	float max_duty, min_duty;

	static float time = 0;

	duty_A = 2*amplitude*(0.5*sinf(6.2832*frequency*time + phase)+0.5)/SQRT_3;
	duty_B = 2*amplitude*(0.5*sinf(6.2832*frequency*time + phase + 2.0944)+0.5)/SQRT_3;
	duty_C = 2*amplitude*(0.5*sinf(6.2832*frequency*time + phase - 2.0944)+0.5)/SQRT_3;

	max_duty = ( (duty_A > duty_B) ? duty_A : duty_B );
	if(duty_C > max_duty)
		max_duty = duty_C;

	min_duty = ( (duty_A < duty_B) ? duty_A : duty_B );
	if(duty_C < min_duty)
			min_duty = duty_C;

	z0 = 0.5 - 0.5*(max_duty + min_duty);

	duty_A += z0;
	duty_B += z0;
	duty_C += z0;

	PWM_set_duty(duty_A, duty_B, duty_C);

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
	ZSM_update(0.86, 0, 1);
}

void
SYSTICK_Init()
{
	uint32_t Systick_Timer_ID;
	  // Create software timer - 1ms
	  Systick_Timer_ID = (uint32_t)SYSTIMER_CreateTimer(SYSTICK_PERIOD_US,SYSTIMER_MODE_PERIODIC,(void*)SYSTIMER_Irq,NULL);
	  SYSTIMER_StartTimer(Systick_Timer_ID);
}

void SYSTIMER_Irq(void)
{

}

#define BSPRTNODEBASE_AI_DEV         VADC
#define BSPRTNODEBASE_AI_DEV_0       VADC_G0
#define BSPRTNODEBASE_AI_SIZE        4UL

//void vadc_init(void)
//{
//  XMC_VADC_GLOBAL_CONFIG_t xGlobalCfg = {
//    .clock_config = {
//      .analog_clock_divider  = 3,
//      .msb_conversion_clock  = 0,
//      .arbiter_clock_divider = 1
//    },
//  };
//  XMC_VADC_GROUP_CONFIG_t xGroupCfg = {
//    .class1 = {
//      .conversion_mode_standard = XMC_VADC_CONVMODE_12BIT,
//      .sample_time_std_conv     = 3U,
//    }
//  };
//  XMC_VADC_QUEUE_CONFIG_t xQueueCfg;
//  int                     i;
//
//  /* Initialize the VADC global registers */
//  XMC_VADC_GLOBAL_Init(BSPRTNODEBASE_AI_DEV, &xGlobalCfg);
//  /* Configure a conversion kernel */
//  XMC_VADC_GROUP_Init(BSPRTNODEBASE_AI_DEV_0, &xGroupCfg);
//  XMC_VADC_GROUP_QueueInit(BSPRTNODEBASE_AI_DEV_0, &xQueueCfg);
//  /* Configure measurement sequence */
//  for(i = 0; i < BSPRTNODEBASE_AI_SIZE; i++)
//  {
//    XMC_VADC_CHANNEL_CONFIG_t  xChannelCfg = {
//      .input_class   = XMC_VADC_CHANNEL_CONV_GROUP_CLASS1,
//      .alias_channel = XMC_VADC_CHANNEL_ALIAS_DISABLED
//    };
//    XMC_VADC_RESULT_CONFIG_t xResultOutCfg = {
//      .post_processing_mode   = XMC_VADC_DMM_REDUCTION_MODE,
//      .data_reduction_control = 0,   /* Reduction disabled */
//      .wait_for_read_mode     = false,
//      .part_of_fifo           = true
//    };
//    XMC_VADC_RESULT_CONFIG_t xResultInCfg = {
//      .post_processing_mode   = XMC_VADC_DMM_REDUCTION_MODE,
//      .data_reduction_control = 1,  /* Accumulate 2 result values */
//      .wait_for_read_mode     = true,
//      .part_of_fifo           = false
//    };
//    XMC_VADC_QUEUE_ENTRY_t xQueueEntry = {
//      .refill_needed      = true,  /* Refill is needed */
//      .external_trigger   = true,  /* External trigger is required */
//      .generate_interrupt = false
//    };
//    /* Configure a channel */
//    xChannelCfg.result_reg_number = 2*i + 1;  /* Input result FIFO */
//    XMC_VADC_GROUP_ChannelInit(BSPRTNODEBASE_AI_DEV_0, i, &xChannelCfg);
//    /* Configure a result registers */
//    XMC_VADC_GROUP_ResultInit(BSPRTNODEBASE_AI_DEV_0, 2*i,     &xResultOutCfg);
//    XMC_VADC_GROUP_ResultInit(BSPRTNODEBASE_AI_DEV_0, 2*i + 1, &xResultInCfg);
//    /* Add the channel to the queue two times -- two measurements in a
//     * sequence are averaged in result register */
//    xQueueEntry.channel_num = i;
//    XMC_VADC_GROUP_QueueInsertChannel(BSPRTNODEBASE_AI_DEV_0, xQueueEntry);
//    if(i == BSPRTNODEBASE_AI_SIZE - 1) xQueueEntry.generate_interrupt = true;
//    XMC_VADC_GROUP_QueueInsertChannel(BSPRTNODEBASE_AI_DEV_0, xQueueEntry);
//  }
//  /* Connect Request Source Event to the NVIC nodes */
//  XMC_VADC_GROUP_QueueSetReqSrcEventInterruptNode(BSPRTNODEBASE_AI_DEV_0,
//                                                  XMC_VADC_SR_GROUP_SR0);
//  /* Enable IRQ */
//  NVIC_SetPriority(VADC0_G0_0_IRQn, 63u);
//  NVIC_EnableIRQ(VADC0_G0_0_IRQn);
//  /* Enable the analog converters */
//  XMC_VADC_GROUP_SetPowerMode(BSPRTNODEBASE_AI_DEV_0,
//                              XMC_VADC_GROUP_POWERMODE_NORMAL);
//  /* Perform calibration of the converter */
//  XMC_VADC_GLOBAL_StartupCalibration(BSPRTNODEBASE_AI_DEV);
//}
