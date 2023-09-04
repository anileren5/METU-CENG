#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/


/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/

/**********************************************************************
 * ------------------------------ Customer0 -------------------------------
 *
 * Task that sends '@' character every 5 seconds.
 *
 **********************************************************************/
extern void compute_hash(unsigned char *inp, unsigned char *out);
extern unsigned char unsolvedHash[10];
extern unsigned char solvedHash[19];
extern commandType;
extern char hashDone;
extern char cursor1;
extern char cookingForFoodJudge;
//THIS TASK SOLVES HASH, IT IS THE LOWEST PRIORTY TASK, SO THAT DOES NOT PREEMPT OTHER TASKS (MOST IMPORTANTLY COMMAND SENDER TASK TASK0)
//WHENEVER HASH RESPONSE IS RECEIVED, THIS TASK IS SENT WITH AN EVENT SO THAT STARTS TO SOLVE HASH
//WHEN FINISHED SOLVING, TRIGGERS TRANSMISSION OF HASH REPLY BY SETTING COMMANDTYPE TO 1 AND TXREG = '$';  
TASK(HASH_TASK) // HASH_TASK
{
    //TerminateTask();
    while(1)
    {
        //TRIGGERED WITHIN INTERRUPT ROUTINE WHEN HASH RESPONSE IS COMPLETELY
        //RECEIVED
        WaitEvent(HASH_EVENT);
        ClearEvent(HASH_EVENT);  
        compute_hash(unsolvedHash+1, solvedHash+1);
        solvedHash[17] = ':';
        //cursor1 = 0;
        hashDone = 1;
        cookingForFoodJudge = 0;
        //TXREG = '$';   
    }
	TerminateTask();
}


/* End of File : tsk_task0.c */


