/*demo3.c/h
@file   demo3.c/h
 *   @author    Seth Richards
 *   @date      16-May-2021
 *   @brief   This c file contains code for Lab 3 Demo 3
 *   
 *   This ino file uses a Data Driven Scheduler (DDS) to flash and LED
 *   and play a song with varying periods, making use of a periodic ISR
 *   to maintain proper timing
 */

#include "timerlib.h"           // timer and ISR functions
#include "basicfunctions.h"     // register setting and clearing functions 

#define LED_REG       PORTA ///< PORTA for the LED pin output value (A0 = pin 22)
#define LED_OUT_REG   DDRA ///< DDRA for LED pin output mode

#define TIMER_5_ISR  TIMER5_COMPA_vect  ///<Name for timer 5 ISR compare vector

#define LED_ON_PERIOD   250/2  ///< LED On time for increment of 2ms for total 250ms on
#define LED_OFF_PERIOD  750/2  ///< LED Off time for 750ms, converted to 100us
#define LED_BIT 0              ///< Bit number of LED in port

#define READY    0  ///< Ready state - tasks are called 
#define RUNNING  1  ///< Running state - tasks are called (allows for single ready call situationally)
#define SLEEPING 2  ///< Sleeping state - tasks are not called and sleep times are decremented
#define DEAD     3  ///< Sleeping state - tasks are not called and sleep times are decremented

#define SPKR_ON_PERIOD        500/2  ///< Speaker On time for 0.5s, converted to 2ms
#define SPKR_ON_PERIOD_LONG   1000/2 ///< Speaker On long time for 1s, converted to 2ms
#define SPKR_OFF_PERIOD       4000/2 ///< Speaker On time for 4s, converted to 2ms

#define D4 293      ///< Frequency of D4, 293 Hz
#define E4 329      ///< Frequency of E4, 329 Hz
#define C4 261      ///< Frequency of C4, 261 Hz
#define C3 130      ///< Frequency of C3, 130 Hz
#define G3 196      ///< Frequency of G3, 196 Hz

#define NTASKS          10   ///< Number of total possible tasks in function pointer
#define DEFINED_TASKS   2    ///< Number of non null tasks in function pointer

int notes[] = {D4*2, E4*2, C4*2, C3*2, G3*2,0}; ///< Order of notes played in song

//function prototypes for tasks
void task1_DDS();
void task2_DDS();

void (*tasks[NTASKS])() = {task1_DDS,task2_DDS,NULL};  // tasks to be entered into tasklist struct
void (*deadTasks[NTASKS])() = {NULL};                  // list of dead tasks (unused, for future efforts)

int ids[] = {0,1,2};                                   // list of unique task ID's in sequential order
String taskNames[] = {"LED Flash", "Song Player"};     // list of unique task names in sequential order

int task_index = 0;       // Current index of task being processed

volatile static unsigned long int isr_flag;     // Flag for ISR to set
volatile static unsigned long int j;            // Randome variable for the ISR scheduler to process to stall time

/**
 * @brief Creates a data driven structure to hold data for task management
 *
 * This structure holds data related to scheduling tasks to operate in a timely fashion
 * defined in later use cases @see scheduler
 * 
 * @param id The unique id of the task
 * @param taskName The name of the task in string form (meta)
 * @param startedTimes The number of times the task has been started
 * @param ftpr The function pointer
 * @param state The state of the task @see READY @see RUNNING @see SLEEPING @see DEAD
 * @param sleepTime The time the task will sleep adjusted to ISR loop time
 * @return Returns nothing
 */
typedef struct TCBstruct
{
    int id;                    
    String taskName;           
    int startedTimes;          
    void (*ftpr)(void);         
    unsigned short int  state;  
    unsigned int sleepTime;     
}TCBstruct;

TCBstruct TaskList[NTASKS];     ///< List of tasks to run @see scheduler


void setup() {

  for(int i = 0;i < DEFINED_TASKS;i++){  //sets up a struct of tasks 1 and 2 using predefined arrays
      TaskList[i].id = ids[i];
      TaskList[i].taskName = taskNames[i];
      TaskList[i].startedTimes = 0;
      TaskList[i].ftpr = tasks[i];
      TaskList[i].state = READY;
      TaskList[i].sleepTime = 0;
  }
  
  LED_OUT_REG = bit_set(LED_BIT);  // sets pin 22 to output mode
  timer_setup(5,250,0);            // sets up timer 5 ISR to 500Hz (2ms, ISR = 2f) to not output at pin
  timer_setup(4,0,1);              // sets up timer 4  to output at pin 6
  timer_ISR_set(5);                // enables ISR for timer 5
}

void loop() { 
  
    scheduler();       // runs DDS    
    sync_schedule(1);  //allows for tasks to operate under ISR delay conditions, stalls until end of 2ms period is reached
}

/**
 * @brief Sets ISR flag to 0 when timer 5 comp a cycles
 *
 * This ISR sets the global integer @p isr_flag to 0 when timer 5 completes its given period
 * 
 * @param TIMER_5_ISR Is the register/bit name for the compare vector for timer 5 A
 * @return Returns nothing
 */
ISR(TIMER_5_ISR){
  isr_flag = 0;
}


/**
 * @brief Operates a DDS to do the tasks given in task list with proper state behavior
 *
 * scheduler() will loop through each task in task list and perform operations based on the given state.
 * Dead tasks don't run, sleeping tasks will have their sleep time decremented on loop, ready tasks will run the task and
 * increment the number of times in started on its counter, and running tasks will run the task.
 * 
 * @return Returns nothing
 * @warning This function only works with TaskList with the proper struct and global variables set up and in place
 */
void scheduler() {

    while(TaskList[task_index].ftpr != NULL){                                                // Goes through each non null task in tasks[] once per loop
        
         if(TaskList[task_index].state == DEAD) return;                                      // Doesn't run dead tasks
         else if(TaskList[task_index].state == SLEEPING){                                    
            TaskList[task_index].sleepTime--;                                                // Decrements current tasks sleeptime when in sleep state
            if(TaskList[task_index].sleepTime <= 0) TaskList[task_index].state = READY;      // Wakes up current task if sleep timer reaches 0
         }
         else if(TaskList[task_index].state == READY){                                       // runs task ones and increments # of starts variable once per start
              TaskList[task_index].startedTimes++;
              TaskList[task_index].ftpr();
              TaskList[task_index].state = RUNNING;                                          

         }
         else if(TaskList[task_index].state == RUNNING){                                     // runs tasks to completion
              TaskList[task_index].ftpr();
         }
         task_index++;                                                                       // Iterates current task index to loop through each task in while
         
    }
    task_index=0;
}

/**
 * @brief Does nothing
 *
 * doNothing() is a non-NULL empty function that will do nothing and return immediately, meant to be a placeholder
 * never actually run in code to work with scheduler. @see task_self_quit
 * 
 * @return Returns nothing
 * @warning This function is boring
 */
void doNothing(){
  return;
}

/**
 * @brief Quits the task that called this function
 *
 * task_self_quit() will set the current tasks state to dead, add its function to the dead task list at
 * its current task index ,and put a non-NULL function pointer placeholder in its place @see doNothing
 * 
 * @return Returns nothing
 * @warning This function can only quit functions based on the current task_index, cannot affect other tasks
 */
void task_self_quit(){
    TaskList[task_index].state = DEAD;
    deadTasks[task_index] = TaskList[task_index].ftpr;
    TaskList[task_index].ftpr = doNothing;
}

/**
 * @brief Revives the task that called this function
 *
 * start_function() will set the specifed task at @p index to ready,
 * repopulate the function pointer in the tasklist, and set its state to ready
 * 
 * @param index is the integer of the desired task to revive in the dead tasks list
 * @return Returns nothing
 * @warning This index given should match the original task index and not exceed the size of the task list
 */
void start_function(int index){
    TaskList[task_index].ftpr = deadTasks[task_index];
    deadTasks[task_index] = NULL;
    TaskList[task_index].state = READY;
}

/**
 * @brief Sleeps function of current task index
 *
 * sleep_474() will set the current task index state to sleep and set its associated
 * sleepTime array value to @p sleepTimeVal
 * 
 * @param sleepTimeVal Sets the amount of loop times the task will be in sleep state until ready
 * @return Returns nothing
 * @warning This function is not pre-emptive
 */
void sleep_474_DDS(int sleepTimeVal){

  TaskList[task_index].sleepTime = sleepTimeVal;
  TaskList[task_index].state = SLEEPING;
}

/**
 * @brief Flahes an LED on for 250ms, and off for 750ms
 *
 * task1_DDS() will keep track of how many times the ISR scheduler loop has run to
 * flash an LED at 250ms on and 750ms off based on an assumed loop time of 2ms
 *
 * @return Returns nothing
 * @warning This function is not pre-emptive
 */
void task1_DDS(){

  static long int timet1 = 0;  //tracks ISR loop time
  
  if(timet1 == 0) LED_REG |= bit_set(LED_BIT); // sets LED on at period begin
  timet1++;                                    //increment every 2ms
  if(timet1 >= LED_ON_PERIOD){
    sleep_474_DDS(LED_OFF_PERIOD);             //sets task to halt state until sleep is over
    timet1 = 0;
    LED_REG &= bit_clr(LED_BIT);               //sets LED off at 250ms interval
  }
  
  
}

/**
 * @brief Plays the tone of "Close Encounters" with a 4 second pause
 *
 * task2_DDS() will keep track of how many times the ISR scheduler loop has run to
 * play the tune from "Close Encounters" with varying note lengths, and a 4 second pause
 *
 * @return Returns nothing
 * @warning This function is not pre-emptive
 */
void task2_DDS(){

  static long int timet2 = 0; //tracks ISR loop time
  static int toneState = 0;  //tracks which note is playing for correct tone time length
  
  if(timet2 == 0){  //plays first 4 tones with 0.5s interval
    timer_set(4,notes[toneState]);
  }
  
  timet2++;                                            //increment on 2ms loop
  
  if(timet2 >= SPKR_ON_PERIOD && toneState < 4){       //plays fourth note for 0.5sec longer than previous
    toneState++;
    timet2=0;
  }
  if(timet2 >= SPKR_ON_PERIOD_LONG && toneState == 4){ //silence for 4 sec
    timer_set(4,0);
    sleep_474_DDS(SPKR_OFF_PERIOD);                    //sets task to halt state until sleep is over
    toneState = 0;
    timet2=0;
  }
}

/**
 * @brief ISR Scheduler Synchroniser
 * @author Prof. Blake Hannaford
 * sync_schedule() will stall itself by running meaningless computations until
 * the ISR flag is called, which will end the stall and prime itself to restall again.
 * When used with an ISR to change the flag, this function will ensure a loop() consistent
 * loop period, assuming the other loop code executes faster than the ISR
 *
 * @return Returns nothing
 * @warning This function is not pre-emptive
 */
void sync_schedule(int d){
  while(d>0){          //sets inital stall condition
    while(isr_flag){   //stops stall when ISR is set
      j = j *2;
      j = j/2;
      j++;
      j--;
    }
    isr_flag = 1;      //primes stall functionality to occur on next loop
    d--;
  }
}