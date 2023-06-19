#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"


typedef struct
{
	uint32 CR1;
	uint32 CR2;
	uint32 SMCR;
	uint32 DIER;
	uint32 SR;
	uint32 EGR;
	uint32 CCMR1;
	uint32 CCMR2;
	uint32 CCER;
	uint32 CNT;
	uint32 PSC;
	uint32 ARR;
	uint32 CCR1;
	uint32 CCR2;
	uint32 CCR3;
	uint32 CCR4;
	uint32 Reserved;
	uint32 DCR;
	uint32 DMAR;
	uint32 TIM2_OR;
	uint32 TIM5_OR;
}Timer;

#define Timer2  ( (Timer*) 0x40000000 )


/*
 * function name: GPT_Init
 * Description: A function to initialize the GPT registers with the needed initial
 * values to support the needed timing actions.
 */
void GPT_Init(void);

/*
 * function name: GPT_StartTimer
 * Description: A function to request the GPT to start and send the required auto-reload value.
 */
void GPT_StartTimer(unsigned long int OverFlowTicks);

/*
 * function name: GPT_CheckTimeIsElapsed
 * Description: A function to return (1) if an overflow occurred after the last call of
 * GPT_StartTimer and (0) if no overflow occurred or GPT_StartTimer is not called from the last read.
 */
unsigned char GPT_CheckTimeIsElapsed(void);

/*
 * function name: GPT_GetElapsedTime
 * Description: A function to return number of elapsed ticks from the last call of the GPT_StartTimer,
 * 0 if it is not called and 0xffffffff if an overflow occurred.
 */
unsigned long int GPT_GetElapsedTime(void);

/*
 * function name: GPT_GetRemainingTime
 * Description: A function to return number of remaining ticks till the overflow ticks passed to
 * GPT_StartTimer, 0xffffffff if GPT_startTime is not called, 0 if an overflow occurred.
 */
unsigned long int GPT_GetRemainingTime(void);

#endif /* TIMER_H_ */
