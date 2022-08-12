/*
 * utils.h
 *
 *  Created on: 11 ago. 2022
 *      Author: pablo
 */

#ifndef SRC_SPX_ULIBS_UTILS_H_
#define SRC_SPX_ULIBS_UTILS_H_

#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

#include "FreeRTOS.h"
#include "task.h"

#include "iopines.h"

void modem_hw_pwr_on(void);
void modem_hw_pwr_off(void);
void modem_sw_pulse(void);



#endif /* SRC_SPX_ULIBS_UTILS_H_ */
