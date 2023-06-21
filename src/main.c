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
	for (i; i < 3; i++)
	{
		Gpio_ConfigPin(GPIO_B, output_pins[i], GPIO_OUTPUT, GPIO_PUSH_PULL);
		Gpio_WritePin(GPIO_B, output_pins[i], LOW);
	}

	// 1 means it's locked 0 is unlocked
	uint8 door_lock = 1;
	uint8 vehicle_lock = 1;

	uint8 button1_pressed = 0, button2_pressed = 0, mode = -1;
	uint32 time_elapsed = 0;
	uint8 auto_reload = 0;
	uint32 counter = 0;

	while (1)
	{

		// vehicle unlock button pressed
		if (!(Gpio_ReadPin(GPIO_A, PIN0_ID)) && !button1_pressed)
		{

			button1_pressed = 1;

			if (vehicle_lock == 1)
			{
				mode = 1;
				// vehicle unlocked
				vehicle_lock = 0;
				// Vehicle lock led on
				Gpio_WritePin(GPIO_B, PIN0_ID, HIGH);
				// Hazzard light
				Gpio_WritePin(GPIO_B, PIN1_ID, HIGH);
				// ambient lights
				Gpio_WritePin(GPIO_B, PIN2_ID, HIGH);

				// 500 ms

				GPT_StartTimer(0x1F4);
				counter = 0;
			}
			else
			{

				mode = 4;
				Gpio_WritePin(GPIO_B, PIN0_ID, LOW);
				vehicle_lock = 1;
				// hazard light blinking 2 times
				Gpio_WritePin(GPIO_B, PIN1_ID, HIGH);
				GPT_StartTimer(0x1F4);
				counter = 0;
				// ambient light is off
				Gpio_WritePin(GPIO_B, PIN2_ID, LOW);
			}
		}
		else if (Gpio_ReadPin(GPIO_A, PIN0_ID))
		{
			button1_pressed = 0;
		}

		if (!(Gpio_ReadPin(GPIO_A, PIN1_ID)) && !button2_pressed)
		{
			if (vehicle_lock == 0)
			{
				if (door_lock == 1)
				{
					mode = 2;
					// door unlocked
					door_lock = 0;
					// ambient light is on
					Gpio_WritePin(GPIO_B, PIN2_ID, HIGH);
				}
			}
			if (door_lock == 0)
			{
				mode = 3;
				door_lock = 1;
				// vehicle lock led off
				Gpio_WritePin(GPIO_B, PIN0_ID, LOW);
				// hazard light off
				Gpio_WritePin(GPIO_B, PIN1_ID, LOW);
				// ambient light on for one second
				Gpio_WritePin(GPIO_B, PIN2_ID, HIGH);
				GPT_StartTimer(0x1F4);
				counter = 0;
			}
			// else nothing the vehicle is closed
		}
		else if (Gpio_ReadPin(GPIO_A, PIN1_ID))
		{
			button2_pressed = 0;
		}


		if (vehicle_lock == 0 && door_lock == 1 && mode == 1)
		{
			auto_reload = GPT_CheckTimeIsElapsed();

			if (auto_reload == 1)
				counter++;

			// Gpio_WritePin(GPIO_B, PIN1_ID, LOW);
			if (counter == 1)
			{
				// Hazzard light off
				Gpio_WritePin(GPIO_B, PIN1_ID, LOW);
			}
			if (counter == 4)
			{
				// ambient lights off
				Gpio_WritePin(GPIO_B, PIN2_ID, LOW);
			}
			if (counter == 20)
			{
				vehicle_lock = 1;
				mode = 0;
				// Vehicle lock led OFF
				Gpio_WritePin(GPIO_B, PIN0_ID, LOW);
				// ambient lights OFF
				Gpio_WritePin(GPIO_B, PIN2_ID, LOW);

				// Hazzard light ON
				Gpio_WritePin(GPIO_B, PIN1_ID, HIGH);
				GPT_StartTimer(0x1F4);
				counter = 0;
			}
		}

		if (vehicle_lock == 0 && door_lock == 1 && mode == 3)
		{
			auto_reload = GPT_CheckTimeIsElapsed();
			if (auto_reload == 1)
				counter++;
			if (counter == 2)
			{
				Gpio_WritePin(GPIO_B, PIN2_ID, LOW);
				mode = 1;
			}
		}


		if (vehicle_lock == 1 && door_lock == 1 && (mode == 0 || mode == 4))
		{
			if (GPT_CheckTimeIsElapsed() == 1)
				counter++;
			if (counter == 1)
			{
				// Hazzard light OFF
				Gpio_WritePin(GPIO_B, PIN1_ID, LOW);
			}
			else if (counter == 2)
			{
				// Hazzard light ON
				Gpio_WritePin(GPIO_B, PIN1_ID, HIGH);
			}
			else if (counter == 3)
			{
				// Hazzard light OFF
				Gpio_WritePin(GPIO_B, PIN1_ID, LOW);
				mode = -1;
			}
		}
	}

	return 0;
}

