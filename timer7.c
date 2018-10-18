#include "stm32f7xx_hal.h"
#include "timer7.h"
#include "global_var.h"

void InitTimer7(void) // �������� - 2 ���� � �������
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN; // �������� ������� 7 ������� � �������� apb1
	TIM7->PSC = 54000 - 1; // how much times (friquency / value) will generate clock
	TIM7->ARR = 2000; // 4000 / 2/; // ������������ ��������, ����� �������� �����
	TIM7->SR &= ~TIM_SR_UIF; // �������� ����, ������������ ��� ������ ����������
	//
	NVIC_EnableIRQ(TIM7_IRQn); 	// �������� ��������� ���������� � 7 �������
	TIM7->DIER |= TIM_DIER_UIE; 		// �������� ������� ���������� �������
	//
	//RCC->DCKCFGR1 |= RCC_DCKCFGR1_TIMPRE; // "�������", ������� ���� ������� ������� 216 MHz
}

void TIM7_IRQHandler(void)
{
	Flags.answer_server = 1;
	TIM7->SR &= ~TIM_SR_UIF; 
}

void Timer7Start(void) 
{
	TIM7->CR1 |= TIM_CR1_CEN; // ��������� ������� 7
}

void Timer7Stop(void)
{
	TIM7->CR1 &= ~TIM_CR1_CEN; // ���������� ������� 7
	TIM7->SR &= ~TIM_SR_UIF; 
	TIM7->CNT = 0;
}
