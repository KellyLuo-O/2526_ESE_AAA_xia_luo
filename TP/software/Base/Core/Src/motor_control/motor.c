/*
 * motor.c
 *
 *  Created on: Nov 11, 2025
 *      Author: nicolas
 */
#include <stdint.h>
#include "tim.h"
#include "motor_control/motor.h"


#define PWM_ARR 4250-1

void motor_init(void)
{
	float dutyCycle = 0.6f;

	HAL_TIM_Base_Start(&htim1);		// commence le timer

	// changer ARR
	__HAL_TIM_SET_AUTORELOAD(&htim1, PWM_ARR);

	// changer la valeur du rapport cyclique de la PWM
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint32_t)(dutyCycle * PWM_ARR));
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (uint32_t)((1 - dutyCycle) * PWM_ARR));

	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);

	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);

}
