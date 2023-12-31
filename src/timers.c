#include "timers.h"

 
void init_timer2(){


	//Enable Timer2 clock
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM2EN);

    //Reset the peripheral.
    SET_BIT(RCC->APB1RSTR,  RCC_APB1RSTR_TIM2RST);
    CLEAR_BIT(RCC->APB1RSTR, RCC_APB1RSTR_TIM2RST);

    //Set prescaler to 210 => 24MHz/2400 = 10000 ticks per second
    WRITE_REG(TIM2->PSC, 2399); //+1 (look datasheet)

    //Set autoreload to 10 so 10000/10 = 1000 interrupts per second
    WRITE_REG(TIM2->ARR, 10);

    //Send an update event to reset the timer and apply settings.
    SET_BIT(TIM2->EGR, TIM_EGR_UG);
    //Enable the hardware interrupt.
    SET_BIT(TIM2->DIER, TIM_DIER_UIE);

    //Enable Timer 2
    SET_BIT(TIM2->CR1, TIM_CR1_CEN);

    //Enable the NVIC interrupt for TIM2
    NVIC_EnableIRQ(TIM2_IRQn);

}

