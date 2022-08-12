/*
 * frtos-io.c
 *
 *  Created on: 11 jul. 2018
 *      Author: pablo
 */

#include "frtos-io.h"

//#define GPRS_IsTXDataRegisterEmpty() ((USARTE0.STATUS & USART_DREIF_bm) != 0)

#define USART_IsTXDataRegisterEmpty(_usart) (((_usart)->STATUS & USART_DREIF_bm) != 0)
#define USART_IsTXShiftRegisterEmpty(_usart) (((_usart)->STATUS & USART_TXCIF_bm) != 0)

#define USART_PutChar(_usart, _data) ((_usart)->DATA = _data)

#define GPRS_USART	USARTE0
//------------------------------------------------------------------------------------
int16_t frtos_open( file_descriptor_t fd, uint32_t flags)
{
	// Funcion general para abrir el puerto que invoca a una mas
	// especializada para c/periferico.
	// Es la que invoca la aplicacion.
	// Retorna -1 en error o un nro positivo ( fd )

int16_t xRet = -1;

	switch(fd) {
	case fdMODEM:
		xRet = frtos_uart_open( &xComGPRS, fd, iUART_GPRS, flags );
		break;

	case fdTERM:
		xRet = frtos_uart_open( &xComTERM, fd, iUART_TERM, flags );
		break;

	default:
		break;
	}

	return(xRet);
}
//------------------------------------------------------------------------------------
int16_t frtos_ioctl( file_descriptor_t fd, uint32_t ulRequest, void *pvValue )
{

int16_t xRet = -1;

	switch(fd) {
	case fdMODEM:
		xRet = frtos_uart_ioctl( &xComGPRS, ulRequest, pvValue );
		break;

	case fdTERM:
		xRet = frtos_uart_ioctl( &xComTERM, ulRequest, pvValue );
		break;

	default:
		break;
	}

	return(xRet);
}
//------------------------------------------------------------------------------------
int16_t frtos_write( file_descriptor_t fd ,const char *pvBuffer, const uint16_t xBytes )
{

int16_t xRet = -1;

	switch(fd) {
	case fdMODEM:
		//xRet = frtos_uart_write( &xComGPRS, pvBuffer, xBytes );
		xRet = frtos_uart_write( &xComGPRS, pvBuffer, xBytes );
		break;

	case fdTERM:
		xRet = frtos_uart_write( &xComTERM, pvBuffer, xBytes );
		break;

	default:
		break;
	}

	return(xRet);
}
//------------------------------------------------------------------------------------
int16_t frtos_read( file_descriptor_t fd , char *pvBuffer, uint16_t xBytes )
{

int16_t xRet = -1;

	switch(fd) {
	case fdMODEM:
		xRet = frtos_uart_read( &xComGPRS, pvBuffer, xBytes );
		break;

	case fdTERM:
		xRet = frtos_uart_read( &xComTERM, pvBuffer, xBytes );
		break;

	default:
		break;
	}

	return(xRet);
}
//------------------------------------------------------------------------------------
// FUNCIONES ESPECIFICAS DE UART's
//------------------------------------------------------------------------------------
int16_t frtos_uart_open( periferico_serial_port_t *xCom, file_descriptor_t fd, uart_id_t uart_id, uint32_t flags)
{

	xCom->fd = fd;
	xCom->xBlockTime = (10 / portTICK_RATE_MS );
	// Inicializo la uart del usb (iUART_USB) y la asocio al periferico
	xCom->uart = drv_uart_init( uart_id, flags );

	return(xCom->fd);

}
//------------------------------------------------------------------------------------
int16_t frtos_uart_write( periferico_serial_port_t *xCom, const char *pvBuffer, const uint16_t xBytes )
{
	// Esta funcion debe poner los caracteres apuntados en pvBuffer en la cola de trasmision del
	// puerto serial apuntado por xCom
	// Actua como si fuese rprintfStr.
	// Debe tomar el semaforo antes de trasmitir. Los semaforos los manejamos en la capa FreeRTOS
	// y no en la de los drivers.
	// El semaforo esta en xprintf.
	// Oficia como productor de datos de la cola txRingBuffer.

char cChar = '\0';
char *p = NULL;
uint16_t bytes2tx = 0;
int16_t wBytes = 0;

	// Controlo no hacer overflow en la cola de trasmision
	bytes2tx = xBytes;

	// Espero que el buffer de TX este vacio. ( La uart se va limpiando al trasmitir )
	while  ( rBufferGetCount( &xCom->uart->TXringBuffer ) > 0 ) {
		vTaskDelay( ( TickType_t)( 1 ) );
	}

	// Cargo el buffer en la cola de trasmision.
	p = (char *)pvBuffer;

	while(1) {
		// Voy  alimentando el txRingBuffer de a uno.
		cChar = *p;
		bytes2tx--;
		rBufferPoke( &xCom->uart->TXringBuffer, &cChar  );
		p++;
		wBytes++;	// Cuento los bytes que voy trasmitiendo

		// Si tengo un fin de pvBuffer, salgo
		if ( *p == '\0') {
			// Habilito a transmitir los datos que hayan en el buffer
			drv_uart_enable_tx_int( xCom->uart->uart_id );
			// Espero que se vacie
			while (  rBufferGetCount( &xCom->uart->TXringBuffer ) > 0)
				vTaskDelay( ( TickType_t)( 1 ) );
			// Termino
			break;
		}

		// Si la cola esta llena, empiezo a trasmitir y espero que se vacie.
		if (  rBufferReachHighWaterMark( &xCom->uart->TXringBuffer ) ) {
			// Habilito a trasmitir para que se vacie
			drv_uart_enable_tx_int( xCom->uart->uart_id );
			// Y espero que se haga mas lugar.
			while ( ! rBufferReachLowWaterMark( &xCom->uart->TXringBuffer ) )
				vTaskDelay( ( TickType_t)( 1 ) );
		}

		// Termine de cargar el pvBuffer
		if ( bytes2tx == 0 ) {
			drv_uart_enable_tx_int( xCom->uart->uart_id );
			while (  rBufferGetCount( &xCom->uart->TXringBuffer ) > 0)
				vTaskDelay( ( TickType_t)( 1 ) );
			break;
		}


	}
	/*
	while (*p && (bytes2tx-- > 0) ) {

		// Voy cargando la cola de a uno.
		cChar = *p;
		rBufferPoke( &xCom->uart->TXringBuffer, &cChar  );
		p++;
		wBytes++;	// Cuento los bytes que voy trasmitiendo

		// Si la cola esta llena, empiezo a trasmitir y espero que se vacie.
		if (  rBufferReachHighWaterMark( &xCom->uart->TXringBuffer ) ) {
			// Habilito a trasmitir para que se vacie
			drv_uart_enable_tx_int( xCom->uart->uart_id );

			// Y espero que se haga mas lugar.
			while ( ! rBufferReachLowWaterMark( &xCom->uart->TXringBuffer ) )
				vTaskDelay( ( TickType_t)( 5 ) );

		}
	}

	// Luego inicio la trasmision invocando la interrupcion.
	drv_uart_enable_tx_int( xCom->uart->uart_id );

	// Espero que trasmita todo
	while  ( rBufferGetCount( &xCom->uart->TXringBuffer ) > 0 )
		vTaskDelay( ( TickType_t)( 5 ) );

	*/

	// Y doy 1 ms para que se vacie el shift register.
	vTaskDelay( ( TickType_t)( 1 ) );

	return (wBytes);
}
//------------------------------------------------------------------------------------
int16_t frtos_uart_write_poll( periferico_serial_port_t *xCom, const char *pvBuffer, const uint16_t xBytes )
{
	// Transmite los datos del pvBuffer de a uno. No usa interrupcion de TX.
	//

char cChar = '\0';
char *p = NULL;
uint16_t bytes2tx = 0;
int16_t wBytes = 0;
int timeout;

	// Controlo no hacer overflow en la cola de trasmision
	bytes2tx = xBytes;

	// Trasmito.
	// Espero que los buffers esten vacios. ( La uart se va limpiando al trasmitir )
	//while  ( rBufferGetCount( &xCom->uart->TXringBuffer ) > 0 )
	//	vTaskDelay( ( TickType_t)( 1 ) );

	// Cargo el buffer en la cola de trasmision.
	p = (char *)pvBuffer;

	while ( bytes2tx-- > 0 ) {
		// Voy cargando la cola de a uno.
		cChar = *p;
		timeout = 10;	// Espero 10 ticks maximo
		while( --timeout > 0) {

			if ( USART_IsTXDataRegisterEmpty(xCom->uart->usart) ) {
				USART_PutChar(xCom->uart->usart, cChar);
				p++;
				wBytes++;	// Cuento los bytes que voy trasmitiendo
				break;
			} else {
				// Espero
				vTaskDelay( ( TickType_t)( 1 ) );
			}

			if ( timeout == 0 ) {
				// Error de transmision: SALGO
				return(-1);
			}

		}
	}

	return (wBytes);
}
//------------------------------------------------------------------------------------
void frtos_uart_putchar( file_descriptor_t fd ,const char cChar )
{
	// Escribe en el puerto (serial) en modo transparente. Sin semaforo !!

	switch(fd) {
	case fdMODEM:
		// Espero que este libre para transmitir

		// Transmito.
		USARTE0.DATA = cChar;
		while ( ( USARTE0.STATUS & USART_DREIF_bm) == 0)
			vTaskDelay( ( TickType_t)( 1 ) );
		break;

	case fdTERM:
		while ( ( USARTF0.STATUS & USART_DREIF_bm) == 0)
			vTaskDelay( ( TickType_t)( 1 ) );
		USARTF0.DATA = cChar;
		break;

	default:
		break;
	}

}
//------------------------------------------------------------------------------------
int16_t frtos_uart_ioctl( periferico_serial_port_t *xCom, uint32_t ulRequest, void *pvValue )
{

int16_t xReturn = 0;

	switch( ulRequest )
	{

		case ioctl_SET_TIMEOUT:
			xCom->xBlockTime = *((uint8_t *)pvValue);
			break;
		case ioctl_UART_CLEAR_RX_BUFFER:
			rBufferFlush(&xCom->uart->RXringBuffer);
			break;
		case ioctl_UART_CLEAR_TX_BUFFER:
			rBufferFlush(&xCom->uart->TXringBuffer);
			break;
		case ioctl_UART_ENABLE_TX_INT:
			drv_uart_enable_tx_int( xCom->uart->uart_id );
			break;
		case ioctl_UART_DISABLE_TX_INT:
			drv_uart_disable_tx_int( xCom->uart->uart_id );
			break;
		case ioctl_UART_ENABLE_RX_INT:
			drv_uart_enable_rx_int( xCom->uart->uart_id );
			break;
		case ioctl_UART_DISABLE_RX_INT:
			drv_uart_disable_rx_int( xCom->uart->uart_id );
			break;
		case ioctl_UART_ENABLE_TX:
			drv_uart_enable_tx( xCom->uart->uart_id );
			break;
		case ioctl_UART_DISABLE_TX:
			drv_uart_disable_tx( xCom->uart->uart_id );
			break;
		case ioctl_UART_ENABLE_RX:
			drv_uart_enable_rx( xCom->uart->uart_id );
			break;
		case ioctl_UART_DISABLE_RX:
			drv_uart_disable_rx( xCom->uart->uart_id );
			break;
		default :
			xReturn = -1;
			break;
	}

	return xReturn;

}
//------------------------------------------------------------------------------------
int16_t frtos_uart_read( periferico_serial_port_t *xCom, char *pvBuffer, uint16_t xBytes )
{
	// Lee caracteres de la cola de recepcion y los deja en el buffer.
	// El timeout lo fijo con ioctl.

int16_t xBytesReceived = 0U;
portTickType xTicksToWait = 0;
xTimeOutType xTimeOut;

	xTicksToWait = 10;
	vTaskSetTimeOutState( &xTimeOut );

	// Are there any more bytes to be received?
	while( xBytesReceived < xBytes )
	{

		if( rBufferPop( &xCom->uart->RXringBuffer, &((char *)pvBuffer)[ xBytesReceived ] ) == true ) {
			xBytesReceived++;
			//taskYIELD();
			vTaskDelay( ( TickType_t)( 1 ) );
		} else {
			// Espero xTicksToWait antes de volver a chequear
			vTaskDelay( ( TickType_t)( xTicksToWait ) );
		}

		// Time out has expired ?
		if( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) != pdFALSE )
		{
			break;
		}
	}

	return ( xBytesReceived );

}
//------------------------------------------------------------------------------------
