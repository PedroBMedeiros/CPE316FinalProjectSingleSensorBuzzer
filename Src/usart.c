#include "main.h"
#include "usart.h"


void USART2_init(void) {
	// RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN); 				// enable GPIOA clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN; 			// enable LPUART clock

	// Configure GPIOA registers
	// select AF mode
	GPIOA->MODER &= ~(GPIO_MODER_MODE2); 		// clear mode bits
	GPIOA->MODER |= (GPIO_MODER_MODE2_1); 		// set to Alternate mode
	GPIOA->MODER &= ~(GPIO_MODER_MODE3); 		// clear mode bits
	GPIOA->MODER |= (GPIO_MODER_MODE3_1); 		// set to alternate mode

	// set PA2 and PA3 to AF7 for USART2
	GPIOA->AFR[0] &= ~(ZEROAF << GPIO_AFRL_AFSEL2_Pos);
	GPIOA->AFR[0] |= (SETAF7 << GPIO_AFRL_AFSEL2_Pos);
	GPIOA->AFR[0] &= ~(ZEROAF << GPIO_AFRL_AFSEL3_Pos);
	GPIOA->AFR[0] |= (SETAF7 << GPIO_AFRL_AFSEL3_Pos);


	USART2->CR1 &= ~USART_CR1_UE; 					// disable USART for configs
	USART2->BRR = BAUDRATE; 				// set baud rate
	USART2->CR1 |= (USART_CR1_TE | USART_CR1_RE); 	// Enable transmitter and receiver
	USART2->CR1 &= ~(USART_CR1_M | USART_CR1_PCE); 	// 8 data bits; no parity
	USART2->CR1 |= USART_CR1_UE; 					// enable USART
	USART2->CR1 |= USART_CR1_RXNEIE; 				// enable interrupt reading
}


// Function for printing any character received
void USART2_Print(const char *string) {
	uint16_t iStrIdx = 0;							// index for iterating through list
	while (string[iStrIdx] != 0) {					// iterate through until reach end of list
		while (!(USART2->ISR & USART_ISR_TXE));		// wait until data register is ready for data
		USART2->TDR = string[iStrIdx];				// transmit data register equal to item in string
		iStrIdx++;									// increment index
	}
	while (!(USART2->ISR & USART_ISR_TXE));			// ensure register is ready for data
	USART2->TDR = RESETTRANSMISSION;								// reset
}


void USART_ESC_Print(const char *message) {
	uint16_t iStrIdx = 0;							// index for iterating
	while (!(USART2->ISR & USART_ISR_TXE));			// ensure register is ready for data
	USART2->TDR = ESCASCII;							// send ESC character in hex
	while (!(USART2->ISR & USART_ISR_TXE));			// ensure register is ready for data
	USART2->TDR = CLOSEBRACKETASCII;				// send ']' in hex
	while (message[iStrIdx] != 0) {					// loop until all characters in string have been sent
		while (!(USART2->ISR & USART_ISR_TXE));
		USART2->TDR = message[iStrIdx];
		iStrIdx++;
	}
	while (!(USART2->ISR & USART_ISR_TXE));
	USART2->TDR = 0;
}

