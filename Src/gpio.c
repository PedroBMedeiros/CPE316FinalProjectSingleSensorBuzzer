#include "main.h"
#include "gpio.h"

void GPIO_PC0_Init(void) { // pc0 will act as 'echo' input from sensor
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN; // enable clock for GPIO C
	// set as digital input
	GPIOC->MODER &= ~(GPIO_MODER_MODE0); // MODER = 00
	// OTYPER = 0
	GPIOC->OSPEEDR &= ~(3 << 0); // SPEEDR = 11
	GPIOC->OSPEEDR |= (3 << 0); // SPEEDR = 11
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD0); // PUPDR = 00 ??????????
}

void GPIO_PC1_Init(void) { // pc1 will act as 'trig' output from sensor
	// set as digital input
	GPIOC->MODER &= ~(GPIO_MODER_MODE1); // MODER = 01
	GPIOC->MODER |= (GPIO_MODER_MODE1_0); // MODER = 01
	// OTYPER = 0
	GPIOC->OSPEEDR &= ~(3 << 2); // SPEEDR = 11
	GPIOC->OSPEEDR |= (3 << 2); // SPEEDR = 11
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD1); // PUPDR = 00 ??????????
	GPIOC->ODR &= ~(GPIO_ODR_OD1); // set  initial output to 0
}

