/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include<stdio.h>
#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

void generate_ISR(){

	uint32_t *pISER0 = (uint32_t *)0xE000E100;
	uint32_t *pSTIR  = (uint32_t *)0xE000EF00;
	
	*pISER0 |=(1<<3);	//enabling interrupt 3 i.e. RTC wakeup
	*pSTIR   = 0x03;	//triggering RTC wakeup interrupt through software.

}

void RTC_WKUP_IRQHandler(void){
/*
 * Bit 1 in SCR decide to go in sleep mode or not
 * 		0= no sleep mode when getting back to Thread Mode.
 * 		1= enter sleep mode when returning from Handler Mode.
 *
 * Bit 2 decide what type of sleep mode
 * 		0= sleep mode.
 * 		1= deep sleep mode.
 */
	uint32_t *pSCR = (uint32_t *)0xE000ED10;	// System Control Register
	*pSCR|= (1<<2);	//deep sleep mode
	*pSCR|= (1<<1);
}

int main(void){
uint32_t *pRCC		=	(uint32_t *)0x40023830;
uint32_t *pModerPA	=	(uint32_t *)0x40020000;
uint32_t *pModerPD	=	(uint32_t *)0x40020C00;
uint32_t *pInPA		=	(uint32_t *)0x40020010;
uint32_t *pOutPD	=	(uint32_t *)0x40020C14;


	*pRCC 	|= 	(1<<0 | 1<<3); //enabling clock on PortA and PortB

	//*pModerPA |= (00<<0);	//setting PA0 as input(doesn't required as by default it is INPUT mode)

	*pModerPD |= (01<<24);	//PD12 as OUTPUT

	*pOutPD |= (1<<12);	//LED turn on

	printf("In Thread Mode.\n");

while(1){
	// loop forever
	if(*pInPA & 0x01)	//read bit status on PA0
	{
		*pOutPD	&= ~(1<<12);	//LED turn off
		generate_ISR();	//for software triggering of interrupt
	}
		}
}
