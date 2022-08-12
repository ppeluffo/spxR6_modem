/*
 * xprintf.h
 *
 *  Created on: 11 ago. 2022
 *      Author: pablo
 */

#ifndef SPX_ULIBS_XPRINTF_H_
#define SPX_ULIBS_XPRINTF_H_

#include "avr/pgmspace.h"
#include "stdbool.h"

void XPRINTF_init(void);
int xprintf( const char *fmt, ...);
int xprintf_P( PGM_P fmt, ...);
int xfprintf( int fd, const char *fmt, ...);
int xfprintf_P( int fd, PGM_P fmt, ...);

void xputChar(unsigned char c);
void xputCharNS(unsigned char c);

int xputs( const char *str );
int xprintf_PD( bool dflag,  PGM_P fmt, ...);


#endif /* SPX_ULIBS_XPRINTF_H_ */
