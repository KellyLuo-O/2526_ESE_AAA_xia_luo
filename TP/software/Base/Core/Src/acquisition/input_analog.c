/*
 * analog_input.c
 *
 *  Created on: Nov 11, 2025
 *      Author: nicolas
 */

#include "acquisition/input_analog.h"
#include "adc.h"
#include "tim.h"

#define PWM_ARR 4250

#define ADC_BUF_SIZE  1

uint16_t adc_buffer[ADC_BUF_SIZE];

uint16_t adc_current_value = 0;


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if (hadc->Instance == ADC1)
	{
		adc_current_value = adc_buffer[0];
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
	}
}

// Injected ADC (trigger TRGO2)
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc->Instance == ADC1)
	{
		adc_current_value = adc_buffer[0];
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
	}
}

void input_analog_init()
{
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, PWM_ARR/2);
	HAL_TIM_OC_Start(&htim1, TIM_CHANNEL_4);

	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buffer, ADC_BUF_SIZE);

	shell_add(&hshell1, "currentRead", current_read, "Mesure de courant");
}

int current_read(h_shell_t* h_shell, int argc, char** argv)
{
	int size;
	if(argc!=1){
		size = snprintf(h_shell->print_buffer, SHELL_PRINT_BUFFER_SIZE, "usage : currentRead\r\n");
		h_shell->drv.transmit(h_shell->print_buffer, size);
		return HAL_ERROR;
	}

	//	HAL_ADC_Start(&hadc1);
	//	if(HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK){
	//		adc_value = HAL_ADC_GetValue(&hadc1);
	//	}
	//	HAL_ADC_Stop(&hadc1);

	float adc_current_analog_value = adc_current_value * 3.3f / (4096 - 1);
	float current = (adc_current_analog_value - 1.65f) / 0.05f;


	size = snprintf(h_shell->print_buffer, SHELL_PRINT_BUFFER_SIZE, "current read : %.2f A\r\n", current);
	h_shell->drv.transmit(h_shell->print_buffer, size);

	return HAL_OK;
}
