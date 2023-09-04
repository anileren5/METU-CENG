#include "common.h"
#include "LCD.h"

char rcv_value;
/**********************************************************************
 * Function you want to call when an IT occurs.
 **********************************************************************/
  extern void AddOneTick(void);
/*extern void MyOwnISR(void); */
  void InterruptVectorL(void);
  void InterruptVectorH(void);

/**********************************************************************
 * General interrupt vector. Do not modify.
 **********************************************************************/
#pragma code IT_vector_low=0x18
void Interrupt_low_vec(void)
{
   _asm goto InterruptVectorL  _endasm
}
#pragma code

#pragma code IT_vector_high=0x08
void Interrupt_high_vec(void)
{
   _asm goto InterruptVectorH  _endasm
}
#pragma code

/**********************************************************************
 * General ISR router. Complete the function core with the if or switch
 * case you need to jump to the function dedicated to the occuring IT.
 * .tmpdata and MATH_DATA are saved automaticaly with C18 v3.
 **********************************************************************/
unsigned short money_low = 0;
unsigned short money_high = 0;
unsigned short money = 0;
char customer_count = 0;
char active_string_pool[2][16] = {"MONEY:     00000", "C: 0    ING:NNNN"};  
char started = 0;
char first = 0;
char mode = 0;
char go[4] = {'$','G','O',':'};
char wait[2] = {'W',':'};
char endResponse[4] = {'E','N','D',':'};
char commandType = 0;
char responseType = 0;
char cursor = 0;
char cursor1 = 0;
char status[19];
char hash[9];
char hashDone = 0;
char cookingForFoodJudge = 0;
char cookCommand[5];
char tossCommand[3] = {'T',1,':'};
char tossMask[4] = {1,1,1,1};
char cookNotSent = 1;
char tossFlag = 0;
char ingrCount = 0;
char Notfound = 1;
char customer0cooking = 0, customer1cooking = 0, customer2cooking = 0;
char slowCookCommand[4];
extern char i,j,k,l,m,n;
extern char ingredientsEnough, ingredientsEnough1, ingredientsEnough2;
extern char customer0Id, customer1Id, customer2Id;
extern char customer0_ingr[2];
extern char customer0_ingr_index[2];
extern char customer1_ingr[2];
extern char customer1_ingr_index[2];
extern char customer2_ingr[2];
extern char customer2_ingr_index[2];
extern char high, mid, low;
extern char end1;
char x,c,v;
unsigned char unsolvedHash[10] = {0,0,0,0,0,0,0,0,0,0};
unsigned char solvedHash[19] = {'H',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,':',':'};
unsigned short convertCharsToUnsignedShort(char lowerByte, char higherByte) {
    unsigned short result = 0;
    
    result = (unsigned short)(lowerByte & 0xFF);      // Store the lower byte as it is
    result |= ((unsigned short)(higherByte & 0xFF) << 8);  // Shift and OR the higher byte
    
    return result;
}


void lcdFunction(void){
//BEFORE THIS FUNCTION IS CALLED MONEY VARIABLE IS SUPPLIED FROM STATUS
    //DETECT MONEY AND SET MONEY STRING ACCORDINGLY
    active_string_pool[0][11] = ((money/10000)%10) + 48;
    active_string_pool[0][12] = ((money/1000)%10) + 48;
    active_string_pool[0][13] = ((money/100)%10) + 48;
    active_string_pool[0][14] = ((money/10)%10) + 48;
    active_string_pool[0][15] = ((money/1)%10) + 48;
    customer_count = 0;
    //DETECT CUSTOMER COUNT
    if (status[1]) customer_count++;
    if (status[5]) customer_count++;
    if (status[9]) customer_count++;
    switch (customer_count){
        case 0: {
            active_string_pool[1][3] = '0';
            break;
        }
        case 1: {
            active_string_pool[1][3] = '1';
            break;
            }
        case 2: {
            active_string_pool[1][3] = '2';
            break;
        }
        case 3: {
            active_string_pool[1][3] = '3';
            break;
        }
    }
    active_string_pool[1][12] = status[13]; 
    active_string_pool[1][13] = status[14];
    active_string_pool[1][14] = status[15];
    active_string_pool[1][15] = status[16];
    LcdPrintString(active_string_pool[0],0,0);
    LcdPrintString(active_string_pool[1],0,1);
    //STATIC PARTS OF THE STRING IS ALREADY SET GLOBALLY
}
void toss(void)
{
    //THE TOSS FUNCTION WORKS FOR 3 or 4 INGR, ELSE DOES NOT TOSS
    ingrCount = 0;
    if(status[13] != 'N')
    {
        ingrCount++;
    }
    if(status[14] != 'N')
    {
        ingrCount++;
    }
    if(status[15] != 'N')
    {
        ingrCount++;
    }
    if(status[16] != 'N')
    {
        ingrCount++;
    }
    if(ingrCount < 3)
    {
        commandType = 0;
        return;
    }
    //IF 3 OR 4 INGREDIENT AVAILABLE, IT TOSSES THE FIRST INGREDIENT WHICH IS NOT IN ANY CUSTOMER'S ORDER
    for(v = 0; v< 4; v++)
    {
        //FOR EACH ITEM, CHECKS IF ITEM IS SPECIFIED IN ANY ORDER!
        Notfound = 1;
        if(status[13+v] != 'N' && status[13+v] != 'C' && status[13+v] != 'S')
        {
            
            for(x = 0; x <3; x++)
            {
                for(c = 0; c <2; c++)
                {
                    if(status[1+4*x+c] == status[13+v]) //COMPARE ORDER WITH INGREDIENTS
                    {
                        Notfound = 0;
                    }

                }
            }
            if (Notfound)
            {
                commandType = 3; //SO THAT TOSS COMMAND IS CHOSEN AS NEXT COMMAND THAT WILL BE SENT.
                tossCommand[1] = v;
                return;
            }
        }
    } 
    commandType = 0; // IF COULDN'T TOSS, AGAIN WAIT IS SPECIFIED BY DEFAULT
    return;
}
//Function that deals with customer at index 0
//Other customer fucntions are not explained, basically same behaviour.
void customer0func(void)
{
    if(cookNotSent)//It is a shared variable indicating if any customer
        //sent a cook command, no data hazard here
    {
        if(customer0Id != status[1])
        {
            //means customer arrived or left
            customer0cooking = 0;
        }
        if(customer0cooking)
        {
            //don't do anything if already cooking
            return;
        }
        customer0Id = status[1];
        if(customer0Id == 0)
        {
            //Customer not present
            ; 
        }
        else
        {
            //Detect if cook possible
            customer0_ingr[0] = status[2];
            customer0_ingr[1] = status[3];
            for(i = 0; i < 2; i++)
            {
                customer0_ingr_index[i] = 'N';
                if(customer0_ingr[i] != 'N')
                {
                    ingredientsEnough = 0;
                    for(j = 0; j < 4; j++)
                    {
                        if(customer0_ingr[i] == status[13+j])
                        {
                            customer0_ingr_index[i] = j;
                            ingredientsEnough = 1;
                            break;
                        }
                    }
                }
                if(!ingredientsEnough)
                {
                    break;
                }

            }
            if(ingredientsEnough)
            {
                //If possible arrrange cook command to be delievered with task0
                customer0cooking = 1;
                cookNotSent = 0;
                ingredientsEnough = 0;
                commandType = 2; // give cook command
                cookCommand[0] = 'C';
                cookCommand[1] = customer0Id;
                cookCommand[2] = customer0_ingr_index[0];      
                cookCommand[3] = customer0_ingr_index[1];
                cookCommand[4] = ':';
            }

        } 
    }
}
void customer1func(void)
{
    //SAME LOGIC EXPLAINED ABOVE
    if(cookNotSent)
    {
        if(customer1Id != status[5])
        {
            customer1cooking = 0;
        }
        if(customer1cooking)
        {
            return;
        }
        customer1Id = status[5];
        if(customer1Id == 0)
        {
            ; //Not arrived
        }
        else
        {
            customer1_ingr[0] = status[6];
            customer1_ingr[1] = status[7];
            for(k = 0; k < 2; k++)
            {
                customer1_ingr_index[k] = 'N';
                if(customer1_ingr[k] != 'N')
                {
                    ingredientsEnough1 = 0;
                    for(l = 0; l < 4; l++)
                    {
                        if(customer1_ingr[k] == status[13+l])
                        {
                            customer1_ingr_index[k] = l;
                            ingredientsEnough1 = 1;
                            break;
                        }
                    }
                }
                if(!ingredientsEnough1)
                {
                    break;
                }

            }
            if(ingredientsEnough1)
            {
                customer1cooking = 1;
                cookNotSent = 0;
                ingredientsEnough1 = 0;
                commandType = 2; // give cook command
                cookCommand[0] = 'C';
                cookCommand[1] = customer1Id;
                cookCommand[2] = customer1_ingr_index[0];
                cookCommand[3] = customer1_ingr_index[1];
                cookCommand[4] = ':';
            }

        } 
    }
}
void customer2func(void)
{
    //SAME LOGIC EXPLAINED ABOVE
    if(cookNotSent)
    {
        if(customer2Id != status[9])
        {
            customer2cooking = 0;
        }
        if(customer2cooking)
        {
            return;
        }
        customer2Id = status[9];
        if(customer2Id == 0)
        {
            ; //Not arrived
        }
        else
        {
            customer2_ingr[0] = status[10];
            customer2_ingr[1] = status[11];
            for(m = 0; m < 2; m++)
            {
                customer2_ingr_index[m] = 'N';
                if(customer2_ingr[m] != 'N')
                {
                    ingredientsEnough2 = 0;
                    for(n = 0; n < 4; n++)
                    {
                        if(customer2_ingr[m] == status[13+n])
                        {
                            customer2_ingr_index[m] = n;
                            ingredientsEnough2 = 1;
                            break;
                        }
                    }
                }
                if(!ingredientsEnough2)
                {
                    break;
                }

            }
            if(ingredientsEnough2)
            {
                customer2cooking = 1;
                cookNotSent = 0;
                ingredientsEnough2 = 0;
                commandType = 2; // give cook command
                cookCommand[0] = 'C';
                cookCommand[1] = customer2Id;
                cookCommand[2] = customer2_ingr_index[0];
                cookCommand[3] = customer2_ingr_index[1];
                cookCommand[4] = ':';
            }

        } 
    }
}
void setPriority(void)
{ 
    //by comparing patience, lower to higher
    //give high to low priorities
    // high var keeps the customerid of the highest priority (lowest patience)
    //customer etc.
    if (status[4] <= status[8])
    {
        if(status[8] <= status[12])
        {
            high = 0;
            mid = 1;
            low = 2;
        }
        else
        {
            if(status[12] <= status[4])
            {
                high = 2;
                mid = 0;
                low = 1;
            }
            else
            {
                high = 0;
                mid = 2;
                low = 1;
            }
        }
    }
    else
    {
        if(status[8] <= status[12])
        {
            if(status[12] <= status[4])
            {
                high = 1;
                mid = 2;
                low = 0;
            }
            else
            {
                high = 1;
                mid = 0;
                low = 2;
            }
        }
        else
        {
            high = 2;
            mid = 1;
            low = 0;
        }
    }
}
void dealWithCustomers(void)
{
    //Food judge are taken care out of customer tasks.
    //If no  food judge, signal customers.
    if(!commandType) // THIS CHECKS IF TOSS DIDNT FIND AN INGREDIENT TO TOSS, ELSE NO NEED TO DEAL WITH CUSTOMERS.
    {
        if(status[2] == 'F' && status[3] == 'J' && (!customer0cooking))
        {
            customer0Id = status[1];
            ingredientsEnough = 0;
            for(j=0;j <4 ; j++)
            {
                if(status[13+j] != 'N' && status[13+j] != 'C')
                {
                    ingredientsEnough = 1;
                    break;
                }
            }
            if(ingredientsEnough)
            {
                ingredientsEnough = 0;
                customer0cooking = 1;
                cookNotSent = 0;
                commandType = 4;
                cookingForFoodJudge = 1;
                slowCookCommand[0] = 'S';
                slowCookCommand[1] = customer0Id;
                slowCookCommand[2] = j;
                slowCookCommand[3] = ':';
            }
        }
        else if(status[6] == 'F' && status[7] == 'J' && (!customer1cooking))
        {
            customer1Id = status[5];
            ingredientsEnough1 = 0;
            for(j=0;j <4 ; j++)
            {
                if(status[13+j] != 'N' && status[13+j] != 'C')
                {
                    ingredientsEnough1 = 1;
                    break;
                }
            }
            if(ingredientsEnough1)
            {
                ingredientsEnough1 = 0;
                customer1cooking = 1;
                cookNotSent = 0;
                commandType = 4;
                cookingForFoodJudge = 1;
                slowCookCommand[0] = 'S';
                slowCookCommand[1] = customer1Id;
                slowCookCommand[2] = j;
                slowCookCommand[3] = ':';
            }
        }
        else if(status[10] == 'F' && status[11] == 'J' && (!customer2cooking))
        {
            customer2Id = status[9];
            ingredientsEnough2 = 0;
            for(j=0;j <4 ; j++)
            {
                if(status[13+j] != 'N' && status[13+j] != 'C')
                {
                    ingredientsEnough2 = 1;
                    break;
                }
            }
            if(ingredientsEnough2)
            {
                ingredientsEnough2 = 0;
                customer2cooking = 1;
                cookNotSent = 0;
                commandType = 4;
                cookingForFoodJudge = 1;
                slowCookCommand[0] = 'S';
                slowCookCommand[1] = customer2Id;
                slowCookCommand[2] = j;
                slowCookCommand[3] = ':';
            }
        }
        else 
        {
            //No food judge present, deal with customers.
            //SETEVENT FOR CUSTOMER TASKS, LET THEM SERVE CUSTOMERS IF POSSIBLE
            SetEvent(CUSTOMER0_TASK_ID, SERVE_EVENT);
            SetEvent(CUSTOMER1_TASK_ID, SERVE_EVENT2);
            SetEvent(CUSTOMER2_TASK_ID, SERVE_EVENT3);
        }   
    }
    
}
#pragma	code _INTERRUPT_VECTORL = 0x003000
#pragma interruptlow InterruptVectorL
void InterruptVectorL(void)
{
	EnterISR();	
	if (INTCONbits.TMR0IF == 1)
		AddOneTick();
	if (PIR1bits.TXIF == 1) {
        PIR1bits.TXIF = 0;
        switch (commandType) //DEFAULT 0: WAIT COMMAND
        {
            //BASICALLY BY COMMAND TYPE, TXREG IS WRITTEN REPEATEDLY UNTIL ITS LENGHT REACHED
            case 0:
            {
                //Send wait
                if (cursor1 == 2) 
                {
                    cursor1 = 0;
                }
                else
                {
                    TXREG = wait[cursor1];
                    cursor1++;
                }

                break;
            }
            case 1:
            {
                //Send solved hash
                if (cursor1 == 19)
                {
                    cursor1 = 0;
                    commandType = 0;
                }
                else
                {
                    TXREG = solvedHash[cursor1];
                    cursor1++;
                }
                break;
            }
            case 2:
            {
                //Send cook command
                if (cursor1 == 5)
                {
                    //cursor = 0;
                    cursor1 = 0;
                    commandType = 0;
                }
                else
                {
                    TXREG = cookCommand[cursor1];
                    cursor1++;
                }
                break;
            }
            case 3:
            {
                //Send toss command
                if (cursor1 == 3)
                {
                    //cursor = 0;
                    cursor1 = 0;
                    commandType = 0;
                }
                else
                {
                    TXREG = tossCommand[cursor1];
                    cursor1++;
                }
                break;
            }
            case 4:
            {
                //Send slow cook command
                if (cursor1 == 4)
                {
                    //cursor = 0;
                    cursor1 = 0;
                    commandType = 0;
                }
                else
                {
                    TXREG = slowCookCommand[cursor1];
                    cursor1++;
                }
                break;
            }
        }
        
        
	}
	if (PIR1bits.RCIF == 1) {
        rcv_value = RCREG;
        switch (mode){ //INITIALLY 0: NON-STARTED 1: ACTIVE 2: END 
            case 0:{
                if(go[cursor] == rcv_value)
                {
                    cursor++;
                }
                else
                {
                    cursor = 0;
                }
                if (cursor == 4)
                {       
                    //TASK0 CAN START SENDING COMMANDS NOW
                    //GO COMPLETELY RECEIVED
                    //SET TASK 0 TO START SENDING '$'
                    SetEvent(TASK0_ID, BLOCK_EVENT);
                    mode = 1; //ACTIVE MODE
                    cursor = 0;
                }
                break;
            }
            case 1:{
                if(first) //MEANS '$' RECEIVED ALREADY, CHECK NEXT ELEMENT TO SPECIFY RESPONSE TYPE!
                {
                    first = 0;
                    if (rcv_value == 'R')
                    {
                        cursor = 0;
                        responseType = 1;
                    }
                    else if(rcv_value == 'H')
                    {
                        cursor = 0;
                        responseType = 2;
                    }
                    else if(rcv_value == 'E')
                    {
                        cursor = 0;
                        responseType = 3;
                    }
                }
                if(rcv_value == '$') //$ RECEIVED, SET FIRST, SO THAT NEXT ELEMENT IS CHECKED TO DETERMINE RESPONSE TYPE
                { 
                    first = 1;
                    cursor = 0;
                }
                else
                {
                    switch(responseType)
                    {
                        case 1:{
                            if(cursor == 19)
                            {
                                if(rcv_value == ':')
                                {
                                    //AFTER STATUS RECEIVED, IMPORTANT ACTIONS ARE TAKEN!
                                    commandType = 0; //WAIT TYPE COMMAND
                                    responseType = 0; //DON'T EXPECT A RESPONSE TYPE, DETERMINED LATER AFTER $
                                    cursor = 0; //USUAL ACTION
                                    money = convertCharsToUnsignedShort(status[18], status[17]) ; //CALCULATE MONEY                                                                     
                                    lcdFunction(); //REFRESH LCD
                                    toss(); //DETERMINE TOSS IF POSSIBLE
                                    setPriority(); // DETERMINE WHICH CUSTOMERS DIFFERENT PRIORITY FUNCTI0NS WILL DEAL
                                    cookNotSent = 1; // MEANS NO COOK IS SPECIFIED YET
                                    dealWithCustomers();  // IN THIS FUNCTION CUSTOMER TASKS WILL BE SIGNALED
                                    //EACH OF WHICH CAN CLEAR cookNotSent, MAKE OTHER TASKS NOT DEAL WITH CUSTOMERS
                                    //BECAUSE A COOK COMMAND IS ALREADY DETERMINED TO BE SENT

                                }
                            }
                            else
                            {
                                //RECEIVE STATUS
                                status[cursor] = rcv_value;
                                cursor++; 
                            }
                            break;
                        }
                        case 2:{
                            if(cursor == 9)
                            {
                                if(rcv_value == ':')
                                {
                                    commandType = 0; //COMMAND TYPE IS ALWAYS 0 AS DEFAULT
                                    responseType = 0; // ALSO...
                                    cursor = 0; // DEFAULT ACTION WHREN A RESPONSE IS RECEIVED.
                                    //HASH RESPONSE RECEIVED, SIGNAL HASH SOLVER 
                                    //SO THAT IT STARTS SOLVING
                                    SetEvent(HASH_TASK_ID, HASH_EVENT); 
                                }
                            }
                            else
                            {
                                //GET THE UNSOLVED HASH
                                unsolvedHash[cursor] = rcv_value;
                                cursor++; 
                            }
                            break;
                        }
                        case 3:{
                            if(cursor == 3)
                            {
                                if(rcv_value == ':')
                                {
                                    //END IS TOTALLY RECEIVED, TAKE ALL PRECAUTIONS SO THAT COMMAND SENDING STOPS
                                    CancelAlarm(ALARM_TSK0);
                                    end1 = 1;
                                    mode = 2;
                                    commandType = 10;
                                    responseType = 0;
                                    cursor = 0;
                                }
                            }
                            else
                            {
                                cursor++; 
                            }
                            break;
                        }
                    }
                }
                
                break;
            }
        }
        PIR1bits.RCIF = 0;
	}
    if (RCSTAbits.OERR)
    {
      RCSTAbits.CREN = 0;
      RCSTAbits.CREN = 1;
    }
	LeaveISR();
}
#pragma	code

/* BE CARREFULL : ONLY BSR, WREG AND STATUS REGISTERS ARE SAVED  */
/* DO NOT CALL ANY FUNCTION AND USE PLEASE VERY SIMPLE CODE LIKE */
/* VARIABLE OR FLAG SETTINGS. CHECK THE ASM CODE PRODUCED BY C18 */
/* IN THE LST FILE.                                              */
#pragma	code _INTERRUPT_VECTORH = 0x003300
#pragma interrupt InterruptVectorH nosave=FSR0, TBLPTRL, TBLPTRH, TBLPTRU, TABLAT, PCLATH, PCLATU, PROD, section(".tmpdata"), section("MATH_DATA")
void InterruptVectorH(void)
{
  if (INTCONbits.INT0IF == 1)
    INTCONbits.INT0IF = 0;
}
#pragma	code



extern void _startup (void);
#pragma code _RESET_INTERRUPT_VECTOR = 0x003400
void _reset (void)
{
    _asm goto _startup _endasm
}
#pragma code


/* End of file : int.c */
