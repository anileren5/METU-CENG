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
char customer0_waiting = 0;
char customer0_cooking = 0;
char customer0_cooking_mask[4] = {0,0,0,0};
char customer0_ingr[2] = {'N','N'};
char customer0_ingr_index[2] = {'N','N'};
char ingredientsEnough = 0;
char customer0Id = 0;
extern char cookCommand[5];
extern char tossCommand[3];
extern char cookNotSent;
extern char k,l;
extern char customer1_waiting;
extern char customer1_cooking;
extern char customer1_cooking_mask[4];
extern char customer1_ingr[2];
extern char customer1_ingr_index[2];
extern char ingredientsEnough1;
extern char customer1Id;
extern char ingredientsEnough2;
extern char customer2Id;

extern char customer2_waiting;
extern char customer2_cooking;
extern char customer2_cooking_mask[4];
extern char customer2_ingr[2];
extern char customer2_ingr_index[2];
extern char m,n;
char high, mid, low;
int i, j;
extern void customer0func(void);
extern void customer1func(void);
extern void customer2func(void);
//FUNCTIONS CUSTOMER0FUNC ETC. BELOW ARE DEFINED IN INT.C
//FOR EACH SERVE EVENT TRIGGER, ONE FUNCTION CALL IS MADE, WHICH DETECTS WHICH COMMAND WILL BE SENT NEXT
//BY ASSIGNING THOSE CHARACTERS TO KNOWN CHAR ARRAYS AND SETTING COMMAND TYPE TO APPROPRIATE VALUE
//LATER IN THE CASE OF TASK0 TRIGGERING THE TRANSMISSION, INTERRUPT ROUTINE SENDS THE CORRECT COMMAND

//THIS HAS THE HIGHEST PRIRITY AMONG CUSTOMER TASKS
//BEFORE THOSE FUNCTIONS, WHEN STATUS RESPONSE IS TAKEN, SETPRIORITY FUNC IN INT.C DEALS WITH SPECIFYING HIGH, MID AND LOW VARIABLES
//SPECIFYING WHICH CUSTOMER FUNCTION (EACH DEALS WITH CUSTOMER IN THAT INDEX I.E. customer0func() DEALS WITH CUSTOMER AT INDEX 0) WILL BE CALLED WITHIN 
//HIGHEST PRIORITY CUSTOMER0_TASK
TASK(CUSTOMER0_TASK) //SORRY FOR THE NAME INCONVENIENCE, THIS TASK IS NOT ABOUT CUSTOMER AT INDEX 0, IT IS THE HIGHEST PRIORITY CUSTOMER TASK
{
    //TerminateTask();
    while(1)
    {
        //TRIGGERED WITHIN INTERRUPT ROUTINE WHENEVER STATUS RESPONSE COMPLETELY
        //RECEIEVED
        WaitEvent(SERVE_EVENT);
        ClearEvent(SERVE_EVENT);
        switch (high) //SPECIFIES WHICH CUSTOMER WAS CHOSEN AS HIGHEST PRIORITY
        {
            case 0:{
                customer0func(); //MEANS CUSTOMER0 NEEDS HIGHEST PRIORITY
                break;
            }
            case 1:{
                customer1func(); //MEANS CUSTOMER1 NEEDS HIGHEST PRIORITY
                break;
            }
            case 2:{
                customer2func(); //MEANS CUSTOMER2 NEEDS HIGHEST PRIORITY
                break;
            }
        }
        
    }
	TerminateTask();
}


/* End of File : tsk_task0.c */

