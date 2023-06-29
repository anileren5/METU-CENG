#include "common.h"

/**********************************************************************
 * --------------------- COUNTER & ALARM DEFINITION -------------------
 **********************************************************************/
Counter Counter_list[] = 
  {
   /*******************************************************************
    * -------------------------- First counter ------------------------
    *******************************************************************/
   {
     {
       200,                                /* maxAllowedValue        */
        10,                                /* ticksPerBase           */
       100                                 /* minCycle               */
     },
     0,                                    /* CounterValue           */
     0                                     /* Nbr of Tick for 1 CPT  */
   },
   /*******************************************************************
    * -------------------------- Second counter ------------------------
    *******************************************************************/
   {
     {
       200,                                /* maxAllowedValue        */
        10,                                /* ticksPerBase           */
       100                                 /* minCycle               */
     },
     0,                                    /* CounterValue           */
     0                                     /* Nbr of Tick for 1 CPT  */
   }
  };

Counter Counter_kernel = 
  {
    {
      65535,                              /* maxAllowedValue        */
          1,                              /* ticksPerBase           */
          0                               /* minCycle               */
    },
    0,                                    /* CounterValue           */
    0                                     /* Nbr of Tick for 1 CPT  */
  };
Counter Counter_kernel_2 = 
  {
    {
      65535,                              /* maxAllowedValue        */
          1,                              /* ticksPerBase           */
          0                               /* minCycle               */
    },
    0,                                    /* CounterValue           */
    0                                     /* Nbr of Tick for 1 CPT  */
  };

AlarmObject Alarm_list[] = 
  {
   /*******************************************************************
    * -------------------------- First task ---------------------------
    *******************************************************************/
   {
     OFF,                                  /* State                   */
     0,                                    /* AlarmValue              */
     0,                                    /* Cycle                   */
     &Counter_kernel,                      /* ptrCounter              */
     TASK0_ID,                             /* TaskID2Activate         */
     ALARM_EVENT,                          /* EventToPost             */
     0                                     /* CallBack                */
   },
   /*******************************************************************
    * -------------------------- Second task ---------------------------
    *******************************************************************/
   /*{
     OFF,                                  
     0,                                    
     0,                                   
     &Counter_kernel_2,                     
     TASK3_ID,                             
     ALARM_EVENT_LCD,                         
     0                                     
   },*/
   /*******************************************************************
    * ---------------------- Alarm LCD task ---------------------------
    *******************************************************************/
   /*{
     OFF,                                  
     0,                                    
     0,                                   
     &Counter_kernel_2,                      
     LCD_ID,                              
     LCD_EVENT,                          
     0                                    
   }*/
 };

#define _ALARMNUMBER_          sizeof(Alarm_list)/sizeof(AlarmObject)
#define _COUNTERNUMBER_        sizeof(Counter_list)/sizeof(Counter)
unsigned char ALARMNUMBER    = _ALARMNUMBER_;
unsigned char COUNTERNUMBER  = _COUNTERNUMBER_;
unsigned long global_counter = 0;

/**********************************************************************
 * --------------------- COUNTER & ALARM DEFINITION -------------------
 **********************************************************************/
Resource Resource_list[] = 
  {
   {
      10,                                /* priority           */
       0,                                /* Task prio          */
       0,                                /* lock               */
   }
  };
  
#define _RESOURCENUMBER_       sizeof(Resource_list)/sizeof(Resource)
unsigned char RESOURCENUMBER = _RESOURCENUMBER_;

/**********************************************************************
 * ----------------------- TASK & STACK DEFINITION --------------------
 **********************************************************************/
#define DEFAULT_STACK_SIZE      256
DeclareTask(TASK0);
//DeclareTask(TASK1);
DeclareTask(CUSTOMER0_TASK);
DeclareTask(CUSTOMER1_TASK);
DeclareTask(CUSTOMER2_TASK);
// DeclareTask(TASK3);
DeclareTask(LCD);
DeclareTask(HASH_TASK);


// to avoid any C18 map error : regroup the stacks into blocks
// of 256 bytes (except the last one).
#pragma		udata      STACK_A   
volatile unsigned char stack0[DEFAULT_STACK_SIZE];
#pragma		udata      STACK_B   
volatile unsigned char customer2Stack[DEFAULT_STACK_SIZE];
#pragma		udata      STACK_C   
volatile unsigned char customer0Stack[DEFAULT_STACK_SIZE];
#pragma		udata      STACK_D   
volatile unsigned char customer1Stack[DEFAULT_STACK_SIZE];
#pragma		udata      STACK_E
volatile unsigned char stack_lcd[DEFAULT_STACK_SIZE];
#pragma		udata      STACK_F   
volatile unsigned char hashTaskStack[DEFAULT_STACK_SIZE];
#pragma		udata

/**********************************************************************
 * ---------------------- TASK DESCRIPTOR SECTION ---------------------
 **********************************************************************/
#pragma		romdata		DESC_ROM
const rom unsigned int descromarea;
/**********************************************************************
 * -----------------------------  task 0 ------------------------------
 **********************************************************************/
rom_desc_tsk rom_desc_task0 = {
	TASK0_PRIO,                       /* prioinit from 0 to 15       */
	stack0,                           /* stack address (16 bits)     */
	TASK0,                            /* start address (16 bits)     */
	READY,                            /* state at init phase         */
	TASK0_ID,                         /* id_tsk from 0 to 15         */
	sizeof(stack0)                    /* stack size    (16 bits)     */
};

/**********************************************************************
 * -----------------------------  task 1 ------------------------------
 **********************************************************************/
/*rom_desc_tsk rom_desc_task1 = {
	TASK1_PRIO,                       
	stack1,                          
	TASK1,                            
	READY,                           
	TASK1_ID,                      
	sizeof(stack1)                    
};*/
/**********************************************************************
 * -----------------------------  Customer0 Task ----------------------
 **********************************************************************/
rom_desc_tsk rom_desc_CUSTOMER0 = {
	CUSTOMER0_PRIO,                       /* prioinit from 0 to 15       */
	customer0Stack,                           /* stack address (16 bits)     */
	CUSTOMER0_TASK,                            /* start address (16 bits)     */
	READY,                            /* state at init phase         */
	CUSTOMER0_TASK_ID,                         /* id_tsk from 0 to 15         */
	sizeof(customer0Stack)                    /* stack size    (16 bits)     */
};
/**********************************************************************
 * -----------------------------  Customer1 Task ----------------------
 **********************************************************************/
rom_desc_tsk rom_desc_CUSTOMER1 = {
	CUSTOMER1_PRIO,                       /* prioinit from 0 to 15       */
	customer1Stack,                           /* stack address (16 bits)     */
	CUSTOMER1_TASK,                            /* start address (16 bits)     */
	READY,                            /* state at init phase         */
	CUSTOMER1_TASK_ID,                         /* id_tsk from 0 to 15         */
	sizeof(customer1Stack)                    /* stack size    (16 bits)     */
};
/**********************************************************************
 * -----------------------------  Customer2 Task ----------------------
 **********************************************************************/
rom_desc_tsk rom_desc_CUSTOMER2 = {
	CUSTOMER2_PRIO,                       /* prioinit from 0 to 15       */
	customer2Stack,                           /* stack address (16 bits)     */
	CUSTOMER2_TASK,                            /* start address (16 bits)     */
	READY,                            /* state at init phase         */
	CUSTOMER2_TASK_ID,                         /* id_tsk from 0 to 15         */
	sizeof(customer2Stack)                    /* stack size    (16 bits)     */
};
/**********************************************************************
 * -----------------------------  task 3 ------------------------------
 **********************************************************************/
/*rom_desc_tsk rom_desc_task3 = {
	TASK3_PRIO,                       
	stack3,                           
	TASK3,                            
	READY,                            
	TASK3_ID,                         
	sizeof(stack3)                    
};*/

/**********************************************************************
 * -------------------------------  LCD  ------------------------------
 **********************************************************************/
rom_desc_tsk rom_desc_LCD = {
    LCD_PRIO,                       /* prioinit from 0 to 15       */
    stack_lcd,                      /* stack address (16 bits)     */
    LCD,                            /* start address (16 bits)     */
    READY,                          /* state at init phase         */
    LCD_ID,                         /* id_tsk from 0 to 15         */
    sizeof(stack_lcd)               /* ctx address   (16 bits)     */
};

/**********************************************************************
 * -------------------------------  HASH  -----------------------------
 **********************************************************************/
rom_desc_tsk rom_desc_HASH_TASK = {
    HASH_TASK_PRIO,                   
    hashTaskStack,                 
    HASH_TASK,                         
    READY,                        
    HASH_TASK_ID,                     
    sizeof(hashTaskStack)             
};

/**********************************************************************
 * --------------------- END TASK DESCRIPTOR SECTION ------------------
 **********************************************************************/
rom_desc_tsk end = {
	0x00,                              /* prioinit from 0 to 15       */
	0x0000,                            /* stack address (16 bits)     */
	0x0000,                            /* start address (16 bits)     */
	0x00,                              /* state at init phase         */
	0x00,                              /* id_tsk from 0 to 15         */
	0x0000                             /* stack size    (16 bits)     */
};

volatile rom unsigned int * taskdesc_addr = (&(descromarea)+1);
	
/* End of File : taskdesc.c */
