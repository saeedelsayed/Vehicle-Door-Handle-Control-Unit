#include "timer.h"
#include "Gpio.h"
#include "Rcc.h"

int main()
{
	Rcc_Init();
	Rcc_Enable(RCC_GPIOA);
	Rcc_Enable(RCC_GPIOB);
	GPT_Init();

	// vehicle lock button
	Gpio_ConfigPin(GPIO_A, PIN0_ID, GPIO_INPUT, GPIO_PULL_UP);

	// door lock button
	Gpio_ConfigPin(GPIO_A, PIN1_ID, GPIO_INPUT, GPIO_PULL_UP);

	// leds pins
	int output_pins[3] = {PIN0_ID, PIN1_ID, PIN2_ID};
	int i = 0;
	for (i = 0; i < 3; i++)
	{
		Gpio_ConfigPin(GPIO_B, output_pins[i], GPIO_OUTPUT, GPIO_PUSH_PULL);
		Gpio_WritePin(GPIO_B, output_pins[i], LOW);
	}

	// 1 means it's locked 0 is unlocked
	uint8 door_lock = 1;
	uint8 vehicle_lock = 1;

	uint8 flag_case1 = 0, flag_case2 = 0, flag_case4 = 0, button1_pressed = 0, button2_pressed = 0;
	uint32 time_elapsed = 0;

	while (1)
	{

		// vehicle unlock button pressed
		if (!(Gpio_ReadPin(GPIO_A, PIN0_ID))&& !button1_pressed)
		{
			button1_pressed = 1;
			if (vehicle_lock == 1)
			{

				// vehicle unlocked
				vehicle_lock = 0;
				flag_case1 = 1;
				// Vehicle lock led on
				Gpio_WritePin(GPIO_B, PIN0_ID, HIGH);
				// Hazzard light
				Gpio_WritePin(GPIO_B, PIN1_ID, HIGH);
				// ambient lights
				Gpio_WritePin(GPIO_B, PIN2_ID, HIGH);

				// 11000 ms
				GPT_StartTimer(0x2AF8);
			}
			else
			{
				Gpio_WritePin(GPIO_B, PIN0_ID, LOW);
				vehicle_lock = 1;
				// hazard light blinking 2 times
				Gpio_WritePin(GPIO_B, PIN1_ID, HIGH);
				GPT_StartTimer(0x2AF8);
				// ambient light is off
				Gpio_WritePin(GPIO_B, PIN2_ID, LOW);
				flag_case4 = 1;
			}
		}
//		else if(Gpio_ReadPin(GPIO_A, PIN0_ID))
//		{
//			button1_pressed = 0;
//		}
//
//		if (!(Gpio_ReadPin(GPIO_A, PIN1_ID))&& button2_pressed)
//		{
//			if (vehicle_lock == 0)
//			{
//				if (door_lock == 1)
//				{
//
//					// door unlocked
//					door_lock = 0;
//					// ambient light is on
//					Gpio_WritePin(GPIO_B, PIN2_ID, HIGH);
//				}
//				else
//				{
//					door_lock = 1;
//					// vehicle lock led off
//					Gpio_WritePin(GPIO_B, PIN0_ID, LOW);
//					// hazard light off
//					Gpio_WritePin(GPIO_B, PIN1_ID, LOW);
//					// ambient light on for one second
//					Gpio_WritePin(GPIO_B, PIN2_ID, HIGH);
//					GPT_StartTimer(0x2AF8);
//				}
//			}
//			// else nothing the vehicle is closed
//
//		}
//		else if(Gpio_ReadPin(GPIO_A, PIN1_ID))
//		{
//			button2_pressed = 0;
//		}
//
//		if (vehicle_lock == 1 && flag_case4)
//		{
//			if (GPT_GetElapsedTime() > 500 && GPT_GetElapsedTime() < 1000)
//			{
//				// Hazzard light OFF
//				Gpio_WritePin(GPIO_B, PIN1_ID, LOW);
//			}
//			else if (GPT_GetElapsedTime() > 1000 && GPT_GetElapsedTime() < 1500)
//			{
//				// Hazzard light ON
//				Gpio_WritePin(GPIO_B, PIN1_ID, HIGH);
//			}
//			else if (GPT_GetElapsedTime() > 1500)
//			{
//				// Hazzard light OFF
//				Gpio_WritePin(GPIO_B, PIN1_ID, LOW);
//				flag_case4 = 0;
//			}
//		}
//
//		if (vehicle_lock == 0 && door_lock == 1 && GPT_GetElapsedTime() > 1000)
//		{
//			// ambient light off
//			Gpio_WritePin(GPIO_B, PIN2_ID, LOW);
//		}
//
//		if (vehicle_lock == 0 && flag_case1)
//		{
//			if (GPT_GetElapsedTime() > 500)
//			{
//				// Hazzard light off
//				Gpio_WritePin(GPIO_B, PIN1_ID, LOW);
//			}
//			if (GPT_GetElapsedTime() > 2000)
//			{
//				// ambient lights off
//				Gpio_WritePin(GPIO_B, PIN2_ID, LOW);
//				flag_case1 = 0;
//			}
//		}
//
//		if (door_lock == 1 && vehicle_lock == 0)
//		{
//			if (GPT_GetElapsedTime() > 10000)
//			{
//				vehicle_lock = 1;
//				// Vehicle lock led OFF
//				Gpio_WritePin(GPIO_B, PIN0_ID, LOW);
//				GPT_StartTimer(0x2AF8);
//				// ambient lights OFF
//				Gpio_WritePin(GPIO_B, PIN2_ID, LOW);
//				// Hazzard light ON
//				Gpio_WritePin(GPIO_B, PIN1_ID, HIGH);
//				flag_case2 = 1;
//			}
//		}
//
//		if (vehicle_lock == 1 && door_lock == 1 && flag_case2)
//		{
//			if ((GPT_GetElapsedTime() > 500 && GPT_GetElapsedTime() < 1000))
//			{
//				// Hazzard light OFF
//				Gpio_WritePin(GPIO_B, PIN1_ID, LOW);
//			}
//			else if (GPT_GetElapsedTime() > 1000 && GPT_GetElapsedTime() < 1500)
//			{
//				// Hazzard light ON
//				Gpio_WritePin(GPIO_B, PIN1_ID, HIGH);
//			}
//			else if (GPT_GetElapsedTime() > 1500)
//			{
//				// Hazzard light OFF
//				Gpio_WritePin(GPIO_B, PIN1_ID, LOW);
//				flag_case2 = 0;
//			}
//		}
	}

	return 0;
}

