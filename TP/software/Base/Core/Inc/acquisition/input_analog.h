/*
 * input_analog.h
 *
 *  Created on: Nov 11, 2025
 *      Author: nicolas
 */

#ifndef INC_INPUT_ANALOG_H_
#define INC_INPUT_ANALOG_H_

#include "user_interface/shell.h"

void input_analog_init();
int current_read(h_shell_t* h_shell, int argc, char** argv);


#endif /* INC_INPUT_ANALOG_H_ */
