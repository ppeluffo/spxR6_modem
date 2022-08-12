/*
 * spx_tkModem.c
 *
 *  Created on: 11 ago. 2022
 *      Author: pablo
 */


#include "spx.h"
#include "utils.h"

#define MAX_MODEM_INPUT_LENGTH		 256
char modem_buffer[MAX_MODEM_INPUT_LENGTH];
uint16_t modem_ptr;

void pv_MODEM_init( void );
void pv_MODEM_process( char cRxedChar );
void pv_MODEM_execute(void);

//-----------------------------------------------------------------------------
void tkModem(void * pvParameters)
{

	// Esta es la primer tarea que arranca.

( void ) pvParameters;
uint8_t c = 0;

	vTaskDelay( ( TickType_t)( 100 / portTICK_RATE_MS ) );
	xprintf_P( PSTR("\r\nstarting tkModem..\r\n"));

	pv_MODEM_init();

	for( ;; )
	{
		//vTaskDelay( ( TickType_t)( 1000 / portTICK_RATE_MS ) );
		c = '\0';	// Lo borro para que luego del un CR no resetee siempre el timer.
		// el read se bloquea 50ms. lo que genera la espera.
		while ( frtos_read( fdMODEM, (char *)&c, 1 ) == 1 ) {
			//pv_MODEM_process(c);
			xputCharNS(c);
		}

	}
}
//-----------------------------------------------------------------------------
void pv_MODEM_init( void )
{
	// Preparo el buffer de entrada para nuevos comandos.
	modem_ptr = 0;
    memset( modem_buffer, 0x00, MAX_MODEM_INPUT_LENGTH );

}
//-----------------------------------------------------------------------------
void pv_MODEM_process( char cRxedChar )
{
	// Si es un CR, ejecuto lo que tengo en el buffer.
	// Sino lo almaceno si hay lugar.

	if ( cRxedChar == ASCII_CR ) {
		pv_MODEM_execute();

	} else {

		if( modem_ptr < MAX_MODEM_INPUT_LENGTH ) {
			modem_buffer[ modem_ptr ] = cRxedChar;
			modem_ptr++;
		}

	}
}
//------------------------------------------------------------------------------
void pv_MODEM_execute(void)
{
	// Imprimo en TERM el buffer y lo vacio.

	xprintf("%s\r", modem_buffer );
	pv_MODEM_init();
}
//------------------------------------------------------------------------------

