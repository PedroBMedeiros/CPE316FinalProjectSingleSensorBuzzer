/*
 * timer.c
 *
 *  Created on: Nov 25, 2024
 *      Author: pedro
 */

#include "main.h"
#include "timer.h"


void Timer_Delay_Init(void) {
    // Enable timer 2 clock
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN;

    // Configure TIM3 for microsecond delay
    TIM3->PSC = 32-1;
    TIM3->ARR = 10; // Auto-reload register (max delay if needed)
    TIM3->CNT = 0;      // Reset counter
    TIM3->CR1 |= TIM_CR1_OPM; // One-pulse mode: Timer stops after reaching ARR
}

void Start_Delay_Timer(void) {
	TIM3->CR1 |= TIM_CR1_CEN;
}

void Wait_Timer(void) {
    // Poll the update event flag (UIF) to wait for the timer to complete
    while (!(TIM3->SR & TIM_SR_UIF)) {
        // Wait for the timer to update
    }
    // Clear the update event flag
    TIM3->SR &= ~TIM_SR_UIF;
}


void Timer_Input_Capture_Init(void) {
    // Enable clock for TIM2 and GPIOC
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN; // Enable TIM2 clock

    // Configure PC6 as alternate function (AF2 for TIM2 CH1)
    GPIOA->MODER &= ~(GPIO_MODER_MODE0); // Clear MODER for PC6
    GPIOA->MODER |= (GPIO_MODER_MODE0_1);  // Set PC6 to alternate function mode
    GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL0); // Set AF2 for TIM2_CH1 (PC6)
    GPIOA->AFR[0] |= (1 << GPIO_AFRL_AFSEL0_Pos);

    //GPIOA->MODER &= ~GPIO_MODER_MODE7;    // Clear mode for PC7
    //GPIOA->MODER |= GPIO_MODER_MODE7_1;   // Set PC7 to Alternate Function mode
    //GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL7);
    //GPIOA->AFR[0] |= (2 << GPIO_AFRL_AFSEL7_Pos); // Set AF2 (TIM2_CH2) for PC7

    // Configure TIM2 for input capture on CH1
    TIM2->PSC = 0;              // No prescaler, timer runs at 32 MHz
    TIM2->ARR = 0xFFFFFFFF;     // Max ARR for long timing intervals
    TIM2->CCMR1 &= ~(TIM_CCMR1_CC1S);
    TIM2->CCMR1 |= (TIM_CCMR1_CC1S_0);   // Set CC1 channel to input mode (TI1)
    TIM2->CCER &= ~TIM_CCER_CC1P;        // Capture on rising edge (default)
    TIM2->CCER &= ~TIM_CCER_CC1NP;       // Ensure no inverted capture

    // Config for falling edge on CH2
    //TIM2->CCMR1 &= ~TIM_CCMR1_CC2S;      // Clear channel 2 selection bits
    //TIM2->CCMR1 |= TIM_CCMR1_CC2S_0;     // CC2 channel is mapped to input (TI2)

    //TIM2->CCER |= TIM_CCER_CC2P;         // Capture on falling edge
    //TIM2->CCER &= ~TIM_CCER_CC2NP;       // Ensure no inverted capture
    // -----------------------------------------

    //TIM2->CCER |= TIM_CCER_CC2E;         // Enable capture for channel 2
    TIM2->CCER |= TIM_CCER_CC1E; // Enable capture on rising edge


    TIM2->CR1 &= ~(TIM_CR1_OPM);

    TIM2->DIER |= TIM_DIER_CC1IE;        // Enable interrupt for CC1
    //TIM2->DIER |= TIM_DIER_CC2IE;        // Enable interrupt for CC2
    NVIC_EnableIRQ(TIM2_IRQn);           // Enable TIM2 interrupt in NVIC

    TIM2->CR1 |= TIM_CR1_CEN;   // Enable TIM2
}
