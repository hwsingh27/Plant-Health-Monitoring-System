/*
 * @file: systick.c
 *
 * @author: Harshwardhan Singh
 * @date: November 29th 2021
 *
 * @tools: MCUXpresso IDE and FRDM-KL25Z Development Board
 * @Credits: Embedded Systems Fundamentals with
 * Arm Cortex-M based Microcontrollers (Textbook)
 * by Alexander G.Dean.
 */
#include "common.h"

static volatile ticktime_t reset=0;
static volatile ticktime_t timer=0;

/**
 * @brief: this Init function is used to configure the clock
 * by loading the counter value as per the requirement.
 * Here 10msec has been taken as 1 count value, hence load
 * value becomes (0.01 x 48000000)/16
 *
 * @param: NULL
 * @return: NULL
 */
void Init_Systick()
{
	SysTick->LOAD = (48000000/16)/100; 	//10msecx48Mhz/16
	NVIC_SetPriority(SysTick_IRQn,1);  	//interrupt priority is set to 1
	SysTick->VAL=0;					   	//force load of reload value
	SysTick->CTRL=SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;	//enable interrupt and systick timer with alternative clock source
}

/**
 * @brief: function becomes active as soon as the Systick is enabled
 * keeps incrementing the values of timer and counter.
 *
 * @param: NULL
 * @return: NULL
 */
void SysTick_Handler()
{
	timer++;
}

/**
 * @brief: function checks if the capacitive sensor's
 * value if greater than the threshold.
 *
 * @param: NULL
 * @return: gives the current time since startup by returning
 * the value of timer. To get the correct time, timer should be multipled
 * by 10 since one count equals 10msec.
 */
ticktime_t Now()
{
	return timer;
}

/**
 * @brief: function stores the current time in reset variable.
 *
 * @param: NULL
 * @return: NULL
 */
void Reset_Timer()
{
	reset=Now();
}

/**
 * @brief: function return the time passed since the last
 * call to reset
 *
 * @param: NULL
 * @return: returns ticks since the last call to Reset_Timer()
 */
ticktime_t Get_Timer()
{
	return (timer-reset);
}
