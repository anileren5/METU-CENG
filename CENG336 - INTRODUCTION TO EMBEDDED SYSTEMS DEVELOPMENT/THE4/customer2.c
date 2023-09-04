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
char customer2_waiting = 0;
char customer2_cooking = 0;
char customer2_cooking_mask[4] = {0,0,0,0};
char customer2_ingr[2] = {'N','N'};
char customer2_ingr_index[2] = {'N','N'};
char ingredientsEnough2 = 0;
char customer2Id = 0;
extern char cookCommand[5];
extern char tossCommand[3];
extern char cookNotSent;
int m, n;
extern char high, mid, low;
extern void customerFunc(char customerIndex);
extern void customer0func(void);
extern void customer1func(void);
extern void customer2func(void);
//FUNCTIONS CUSTOMER0FUNC ETC. BELOW ARE DEFINED IN INT.C
//FOR EACH SERVE EVENT TRIGGER, ONE FUNCTION CALL IS MADE, WHICH DETECTS WHICH COMMAND WILL BE SENT NEXT
//BY ASSIGNING THOSE CHARACTERS TO KNOWN CHAR ARRAYS AND SETTING COMMAND TYPE TO APPROPRIATE VALUE
//LATER IN THE CASE OF TASK0 TRIGGERING THE TRANSMISSION, INTERRUPT ROUTINE SENDS THE CORRECT COMMAND


//THIS HAS THE LOW PRIORITY AMONG CUSTOMER TASKS
//BEFORE THOSE FUNCTIONS, WHEN STATUS RESPONSE IS TAKEN, SETPRIORITY FUNC IN INT.C DEALS WITH SPECIFYING HIGH, MID AND LOW VARIABLES
//SPECIFYING WHICH CUSTOMER FUNCTION (EACH DEALS WITH CUSTOMER IN THAT INDEX I.E. customer0func() DEALS WITH CUSTOMER AT INDEX 0) WILL BE CALLED WITHIN 
//LOW PRIORITY CUSTOMER1_TASK
TASK(CUSTOMER2_TASK) //SORRY FOR THE NAME INCONVENIENCE, THIS TASK IS NOT ABOUT CUSTOMER AT INDEX 2, IT IS THE LOW PRIORITY CUSTOMER TASK
{
    //TerminateTask();
    while(1)
    {
        //TRIGGERED WITHIN INTERRUPT ROUTINE WHENEVER STATUS RESPONSE COMPLETELY
        //RECEIEVED
        WaitEvent(SERVE_EVENT3);
        ClearEvent(SERVE_EVENT3);
        switch (low) //SPECIFIES WHICH CUSTOMER WAS CHOSEN AS LOW PRIORITY
        {
            case 0:{
                customer0func(); //MEANS CUSTOMER0 NEEDS LOW PRIORITY
                break;
            }
            case 1:{
                customer1func(); //MEANS CUSTOMER1 NEEDS LOW PRIORITY
                break;
            }
            case 2:{
                customer2func(); //MEANS CUSTOMER2 NEEDS LOW PRIORITY
                break;
            }
        }
    }
	TerminateTask();
}


/* End of File : tsk_task0.c */

