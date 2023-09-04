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
extern char status[18];
extern char commandType;
char customer1_waiting = 0;
char customer1_cooking = 0;
char customer1_cooking_mask[4] = {0,0,0,0};
char customer1_ingr[2] = {'N','N'};
char customer1_ingr_index[2] = {'N','N'};
char ingredientsEnough1 = 0;
char customer1Id = 0;
extern char cookCommand[5];
extern char tossCommand[3];
extern char cookNotSent;
int k, l;
extern void customerFunc(char customerIndex);
extern char high, mid, low;
extern void customer0func(void);
extern void customer1func(void);
extern void customer2func(void);
//FUNCTIONS CUSTOMER0FUNC ETC. BELOW ARE DEFINED IN INT.C
//FOR EACH SERVE EVENT TRIGGER, ONE FUNCTION CALL IS MADE, WHICH DETECTS WHICH COMMAND WILL BE SENT NEXT
//BY ASSIGNING THOSE CHARACTERS TO KNOWN CHAR ARRAYS AND SETTING COMMAND TYPE TO APPROPRIATE VALUE
//LATER IN THE CASE OF TASK0 TRIGGERING THE TRANSMISSION, INTERRUPT ROUTINE SENDS THE CORRECT COMMAND
//THIS HAS THE MID PRIORITY AMONG CUSTOMER TASKS
//BEFORE THOSE FUNCTIONS, WHEN STATUS RESPONSE IS TAKEN, SETPRIORITY FUNC IN INT.C DEALS WITH SPECIFYING HIGH, MID AND LOW VARIABLES
//SPECIFYING WHICH CUSTOMER FUNCTION (EACH DEALS WITH CUSTOMER IN THAT INDEX I.E. customer0func() DEALS WITH CUSTOMER AT INDEX 0) WILL BE CALLED WITHIN 
//MID PRIORITY CUSTOMER1_TASK
TASK(CUSTOMER1_TASK) //SORRY FOR THE NAME INCONVENIENCE, THIS TASK IS NOT ABOUT CUSTOMER AT INDEX 1, IT IS THE MID PRIORITY CUSTOMER TASK
{
    //TerminateTask();
    while(1)
    {
        //TRIGGERED WITHIN INTERRUPT ROUTINE WHENEVER STATUS RESPONSE COMPLETELY
        //RECEIEVED
        WaitEvent(SERVE_EVENT2);
        ClearEvent(SERVE_EVENT2);
        switch (mid) //SPECIFIES WHICH CUSTOMER WAS CHOSEN AS MID PRIORITY
        {
            case 0:{
                customer0func(); //MEANS CUSTOMER0 NEEDS MID PRIORITY
                break;
            }
            case 1:{
                customer1func(); //MEANS CUSTOMER1 NEEDS MID PRIORITY
                break;
            }
            case 2:{
                customer2func(); //MEANS CUSTOMER2 NEEDS MID PRIORITY
                break;
            }
        }
    }
	TerminateTask();
}


/* End of File : tsk_task0.c */

