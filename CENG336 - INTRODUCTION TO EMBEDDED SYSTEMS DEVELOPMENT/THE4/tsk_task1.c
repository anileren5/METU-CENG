#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
extern char rcv_value;
/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/


/**********************************************************************
 * ------------------------------ TASK1 -------------------------------
 *
 * Basic echo function test. Typing a single char should reply with the same char, 
 * then some chars that alphabetically precede this char. You will need buffers to echo strings.
 *
 **********************************************************************/
/*TASK(TASK1) 
{
	while(1) {
        WaitEvent(START_EVENT);
        ClearEvent(START_EVENT);
        TXREG = rcv_value;
	}
	TerminateTask();
}*/

/* End of File : tsk_task1.c */