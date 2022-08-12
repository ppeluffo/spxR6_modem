/*
 * spx.h
 *
 *  Created on: 8 dic. 2018
 *      Author: pablo
 */

#ifndef SRC_SPX_H_
#define SRC_SPX_H_

//------------------------------------------------------------------------------------
// INCLUDES
//------------------------------------------------------------------------------------
#include <avr/io.h>
#include <avr/wdt.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <compat/deprecated.h>
#include <avr/pgmspace.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <avr/sleep.h>
#include <avr_compiler.h>
#include <clksys_driver.h>
#include <pmic_driver.h>
#include <TC_driver.h>
#include <wdt_driver.h>
#include <ctype.h>
#include <frtos_cmd.h>
#include <inttypes.h>


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"
#include "croutine.h"
#include "semphr.h"
#include "timers.h"
#include "limits.h"
#include "portable.h"
#include "math.h"

#include "frtos-io.h"
#include "frtos_cmd.h"
#include "iopines.h"
#include "xprintf.h"

//------------------------------------------------------------------------------------
// DEFINES
//------------------------------------------------------------------------------------
#define SPX_FW_REV "1.0.0a"
#define SPX_FW_DATE "@ 20220811"

#define SPX_HW_MODELO "spxR6 HW:xmega256A3B R1.1"
#define SPX_FTROS_VERSION "FW:FRTOS10"

//#define F_CPU (32000000UL)
//#define SYSMAINCLK 2
//#define SYSMAINCLK 8
#define SYSMAINCLK 32
//
#define CHAR32	32
#define CHAR64	64
#define CHAR128	128
#define CHAR256	256

#define tkCtl_STACK_SIZE		384
#define tkCtl_TASK_PRIORITY	 		( tskIDLE_PRIORITY + 1 )
StaticTask_t xTask_Ctl_Buffer_Ptr;
StackType_t xTask_Ctl_Buffer [tkCtl_STACK_SIZE];
TaskHandle_t xHandle_tkCtl;
void tkCtl(void * pvParameters);

#define tkCmd_STACK_SIZE		384
#define tkCmd_TASK_PRIORITY	 		( tskIDLE_PRIORITY + 1 )
StaticTask_t xTask_Cmd_Buffer_Ptr;
StackType_t xTask_Cmd_Buffer [tkCtl_STACK_SIZE];
TaskHandle_t xHandle_tkCmd;
void tkCmd(void * pvParameters);

#define tkModem_STACK_SIZE		384
#define tkModem_TASK_PRIORITY	 		( tskIDLE_PRIORITY + 1 )
StaticTask_t xTask_Modem_Buffer_Ptr;
StackType_t xTask_Modem_Buffer [tkModem_STACK_SIZE];
TaskHandle_t xHandle_tkModem;
void tkModem(void * pvParameters);



//------------------------------------------------------------------------------------




//------------------------------------------------------------------------


#endif /* SRC_SPX_H_ */
