/*
 * spx_tkCtl.c
 *
 *  Created on: 4 de oct. de 2017
 *      Author: pablo
 *
 */

#include "spx.h"

//------------------------------------------------------------------------------------

static void pv_ctl_wink_led(void);

//------------------------------------------------------------------------------------
void tkCtl(void * pvParameters)
{

	// Esta es la primer tarea que arranca.

( void ) pvParameters;


	vTaskDelay( ( TickType_t)( 100 / portTICK_RATE_MS ) );
	xprintf_P( PSTR("\r\nstarting tkControl..\r\n"));

	vTaskDelay( ( TickType_t)( 1000 / portTICK_RATE_MS ) );
	xprintf_P( PSTR("Spymovil SRL.\r\n"));

	for( ;; )
	{
		WDT_Reset();

		pv_ctl_wink_led();

		vTaskDelay( ( TickType_t)( 1000 / portTICK_RATE_MS ) );

	}
}
//------------------------------------------------------------------------------------
static void pv_ctl_wink_led(void)
{


	// Prendo los leds
	IO_set_LED_KA();
	vTaskDelay( ( TickType_t)( 10 ) );
	// Apago
	IO_clr_LED_KA();

}
//------------------------------------------------------------------------------------
