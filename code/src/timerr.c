#include "timer.h"
#include "Rcc_Private.h"
#include "Bit_Operations.h"

uint8 GPT_StartTimerIsCalled = 0; /* to let us know if the GPT_StartTimer function is called */

/*
 * function name: GPT_Init
 * Description: A function to initialize the GPT registers with the needed initial
 * values to support the needed timing actions.
 */
void GPT_Init(void)
{
	RCC_APB1ENR |= (1 << 0 );  /* TIM2 clock enabled */
	Timer2->PSC = 15999;       /* set the required pre-scaler value */
}

/*
 * function name: GPT_StartTimer
 * Description: A function to request the GPT to start and send the required auto-reload value.
 */
void GPT_StartTimer(unsigned long int OverFlowTicks)
{
	GPT_StartTimerIsCalled = 1;  /* to know that the function is called */
	Timer2->CNT = 0;             /* set the counter value to zero */
	Timer2->ARR = OverFlowTicks; /* set the required auto-reload value */
	SET_BIT(Timer2->CR1, 0);     /* enable the counter for timer2 */
	while(!(Timer2->SR & (1<<0))); /* wait until the counter is enabled */
}

/*
 * function name: GPT_CheckTimeIsElapsed
 * Description: A function to return (1) if an overflow occurred after the last call of
 * GPT_StartTimer and (0) if no overflow occurred or GPT_StartTimer is not called from the last read.
 */
unsigned char GPT_CheckTimeIsElapsed(void)
{
	if(Timer2->SR & (1<<0))   /* if an overflow occurred return 1 */
	{
		return 1;
	}
	else    /* may be the overflow has not occurred yet or we didn't call the GPT_StartTimer
	           function after the last check of the overflow */
	{
		return 0;
	}
}

/*
 * function name: GPT_GetElapsedTime
 * Description: A function to return number of elapsed ticks from the last call of the GPT_StartTimer,
 * 0 if it is not called and 0xffffffff if an overflow occurred.
 */
unsigned long int GPT_GetElapsedTime(void)
{
	if(Timer2->SR & (1<<0))       /* if an overflow occurred return 0xffffffff */
	{
		return 0xffffffff;
	}

	return Timer2->CNT;          /* if no over flow occurred return the value of the counter and
	                                it will be 0 if we didn't call the GPT_StartTimer function */
}

/*
 * function name: GPT_GetRemainingTime
 * Description: A function to return number of remaining ticks till the overflow ticks passed to
 * GPT_StartTimer, 0xffffffff if GPT_startTime is not called, 0 if an overflow occurred.
 */
unsigned long int GPT_GetRemainingTime(void)
{
	if(Timer2->SR & (1<<0))        /* check if an overflow occurred to return 0 */
		return 0;
	if(GPT_StartTimerIsCalled == 0) /* if we didn't call GPT_startTime function */
		return 0xffffffff;          /* then return 0xffffffff */

	return Timer2->ARR - Timer2->CNT; /* if we called the GPT_startTime function and no overflow
	                                     occurred then return the remaining time value */
}
