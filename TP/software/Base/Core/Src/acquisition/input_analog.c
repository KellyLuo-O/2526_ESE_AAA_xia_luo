/*
 * analog_input.c
 *
 *  Created on: Nov 11, 2025
 *      Author: nicolas
 */

#include "acquisition/input_analog.h"
#include "adc.h"
#include "tim.h"


void input_analog_init()
{
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
	HAL_TIM_OC_Start(&htim1, TIM_CHANNEL_4);

	shell_add(&hshell1, "currentRead", current_read, "Mesure de courant");
}

int current_read(h_shell_t* h_shell, int argc, char** argv)
{
	int size;
	uint32_t adc_value;

	if(argc!=1){
		size = snprintf(h_shell->print_buffer, SHELL_PRINT_BUFFER_SIZE, "usage : currentRead\r\n");
		h_shell->drv.transmit(h_shell->print_buffer, size);
		return HAL_ERROR;
	}

	HAL_ADC_Start(&hadc1);
	if(HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK){
		adc_value = HAL_ADC_GetValue(&hadc1);
	}
	HAL_ADC_Stop(&hadc1);


	size = snprintf(h_shell->print_buffer, SHELL_PRINT_BUFFER_SIZE, "Analog value : %lu\r\n", adc_value);
	h_shell->drv.transmit(h_shell->print_buffer, size);

	return HAL_OK;
}
