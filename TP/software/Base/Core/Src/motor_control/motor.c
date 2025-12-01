/*
 * motor.c
 *
 *  Created on: Nov 11, 2025
 *      Author: nicolas
 */
#include <stdlib.h>
#include "tim.h"
#include "motor_control/motor.h"


#define PWM_ARR 4250-1

#define MOTOR_SPEED_MAX 3000

float dutyCycle = 0.6f;

void motor_init(void)
{

	__HAL_TIM_SET_AUTORELOAD(&htim1, PWM_ARR);

	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint32_t)(dutyCycle * PWM_ARR));
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (uint32_t)((1 - dutyCycle) * PWM_ARR));

	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);

	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);

	shell_add(&hshell1, "speed", motor_control, "Control motor speed (tr/min)");
}


int motor_control(h_shell_t* h_shell, int argc, char** argv)
{
	int size;
	int speedValue;

	if(argc!=2){
		size = snprintf(h_shell->print_buffer, SHELL_PRINT_BUFFER_SIZE, "Need 2 arguments : speed ###\r\n");
		h_shell->drv.transmit(h_shell->print_buffer, size);
		return HAL_ERROR;
	}

	speedValue = atoi(argv[1]);

	if(speedValue > MOTOR_SPEED_MAX){
		size = snprintf(h_shell->print_buffer, SHELL_PRINT_BUFFER_SIZE, "speed exceeding max value : 3000 tours/min\r\n");
		h_shell->drv.transmit(h_shell->print_buffer, size);
		return HAL_ERROR;
	}
	dutyCycle = (float)speedValue / (float)MOTOR_SPEED_MAX;
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint32_t)(dutyCycle * PWM_ARR));
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (uint32_t)((1 - dutyCycle) * PWM_ARR));

	size = snprintf(h_shell->print_buffer, SHELL_PRINT_BUFFER_SIZE, "Speed %d tr/min ==> DutyCycle %.2f\r\n", speedValue, dutyCycle);
	h_shell->drv.transmit(h_shell->print_buffer, size);
	return HAL_ERROR;
}
