/*
 * utils.c
 *
 *  Created on: 11 ago. 2022
 *      Author: pablo
 */


#include "utils.h"


//------------------------------------------------------------------------------------
void modem_hw_pwr_on(void)
{
	/*
	 * Prendo la fuente del modem y espero que se estabilize la fuente.
	 */

	IO_clr_MODEM_SW();	// MODEM=0V, PWR_ON pullup 1.8V )
	IO_set_MODEM_PWR();	// Prendo la fuente ( alimento al modem ) HW

	vTaskDelay( (portTickType)( 2000 / portTICK_RATE_MS ) );

}
//------------------------------------------------------------------------------------
void modem_hw_pwr_off(void)
{
	/*
	 * Apaga el dispositivo quitando la energia del mismo
	 *
	 */

	IO_clr_MODEM_SW();	// Es un FET que lo dejo cortado
	IO_clr_MODEM_PWR();	// Apago la fuente.

	vTaskDelay( (portTickType)( 1000 / portTICK_RATE_MS ) );

}
//------------------------------------------------------------------------------------
void modem_sw_pulse(void)
{
	/*
	 * Genera un pulso en la linea PWR_SW. Como tiene un FET la senal se invierte.
	 * En reposo debe la linea estar en 0 para que el fet flote y por un pull-up del modem
	 * la entrada PWR_SW esta en 1.
	 * El PWR_ON se pulsa a 0 saturando el fet.
	 */
	IO_set_MODEM_SW();	// MODEM_SW = 3V, PWR_ON = 0V.
	vTaskDelay( (portTickType)( 1000 / portTICK_RATE_MS ) );
	IO_clr_MODEM_SW();	// MODEM_SW = 0V, PWR_ON = pullup, 1.8V

}
//------------------------------------------------------------------------------------
