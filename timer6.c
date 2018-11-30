#include "stm32f7xx_hal.h"
#include "timer6.h"

bool isTextErrChangable, isButton;

extern void ModeChanging(void);
extern void ModeChanged(void);

void InitTimer6(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN; // turn-on timer6on apb1 registers
	TIM6->PSC = 54000 - 1; // how much times (friquency / value) will generate clock
	TIM6->ARR = 12000; // max value, after that - reset
	TIM6->SR &= ~TIM_SR_UIF; // flag, which reports that timer filled, turn off
	//
	NVIC_EnableIRQ(TIM6_DAC_IRQn); 	// check interrupt for TIM6 on
	TIM6->DIER |= TIM_DIER_UIE; 		// fill timer event on
	//
	RCC->DCKCFGR1 |= RCC_DCKCFGR1_TIMPRE;
	TIM6->CR1 |= TIM_CR1_CEN; // первое включение, почему-то без него не желает работать(сразу выключается, не проработав 3 секунды)
}

void TIM6_DAC_IRQHandler(void)
{
	if (isButton)
		TimerStopForButton();
	else
		TimerStop();
}

void TimerStart(void) 
{
	isButton = false;
	isTextErrChangable = false;
	TIM6->CR1 |= TIM_CR1_CEN; // timer on
}

void TimerStartForButton(void)
{
	isButton = true;
	TIM6->CR1 |= TIM_CR1_CEN; // timer on   
	ModeChanging();
}

void TimerStopForButton(void)
{
	TIM6->CR1 &= ~TIM_CR1_CEN; // timer off
	TIM6->SR &= ~TIM_SR_UIF; // reset interrupt fill-flag in timer6
	TIM6->CNT = 0;
	ModeChanged();
}


void TimerStop(void)
{
	TIM6->CR1 &= ~TIM_CR1_CEN; // timer off
  TIM6->SR &= ~TIM_SR_UIF; // reset interrupt fill-flag in timer6
	TIM6->CNT = 0;
	isTextErrChangable = true;
}
