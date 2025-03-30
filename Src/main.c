/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */


void buttonB1Init(){
	__HAL_RCC_GPIOA_CLK_ENABLE();
	*GPIOA_MODER &= ~(0b11 << 0);
	*GPIOA_MODER |= (0b00 << 0);

	*GPIOA_PUPDR &= ~(0b11 << 0);
	*GPIOA_PUPDR |= (0b10 << 0);
}

void buttonControl(){
	while(*GPIOA_IDR & 0x01){
		LED_Control(LED3_PIN, 1);
		HAL_Delay(100);
		LED_Control(LED4_PIN, 1);
		HAL_Delay(100);
		LED_Control(LED5_PIN, 1);
		HAL_Delay(100);
		LED_Control(LED6_PIN, 1);
		HAL_Delay(100);
		LED_Control(LED3_PIN, 0);
		LED_Control(LED4_PIN, 0);
		LED_Control(LED5_PIN, 0);
		LED_Control(LED6_PIN, 0);
		HAL_Delay(100);
	}
		LED_Control(LED3_PIN, 0);
		LED_Control(LED4_PIN, 0);
		LED_Control(LED5_PIN, 0);
		LED_Control(LED6_PIN, 0);
}
int main(void){
	HAL_Init();
	LEDs_Init();
	buttonB1Init();

	while(1){
		buttonControl();
	}
}
