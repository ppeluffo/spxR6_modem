/*
 * spx_tkCmd.c
 *
 *  Created on: 11 ago. 2022
 *      Author: pablo
 */

#include "spx.h"
#include "utils.h"

static void pv_snprintfP_OK(void );
static void pv_snprintfP_ERR(void);

static void cmdHelpFunction(void);
static void modem_prender(void);
static void modem_apagar(void);
static void send_function(void);

//-----------------------------------------------------------------------------

#define MAX_CMD_INPUT_LENGTH		 256
char cmd_buffer[MAX_CMD_INPUT_LENGTH];
uint16_t cmd_ptr;

void pv_xCMD_init( void );
void pv_xCMD_process( char cRxedChar );
void pv_xCMD_execute(void);

bool local_echo = true;

//-----------------------------------------------------------------------------
void tkCmd_0(void * pvParameters)
{

	// Esta es la primer tarea que arranca.

( void ) pvParameters;
uint8_t c = 0;

	vTaskDelay( ( TickType_t)( 100 / portTICK_RATE_MS ) );
	xprintf_P( PSTR("\r\nstarting tkCmd..\r\n"));

	pv_xCMD_init();

	for( ;; )
	{
		//vTaskDelay( ( TickType_t)( 1000 / portTICK_RATE_MS ) );
		c = '\0';	// Lo borro para que luego del un CR no resetee siempre el timer.
		// el read se bloquea 50ms. lo que genera la espera.
		while ( frtos_read( fdTERM, (char *)&c, 1 ) == 1 ) {
			pv_xCMD_process(c);
			//xputCharNS(c);
		}

	}
}
//-----------------------------------------------------------------------------
void pv_xCMD_init( void )
{
	// Preparo el buffer de entrada para nuevos comandos.
	cmd_ptr = 0;
    memset( cmd_buffer, 0x00, MAX_CMD_INPUT_LENGTH );

}
//-----------------------------------------------------------------------------
void pv_xCMD_process( char cRxedChar )
{
	// Si es un CR, ejecuto lo que tengo en el buffer.
	// Sino lo almaceno si hay lugar.

	if ( local_echo )
		xputCharNS(cRxedChar);

	if ( cRxedChar == ASCII_CR ) {
		pv_xCMD_execute();

	} else {

		if( cmd_ptr < MAX_CMD_INPUT_LENGTH ) {
			cmd_buffer[ cmd_ptr ] = cRxedChar;
			cmd_ptr++;
		}

	}
}
//------------------------------------------------------------------------------
void pv_xCMD_execute(void)
{
	// Imprimo en TERM el buffer y lo vacio.

	if( !strcmp("prender", cmd_buffer) ) {
		modem_prender();
		pv_snprintfP_OK();
		pv_xCMD_init();
		return;
	}

	if( !strcmp("apagar", cmd_buffer) ) {
		modem_apagar();
		pv_snprintfP_OK();
		pv_xCMD_init();
		return;
	}

	if( !strcmp("echo on", cmd_buffer) ) {
		local_echo = true;
		pv_snprintfP_OK();
		pv_xCMD_init();
		return;
	}

	if( !strcmp("echo off", cmd_buffer) ) {
		local_echo = false;
		pv_snprintfP_OK();
		pv_xCMD_init();
		return;
	}


	xfprintf(fdMODEM, "%s\r", cmd_buffer );
	pv_xCMD_init();
}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
void tkCmd(void * pvParameters)
{

	// Esta es la primer tarea que arranca.

( void ) pvParameters;
uint8_t c = 0;

	vTaskDelay( ( TickType_t)( 100 / portTICK_RATE_MS ) );
	xprintf_P( PSTR("\r\nstarting tkCmd..\r\n"));

	FRTOS_CMD_init();
	FRTOS_CMD_register( "help", cmdHelpFunction );
	FRTOS_CMD_register( "prender", modem_prender );
	FRTOS_CMD_register( "apagar", modem_apagar );
	FRTOS_CMD_register( "send", send_function );

	for( ;; )
	{
		//vTaskDelay( ( TickType_t)( 1000 / portTICK_RATE_MS ) );
		c = '\0';	// Lo borro para que luego del un CR no resetee siempre el timer.
		// el read se bloquea 50ms. lo que genera la espera.
		//while ( CMD_read( (char *)&c, 1 ) == 1 ) {
		while ( frtos_read( fdTERM, (char *)&c, 1 ) == 1 ) {
			FRTOS_CMD_process(c);
		}

	}
}
//------------------------------------------------------------------------------------
static void send_function(void)
{
	FRTOS_CMD_makeArgv();
	xfprintf_P( fdMODEM, PSTR("%s\r"),argv[1] );
}
//------------------------------------------------------------------------------------
static void cmdHelpFunction(void)
{

	FRTOS_CMD_makeArgv();

	xprintf_P( PSTR("\r\nSpymovil Modem Loop\r\n"));
	xprintf_P( PSTR("prender\r\n"));
	xprintf_P( PSTR("apagar\r\n"));
	pv_snprintfP_OK();

}
//------------------------------------------------------------------------------------
static void modem_prender(void)
{
	modem_hw_pwr_on();
	modem_sw_pulse();

}
//------------------------------------------------------------------------------------
static void modem_apagar(void)
{
	modem_sw_pulse();
	modem_hw_pwr_off();

}
//------------------------------------------------------------------------------------
static void pv_snprintfP_OK(void )
{
	xprintf_P( PSTR("ok\r\n\0"));
}
//------------------------------------------------------------------------------------
static void pv_snprintfP_ERR(void)
{
	xprintf_P( PSTR("error\r\n\0"));
}
//------------------------------------------------------------------------------------

