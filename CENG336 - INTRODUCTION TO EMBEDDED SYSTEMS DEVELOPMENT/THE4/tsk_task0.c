#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/


/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/

/**********************************************************************
 * ------------------------------ TASK0 -------------------------------
 *
 * Task that sends '@' character every 5 seconds.
 *
 **********************************************************************/
extern char mode;
extern char cursor1;
extern char hashDone;
extern char commandType;
char end1 = 0;
/**********************************************************************
 * ----------------SUMMARY OF THE IMPLEMENTATION-----------------------
 * 
 * TASK0, CUSTOMER0,1,2 , LCD AND HASH FUNCTIONS ARE RUNNING AT THE BEGINNING 
 * EACH OF THEM ARE BLOCKED INITIALLY (MAYBE AFTER SOME ONE TIME INITIALIZATIONS.
 *
 * TASK0: SENDS '$' CHARACTER EVERY 50 MS TO TRIGGER TRANSMISSION, SUCCESIVE 
 *  TRANSMISSIONS ARE DETERMINED BY COMMANDTYPE VARIABLE, SO THAT EITHER WAIT, COOK ETC. ARE SENT.
 * 
 * CUSTOMER TASKS(0-1-2)
 *  BEFORE THOSE TASKS ARE SIGNALED, PRIOTITIES OF EACH CUSTOMER IS CHOSEN BY THEIR PATIENCE
 *  THE EVENTS THEY WAIT ON ARE SET AFTER STATUS RESPONSE RECEIVED, THAN BY STATUS, EACH TASK DEALS WITH
 *  CUSTOMERS:  CT0 DEALS WITH HIGHEST PRIORITY CUSTOMER CT1 WITH MID PRIROTY ETC. 
 *  NUMBERING COULD BE CONFUESED BY INDEX OF CUSTOMERS, BUT IT INDICATES RESPECTIVE PRIORITY
 * 
 * LCD TASK
 *  INITIALIZE NECESSARY REGISTERS FOR LCD. THEN WAITS IN A REFRESHING LOOP THAT ONLY SIGNALED WHEN
 *  NEW STATUS IS RECEIVED, THEN REFRESHES DISPLAY, NEW LCD CONFIG IS DEALT WITH INSIDE ISR.
 * 
 * HASH TASK
 *  WHEN HASH RESPONSE IS RECEIVED, THIS TASK IS SIGNALED BY AN EVENT, IN A WHILE LOOP FOR EACH SIGNAL,
 *  CALLS THE HASHCOMPUTE FUNCTION ONCE, WHICH ASSIGNS SOLVED HASH TO A VARIABLE TO BE LATER USED WITH HASH REPLY.
 *  AFTER THE LINE TO CALL HASHFUNCTION, COMMANDTYPE IS SET TO 3 (HASH REPLY) SO THAT WHEN NEXT ALARM, TASK0 SENDS HASH REPLY.
 * 
 * ISR:
 *  DEALS WITH TRANSMISSION, RECEPTION, COUNTING TICKS WITH TMR0. THE OVERWHELMING PART IS AFTER A STATUS RESPONSE WHEN
 *  NECESSARY COMPUTATIONS EXPLAINED ABOVE ARE DONE, RELATED EVENTS ARE SIGNALED.
 * 
 *ALL THOSE TASKS RUN TOGETHER WITH SUITABLE PRIORITIES EXPLAINED IN COMMON.H
 **********************************************************************/
/**********************************************************************
 * ----------------EVENTS ACTIVATE EACH TASK---------------------------
 * 
 * 
ALARM_EVENT    : FOR TASK0, TO SEND COMMAND EACH 50MS
SERVE_EVENT    : THESE 3 ARE FOR CUSTOMER EVENTS, TO SERVE THEM AT EVERY STATUS RESPONSE
SERVE_EVENT2        
SERVE_EVENT3      
BLOCK_EVENT    : FOR TASK0, TO START IT INITIALLY WITH GO RESPONSE, SET ONLY ONCE.
HASH_EVENT     : FOR HASH TASK TO START COMPUTING THE HASH, SET AFTER HASH RESPONSE IS RECEIVED.
LCD_EVENT      : FOR LCD TASK, SO THAT IT REFRESHES LCD AFTER EACH STATUS RESPONSE
 * 
 * I DIDN'T EXPLICITLY MENTIONED. FOR CELARENCE: 
 *  WHEN EVENTS ARE SET, CORRESPONDING TASKS ARE CHANGED FROM SUSPENDED TO RUNNING IF THEY REACHED THE LINE
 *  WHERE THEY WAIT ON THE EVENTS. IN MY CASE, THEY ARE DEFINITELY SUSPENDED WHEN I SET THEIR EVENTS.
 * 
 * 
 **********************************************************************/
TASK(TASK0) // Command Sender
{
    //THIS IS THE TASK THAT SENDS COMMANDS REGULARLY
    //WAIT FOR $GO: GO TRIGGERS BLOCK_EVENT
    //TASK0 TRIGGERS SERIAL TRANSMISSION BY '$'
    WaitEvent(BLOCK_EVENT);
    ClearEvent(BLOCK_EVENT);
    //SET THE ALARM
    SetRelAlarm(ALARM_TSK0, 50, 50);
	while(1) {

        // WAITS FOR ALARM TO TRIGGER. 
        WaitEvent(ALARM_EVENT);
        ClearEvent(ALARM_EVENT);
        if (hashDone)
        {
            //HASHDONE IS SET BY HASH TASK
            //IF A HASH COMUTATION IS DONE, COMMANDTYPE IS CHANGED HERE TO HASH REPLY
            //COMMMANDTYPE CHANGE HERE, BECAUSE COMPUTATION MIGHT BE DONE BEFORE A TRANSMISSION ENDS.
            //THIS WAY IT DOES NOT DISRUPT ONGOING TRANSMISSION.
            commandType = 1;
            hashDone = 0;
        }
        if (end1)
        {
            //SET WHEN END RESPONSE IS RECEIVED, COMMAND SENDING IS STOPPED BY TERMINATION
            TerminateTask();
        }
        else
        {
            cursor1 = 0;
            //REMAINING CHARS ARE SENT IN THE INTERRUPT ROUTINE 
            TXREG = '$'; 
        }
	}
	TerminateTask();
}


/* End of File : tsk_task0.c */
