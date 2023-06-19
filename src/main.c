#include "timer.h"
#include "Gpio.h"
#include "Rcc.h"

int main()
{
	Rcc_Init();
	Rcc_Enable(RCC_GPIOA);

	GPT_Init();
	GPT_StartTimer(0x7D0);

	Gpio_ConfigPin(GPIO_A, PIN0_ID, GPIO_OUTPUT, GPIO_PUSH_PULL);
	Gpio_WritePin(GPIO_A, PIN0_ID, LOW);

	Gpio_ConfigPin(GPIO_A, PIN1_ID, GPIO_OUTPUT, GPIO_PUSH_PULL);
	Gpio_WritePin(GPIO_A, PIN1_ID, LOW);

	Gpio_ConfigPin(GPIO_A, PIN2_ID, GPIO_OUTPUT, GPIO_PUSH_PULL);
	Gpio_WritePin(GPIO_A, PIN2_ID, LOW);

	uint8 flag = 0;
	uint32 x;
    while(1)
    {

    	x  = GPT_CheckTimeIsElapsed();
    	if(x == 1)
    	{
    		if(flag == 0)
    		{
    			flag = 1;
    			Gpio_WritePin(GPIO_A, PIN1_ID, HIGH);
    		}

    	}


    }

    return 0;

}
