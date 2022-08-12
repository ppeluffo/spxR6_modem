/*
 * iopines.h
 *
 *  Created on: 11 ago. 2022
 *      Author: pablo
 */

#ifndef SRC_SPX_ULIBS_IOPINES_H_
#define SRC_SPX_ULIBS_IOPINES_H_

#include <avr/io.h>

#define PORT_SetPinAsOutput( _port, _bitPosition ) ( (_port)->DIR = (_port)->DIR | (1 << _bitPosition) )
#define PORT_SetPinAsInput( _port, _bitPosition ) ( (_port)->DIR = (_port)->DIR & ~(1 << _bitPosition) )
#define PORT_SetOutputBit( _port, _bitPosition ) ( (_port)->OUT = (_port)->OUT | (1 << _bitPosition) )
#define PORT_ClearOutputBit( _port, _bitPosition ) ( (_port)->OUT = (_port)->OUT & ~(1 << _bitPosition) )
#define PORT_GetBitValue( _port , _bitPosition ) ( (((_port)->IN) >> _bitPosition ) & 0x01 )
#define PORT_TogglePins( _port, _toggleMask ) ( (_port)->OUTTGL = _toggleMask )

#define TICK_BITPOS		2
#define TICK_PORT		PORTA
#define IO_config_TICK()	PORT_SetPinAsOutput( &TICK_PORT, TICK_BITPOS)
#define IO_set_TICK()		PORT_SetOutputBit( &TICK_PORT, TICK_BITPOS)
#define IO_clr_TICK()		PORT_ClearOutputBit( &TICK_PORT, TICK_BITPOS)

#define LED_KA_BITPOS	7
#define LED_KA_PORT		PORTF
#define IO_config_LED_KA()	PORT_SetPinAsOutput( &LED_KA_PORT, LED_KA_BITPOS)
#define IO_set_LED_KA()		PORT_SetOutputBit( &LED_KA_PORT, LED_KA_BITPOS)
#define IO_clr_LED_KA()		PORT_ClearOutputBit( &LED_KA_PORT, LED_KA_BITPOS)

// Salidas de micro, entradas del SIM
#define MODEM_PWR_BITPOS			4
#define MODEM_PWR_PORT			PORTD
#define IO_config_MODEM_PWR()	PORT_SetPinAsOutput( &MODEM_PWR_PORT, MODEM_PWR_BITPOS)
#define IO_set_MODEM_PWR()		PORT_SetOutputBit( &MODEM_PWR_PORT, MODEM_PWR_BITPOS)
#define IO_clr_MODEM_PWR()		PORT_ClearOutputBit( &MODEM_PWR_PORT, MODEM_PWR_BITPOS)

#define MODEM_SW_BITPOS			5
#define MODEM_SW_PORT			PORTD
#define IO_config_MODEM_SW()	PORT_SetPinAsOutput( &MODEM_SW_PORT, MODEM_SW_BITPOS)
#define IO_set_MODEM_SW()		PORT_SetOutputBit( &MODEM_SW_PORT, MODEM_SW_BITPOS)
#define IO_clr_MODEM_SW()		PORT_ClearOutputBit( &MODEM_SW_PORT, MODEM_SW_BITPOS)

void iopines_config(void);

#endif /* SRC_SPX_ULIBS_IOPINES_H_ */
