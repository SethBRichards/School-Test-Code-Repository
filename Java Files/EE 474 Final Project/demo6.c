/*demo6.c/h
@file   demo6.c/h
 *   @author    Seth Richards
 *   @date      16-May-2021
 *   @brief   This c file contains code for Lab 3 Demo 5
 *   
 *   This ino file uses a Data Driven Scheduler (DDS) to flash an LED forever, while playing a tune once
 *   with a 3 second countdown displayed until playing the tune again while displaying the current tone frequency once,
 *   before holding a 2 second smile once.
 */

#include "timerlib.h"           // timer and ISR functions
#include "basicfunctions.h"     // register setting and clearing functions 
#include "sevenseg.h"           // library for all seven segment display functions

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

#define SMILE_TIME            2000/2 ///< Smile on time for 2s, converted to 2ms

#define SEGMENT_UPDATE 100/2 ///< Speaker On time for 4s, converted to 2ms
#define MAX_DISPLAY    9999  ///< Highest number 7 segment display can show with base ten

#define D4 293      ///< Frequency of D4, 293 Hz
#define E4 329      ///< Frequency of E4, 329 Hz
#define C4 261      ///< Frequency of C4, 261 Hz
#define C3 130      ///< Frequency of C3, 130 Hz
#define G3 196      ///< Frequency of G3, 196 Hz

#define LED_FLASH_INDEX       0  ///< Index of led flashing task
#define SONG_PLAYER_INDEX     1  ///< Index of tune playing task
#define SONG_PLAYER_2_INDEX   2  ///< Index of tune playing task
#define COUNTDOWN_INDEX       3  ///< Index of countdown timer task
#define SMILE_INDEX           4  ///< Index of smile display task

#define NTASKS          10   ///< Number of total possible tasks in function pointer
#define DEFINED_TASKS   5    ///< Number of non null tasks in function pointer

int notes[] = {D4*2, E4*2, C4*2, C3*2, G3*2,0}; ///< Order of notes played in song

//function prototypes for tasks
void task1_DDS();
void task2_DDS();
void task2p2_DDS();
void task4_DDS();
void task5_DDS();

void (*tasks[NTASKS])() = {task1_DDS,task2_DDS,task2p2_DDS,task4_DDS,task5_DDS,NULL};  // tasks to be entered into tasklist struct
void (*deadTasks[NTASKS])() = {NULL};                            // list of dead tasks (unused, for future efforts)

int ids[] = {LED_FLASH_INDEX,SONG_PLAYER_INDEX,COUNTDOWN_INDEX,SMILE_INDEX};                                   // list of unique task ID's in sequential order
String taskNames[] = {"LED Flash", "Song Player", "Song and Display Player", "Countdown", "Smile"};            // list of unique task names in sequential order

int task_index = 0;       // Current index of task being processed

volatile static unsigned long int isr_flag;     // Flag for ISR to set
volatile static unsigned long int j;            // Randome variable for the ISR scheduler to process to stall time

typedef struct TCBstruct
{
    int id;                     // the unique id of the task
    String taskName;            // the task name
    int startedTimes;           // the number of times the task has been started
    void (*ftpr)(void);         // the function pointer
    unsigned short int  state;  // the task state
    unsigned int sleepTime;     // sleep delay
}TCBstruct;

TCBstruct TaskList[NTASKS];     // struct of tasks

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
  segment_setup();                 // sets up pins for the seven segment display
  timer_setup(5,250,0);            // sets up timer 5 ISR to 500Hz (2ms, ISR = 2f) to not output at pin
  timer_setup(4,0,1);              // sets up timer 4  to output at pin 6
  timer_ISR_set(5);                // enables ISR for timer 5
  Serial.begin(9600);

  // Unprimes all tasks that shouldn't play from the start
  TaskList[SONG_PLAYER_2_INDEX].state = DEAD;
  TaskList[COUNTDOWN_INDEX].state = DEAD;
  TaskList[SMILE_INDEX].state = DEAD;
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
         if(TaskList[task_index].state == DEAD) j++;                                         // Doesn't run dead tasks
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
 * @brief Plays the tone of "Close Encounters" with a 4 second pause twice
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
  
  timet2++;                                               //increment on 2ms loop
  
  if(timet2 >= SPKR_ON_PERIOD && toneState < 4){          //plays fourth note for 0.5sec longer than previous
    toneState++;
    timet2=0;
  }
  if(timet2 >= SPKR_ON_PERIOD_LONG && toneState == 4){    //silence for 4 sec
    timer_set(4,0);
    toneState = 0;
    timet2=0;
    if(TaskList[SONG_PLAYER_INDEX].startedTimes == 2){    // only plays twice before quitting itself and priming countdown task
        task_self_quit();
        TaskList[COUNTDOWN_INDEX].state = READY;
    }
    else{
        sleep_474_DDS(SPKR_OFF_PERIOD);                    //sets task to halt state until sleep is over
    }
  }
}

/**
 * @brief Plays the tone of "Close Encounters" with a 4 second pause, and displays its frequency
 *
 * task2_DDS() will keep track of how many times the ISR scheduler loop has run to
 * play the tune from "Close Encounters" with varying note lengths, and a 4 second pause.
 * It will also display the frequency currently playing on the seven segment display
 *
 * @return Returns nothing
 * @warning This function is not pre-emptive
 */
void task2p2_DDS(){

  static long int timet2 = 0; //tracks ISR loop time
  static int toneState = 0;  //tracks which note is playing for correct tone time length

  if(TaskList[task_index].state == READY){
    TaskList[COUNTDOWN_INDEX].state = DEAD;
  }
  
  if(timet2 == 0){                                     // plays first 4 tones with 0.5s interval
    timer_set(4,notes[toneState]);
    breakdown(notes[toneState]/2);                     // division scaling factor adjusts for frequency shift (doubled for correct tone in timer) and puts into display register
  }
  if(TaskList[SONG_PLAYER_INDEX].state != SLEEPING){   // displays the frequency whenever a tone is being played
    display_number();
  }
  
  timet2++;                                            //increment on 2ms loop
  
  if(timet2 >= SPKR_ON_PERIOD && toneState < 4){       //plays fourth note for 0.5sec longer than previous
    toneState++;
    timet2=0;
  }
  if(timet2 >= SPKR_ON_PERIOD_LONG && toneState == 4){ //quits itself permenantly, clears the display, and primes the smile task
    timer_set(4,0);
    task_self_quit();
    clr_display();
    TaskList[SMILE_INDEX].state = READY;
    
  }
}

/**
 * @brief Displays a countdown for 3 seconds in between task 2 and task 2 part 2
 *
 * task4_DDS() will countdown by 10ths of a second until task 2p2 begins,
 * by decrementing its register number every 2ms to display a new number on the seven segment display
 * every 100ms
 *
 * @return Returns nothing
 * @warning This function is not pre-emptive
 */
void task4_DDS(){

  static long int timet4 = 0;                                                 //tracks ISR loop time
  static long int displayNum = SPKR_OFF_PERIOD/50 - SPKR_OFF_PERIOD/200;      //tracks ms number to display for countdown
    
  if(timet4 == 0) breakdown(displayNum);                                      //creates new display number for each digit internally
  display_number();                                                           //displays the current display number

  timet4++;                                                                   // increment every 2ms
  if(timet4 >= SEGMENT_UPDATE){                                               // increments the displayed number every 100ms and resets process
    displayNum--;;
    timet4 = 0;
  }
  if(displayNum == 0){                                                        // sets itself to dead to permenantly halt, primes task 2p2
    task_self_quit();  
    TaskList[SONG_PLAYER_2_INDEX].state = READY;
  }
}

/**
 * @brief Displays a smile for 2 seconds
 *
 * task5_DDS() will display a smile on the seven segment display for 2 seconds
 * before permenantly ending itself and clearing the display
 *
 * @return Returns nothing
 * @warning This function is not pre-emptive
 */
void task5_DDS(){

  static long int timet5 = 0;                           // tracks ISR loop time

  if(timet5 == 0) pushSmile();                          // pushes the smile pattern to the internal display register
  display_number();                                     // displays the current display pattern

  timet5++;                                             // increment every 2ms
  if(timet5 >= SMILE_TIME){                             // increments the displayed number every 100ms and resets process
    timet5 = 0;                                         
    task_self_quit();                                   // task ends intself permenantly and clears the display
    clr_display();
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