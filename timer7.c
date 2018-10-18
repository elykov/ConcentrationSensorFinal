#include "stm32f7xx_hal.h"
#include "timer7.h"
#include "global_var.h"

void InitTimer7(void) // скорость - 2 раза в секунду
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN; // включить питание 7 таймера в регистре apb1
	TIM7->PSC = 54000 - 1; // how much times (friquency / value) will generate clock
	TIM7->ARR = 2000; // 4000 / 2/; // максимальное значение, посде которого сброс
	TIM7->SR &= ~TIM_SR_UIF; // сбросить флаг, показывающий что таймер заполнился
	//
	NVIC_EnableIRQ(TIM7_IRQn); 	// включить поддержку прерываний в 7 таймере
	TIM7->DIER |= TIM_DIER_UIE; 		// включить событие заполнения таймера
	//
	//RCC->DCKCFGR1 |= RCC_DCKCFGR1_TIMPRE; // "костыль", который дает таймеру частоту 216 MHz
}

void TIM7_IRQHandler(void)
{
	Flags.answer_server = 1;
	TIM7->SR &= ~TIM_SR_UIF; 
}

void Timer7Start(void) 
{
	TIM7->CR1 |= TIM_CR1_CEN; // включение таймера 7
}

void Timer7Stop(void)
{
	TIM7->CR1 &= ~TIM_CR1_CEN; // выключение таймера 7
	TIM7->SR &= ~TIM_SR_UIF; 
	TIM7->CNT = 0;
}
