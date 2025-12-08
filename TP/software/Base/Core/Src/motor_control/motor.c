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

float dutyCycle = 0.0f;

void motor_init(void)
{

	__HAL_TIM_SET_AUTORELOAD(&htim1, PWM_ARR);

	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint32_t)(dutyCycle * PWM_ARR));
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (uint32_t)((1 - dutyCycle) * PWM_ARR));


	shell_add(&hshell1, "speed", motor_speed, "Control motor speed (tr/min)");
	shell_add(&hshell1, "dutycycle", motor_dutycycle, "Control duty cycle (0-100)");
	shell_add(&hshell1, "motorStart", motor_start, "Start les PWM");
	shell_add(&hshell1, "motorStop", motor_stop, "Stop les PWM");
}


int motor_speed(h_shell_t* h_shell, int argc, char** argv)
{
	int size;
	int speedValue;

	if(argc!=2){
		size = snprintf(h_shell->print_buffer, SHELL_PRINT_BUFFER_SIZE, "Need 2 arguments : speed ###\r\n");
		h_shell->drv.transmit(h_shell->print_buffer, size);
		return HAL_ERROR;
	}

	// on vérifie que l'argument est bien un nombre
	char* str = argv[1];
	int reading_head = 0;
	while(str[reading_head] != '\0')
	{
		char c = str[reading_head];
		if(!(c >= '0' && c <= '9'))
		{
			size = snprintf(h_shell->print_buffer, SHELL_PRINT_BUFFER_SIZE, "2nd arguments isn't number\r\n");
			h_shell->drv.transmit(h_shell->print_buffer, size);
			return HAL_ERROR;
		}
		reading_head++;
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


int motor_dutycycle(h_shell_t* h_shell, int argc, char** argv)
{
	int size;
	int value;

	if(argc!=2){
		size = snprintf(h_shell->print_buffer, SHELL_PRINT_BUFFER_SIZE, "Need 2 arguments : dutycycle ###\r\n");
		h_shell->drv.transmit(h_shell->print_buffer, size);
		return HAL_ERROR;
	}

	// on vérifie que l'argument est bien un nombre
	char* str = argv[1];
	int reading_head = 0;
	while(str[reading_head] != '\0')
	{
		char c = str[reading_head];
		if(!(c >= '0' && c <= '9'))
		{
			size = snprintf(h_shell->print_buffer, SHELL_PRINT_BUFFER_SIZE, "2nd arguments isn't number\r\n");
			h_shell->drv.transmit(h_shell->print_buffer, size);
			return HAL_ERROR;
		}
		reading_head++;
	}

	value = atoi(argv[1]);

	if(value > 100 || value < 0){
		size = snprintf(h_shell->print_buffer, SHELL_PRINT_BUFFER_SIZE, "duty cycle valeur de 0 à 100\r\n");
		h_shell->drv.transmit(h_shell->print_buffer, size);
		return HAL_ERROR;
	}
	dutyCycle = (float)value/ (float)100.0f;
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint32_t)(dutyCycle * PWM_ARR));
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (uint32_t)((1 - dutyCycle) * PWM_ARR));

	size = snprintf(h_shell->print_buffer, SHELL_PRINT_BUFFER_SIZE, "DutyCycle %.2f\r\n", dutyCycle);
	h_shell->drv.transmit(h_shell->print_buffer, size);
	return HAL_ERROR;
}


int motor_start(h_shell_t* h_shell, int argc, char** argv)
{
	int size;

	if(argc != 1){
		size = snprintf(h_shell->print_buffer, SHELL_PRINT_BUFFER_SIZE, "usage : motorStart\r\n");
		h_shell->drv.transmit(h_shell->print_buffer, size);
		return HAL_ERROR;
	}

	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);

	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
}


int motor_stop(h_shell_t* h_shell, int argc, char** argv)
{
	int size;

	if(argc != 1){
		size = snprintf(h_shell->print_buffer, SHELL_PRINT_BUFFER_SIZE, "usage : motorStop\r\n");
		h_shell->drv.transmit(h_shell->print_buffer, size);
		return HAL_ERROR;
	}

	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
}
