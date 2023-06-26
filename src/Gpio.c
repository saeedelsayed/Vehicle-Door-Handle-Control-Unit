/**
 * Gpio.c
 *
 *  Created on: Tue Apr 11 2023
 *  Author    : Abdullah Darwish
 */

#include "Gpio.h"

#include "Gpio_Private.h"

#define GPIO_REG(REG_ID, PORT_ID) ((uint32 *)((PORT_ID) + (REG_ID)))

uint32 gpioAddresses[2] = {GPIOA_BASE_ADDR, GPIOB_BASE_ADDR};

void Gpio_ConfigPin(uint8 PortName, uint8 PinNum, uint8 PinMode,
                    uint8 DefaultState) {
  uint8 OutputState = DefaultState & 0x1;
  uint8 InputState = DefaultState >> 1;

  uint8 portId = PortName - GPIO_A;
  uint32 *gpioModerReg = GPIO_REG(GPIOx_MODER, gpioAddresses[portId]);
  uint32 *gpioOutputTypeReg = GPIO_REG(GPIOx_OTYPER, gpioAddresses[portId]);
  uint32 *gpioPullUpDownReg = GPIO_REG(GPIOx_PUPDR, gpioAddresses[portId]);
  *gpioModerReg &= ~(0x3 << (2 * PinNum));
  *gpioModerReg |= (PinMode << (2 * PinNum));
  *gpioOutputTypeReg &= ~(0x01 << PinNum);
  *gpioOutputTypeReg |= (OutputState << PinNum);
  *gpioPullUpDownReg &= ~(0x3 << (2 * PinNum));
  *gpioPullUpDownReg |= (InputState << (2 * PinNum));
}

void Gpio_WritePin(uint8 PortName, uint8 PinNum, uint8 Data) {
  uint8 portId = PortName - GPIO_A;
  uint32 *gpioOutputDataReg = GPIO_REG(GPIOx_ODR, gpioAddresses[portId]);
  *gpioOutputDataReg &= ~(0x01 << PinNum);
  *gpioOutputDataReg |= (Data << PinNum);
}

uint8 Gpio_ReadPin(uint8 PortName, uint8 PinNum) {
  uint8 data = 0;
  uint8 portId = PortName - GPIO_A;
  uint32 *gpioInputDataReg = GPIO_REG(GPIOx_IDR, gpioAddresses[portId]);
  data = ((*gpioInputDataReg) & (1 << PinNum)) >> PinNum;
  return data;
}
