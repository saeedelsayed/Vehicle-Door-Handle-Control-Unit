/**
 * Gpio.h
 *
 *  Created on: Tue Apr 11 2023
 *  Author    : Abdullah Darwish
 */

#ifndef GPIO_H
#define GPIO_H
#include "Std_Types.h"

#define GPIO_A 'A'
#define GPIO_B 'B'
#define GPIO_C 'C'
#define GPIO_D 'D'
#define GPIO_E 'E'
#define GPIO_h 'H'


#define GPIO_INPUT  0x00
#define GPIO_OUTPUT 0x01
#define GPIO_AF     0x02
#define GPIO_ANALOG 0x03


#define GPIO_PUSH_PULL  0x00
#define GPIO_OPEN_DRAIN 0x01


#define LOW    0x00   
#define HIGH   0x01

#define PIN0_ID                    0
#define PIN1_ID                    1
#define PIN2_ID                    2
#define PIN3_ID                    3
#define PIN4_ID                    4
#define PIN5_ID                    5
#define PIN6_ID                    6
#define PIN7_ID                    7
#define PIN8_ID                    8
#define PIN9_ID                    9
#define PIN10_ID                   10
#define PIN11_ID                   11
#define PIN12_ID                   12
#define PIN13_ID                   13
#define PIN14_ID                   14
#define PIN15_ID                   15


#define GPIO_NO_PULL   (0x00 << 1)
#define GPIO_PULL_UP   (0x01 << 1)
#define GPIO_PULL_DOWN (0x02 << 1)

void Gpio_ConfigPin(uint8 PortName, uint8 PinNum, uint8 PinMode, uint8 DefaultState);
void Gpio_WritePin(uint8 PortName, uint8 PinNum, uint8 Data);
uint8 Gpio_ReadPin(uint8 PortName, uint8 PinNum);

#endif /* GPIO_H */
