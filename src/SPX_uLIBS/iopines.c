/*
 * iopines.c
 *
 *  Created on: 11 ago. 2022
 *      Author: pablo
 */

#include "iopines.h"

//------------------------------------------------------------------------------------
void iopines_config(void)
{
	IO_config_TICK();
	IO_config_LED_KA();
	IO_config_MODEM_PWR();
	IO_config_MODEM_SW();

}
//------------------------------------------------------------------------------------
