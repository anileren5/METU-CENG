#ifndef COMMON_H
#define COMMON_H

#include "device.h"

/***********************************************************************
 * ------------------------ Timer settings -----------------------------
 **********************************************************************/
#define _10MHZ	63320
#define _16MHZ	61768
#define _20MHZ	60768
#define _32MHZ	57768
#define _40MHZ 	55768

//char mode;
/***********************************************************************
 * ----------------------------- Events --------------------------------
 **********************************************************************/
#define ALARM_EVENT       0x80
#define SERVE_EVENT        0x40
#define SERVE_EVENT2        0x04
#define SERVE_EVENT3        0x08
#define BLOCK_EVENT        0x10
#define HASH_EVENT        0x20
#define LCD_EVENT         0x01
#define ALARM_EVENT_LCD       0x02

/***********************************************************************
 * ----------------------------- Alarms --------------------------------
 **********************************************************************/
#define ALARM_TSK3          1       /* Alarm ID (index) in tascdesc.c */
#define LCD_ALARM_ID        2       /* Alarm ID (index) in tascdesc.c */
#define ALARM_TSK0           0

/***********************************************************************
 * ----------------------------- Task ID -------------------------------
 **********************************************************************/
/* Info about the tasks:
 * TASK0: USART
 * TASK1: USART
 */

#define TASK0_ID             1
#define TASK1_ID             2
#define CUSTOMER0_TASK_ID             3
#define CUSTOMER1_TASK_ID             4
#define CUSTOMER2_TASK_ID             6
#define LCD_ID              5
#define HASH_TASK_ID 7

// HERE ARE THE PRIORITIES
//  CUSTOMER TASKS ARE PROIRITIZED SUCH THAT CUSTOMER0_TASK HAS THE HIGHEST PRIORITY
//  AND CUSTOMER2_TASK HAS THE LOWEST PRIORITY. NUMBERS ARE NOT RELATED TO CUSTOMER INDEX!

// TO BETTER EXAMPLIFY: IF CUSTOMER WITH INDEX 1 HAS LOWER PATIENCE OR HIGHER NUMBER OF INGREDIENTS IN THE ORDER,
//  CUSTOMER0_TASK HAS AN INDEX PARAMETER WHICH IS SET TO 2 BEFORE ITS RUNNING, SO THAT IT DEALS WITH CUSTOMER 1.
//  AT THE END CUSTOMER 1 IS TAKEN CARE OF BEFORE OTHERS. 

// LCD_TASK HAS THE LOWEST PRIORITY, IT IS NOT AS TIME CRITIC AS OTHER TASKS.

// HASH TASK HAS LOWER PRIORITY THAN CUSTOMERS, CUSTOMERS ARE CRITICAL. THEREFORE,
//  HASH CALCULATION DOES NOT BLOCK COMMAND SENDING.

#define TASK0_PRIO           8
#define CUSTOMER0_PRIO          2
#define CUSTOMER1_PRIO          4
#define CUSTOMER2_PRIO          5
#define LCD_PRIO            11
#define HASH_TASK_PRIO       6





#endif

/* End of File : common.h */
