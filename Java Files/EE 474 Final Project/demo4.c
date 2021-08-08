/*demo4.c/h
@file   demo4.c/h
 *   @author    Seth Richards
 *   @date      15-May-2021
 *   @brief   This c file contains code for Lab 3 Demo 4
 *   
 *  This file calls Lab 3 tasks 1,2, & 3 using an ISR scheduler to loop through an array of function
 *  pointers of tasks to flash and LED, play a tune with specific timing, and counting every 100ms on a 
 *  7 segment display
 *  via ISR every 2ms to increment said tasks
 */

#include "basicfunctions.h"     // register setting and clearing functions 
#include "timerlib.h"           // timer and ISR functions
#include "sevenseg.h"           // library for all seven segment display functions

#define LED_REG       PORTA ///< PORTA for the LED pin output value (A0 = pin 22)
#define LED_OUT_REG   DDRA ///< DDRA for LED pin output mode

#define TIMER_5_ISR  TIMER5_COMPA_vect  ///<Name for timer 5 ISR compare vector

#define READY    0  ///< Ready state - tasks are called 
#define RUNNING  1  ///< Running state - tasks are called (allows for single ready call situationally)
#define SLEEPING 2  ///< Sleeping state - tasks are not called and sleep times are decremented

#define LED_ON_PERIOD   250/2  ///< LED On time for increment of 2ms for total 250ms on
#define LED_OFF_PERIOD  750/2  ///< LED Off time for 750ms, converted to 100us
#define LED_BIT 0              ///< Bit in port to define LED pin

#define SPKR_ON_PERIOD        500/2  ///< Speaker On time for 0.5s, converted to 2ms
#define SPKR_ON_PERIOD_LONG   1000/2 ///< Speaker On long time for 1s, converted to 2ms
#define SPKR_OFF_PERIOD       4000/2 ///< Speaker On time for 4s, converted to 2ms

#define SEGMENT_UPDATE 100/2 ///< Speaker On time for 4s, converted to 2ms
#define MAX_DISPLAY    9999  ///< Highest number 7 segment display can show with base ten

#define D4 293  ///< Frequency of D4, 293 Hz
#define E4 329  ///< Frequency of E4, 329 Hz
#define C4 261  ///< Frequency of C4, 261 Hz
#define C3 130  ///< Frequency of C3, 130 Hz
#define G3 196  ///< Frequency of G3, 196 Hz

#define NTASKS     10   ///< Number of total possible tasks in function pointer

int notes[] = {D4*2, E4*2, C4*2, C3*2, G3*2,0}; ///< Order of notes played in song

//function prototypes for tasks
void task1_ISR();
void task2_ISR();
void task3_ISR();

//(*tasks[task_index])();
void (*tasks[NTASKS])() = {task1_ISR,task2_ISR,task3_ISR,NULL};  //array of tasks to loop through

int task_index = 0;       // Current index of task being processed
int stateIndex[NTASKS];   // Array of each task's states
int sleepTime[NTASKS];    // Array of each task's sleep times

volatile static unsigned long int isr_flag;     // Flag for ISR to set
volatile static unsigned long int j;            // Randome variable for the ISR scheduler to process to stall time

void setup() {
  LED_OUT_REG = bit_set(LED_BIT);  // sets pin 22 to output mode
  segment_setup();
  timer_setup(5,250,0);            // sets up timer 5 ISR to 500Hz (2ms, ISR = 2f) to not output at pin
  timer_setup(4,0,1);              // sets up timer 4  to output at pin 6
  timer_ISR_set(5);                // enables ISR for timer 5
}

void loop() {  
    
    while(tasks[task_index] != NULL){                                                // Goes through each non null task in tasks[] once per loop

       if(stateIndex[task_index] == SLEEPING){                                         // Decrements current tasks sleeptime when in sleep state
          sleepTime[task_index]--;                                                     // Wakes up current task if sleep timer reaches 0
          if(sleepTime[task_index] <= 0) stateIndex[task_index] = READY;
       }
       else if(stateIndex[task_index] == READY){                                       // runs task when not in sleep state
            (*tasks[task_index])();
       }
       else if(stateIndex[task_index] == RUNNING){                                     //redundant copy state allows for future flexibility in code structure
            (*tasks[task_index])();
       }
       task_index++;                                                                  // Iterates current task index to loop through each task in while
    }
    task_index=0;                                                                    // Resets task index for next loop cycle
    sync_schedule(1);  //allows for tasks to operate under ISR delay conditions      // Stalls until end of 2ms period is reached
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
 * @brief Sleeps function of current task index
 *
 * sleep_474() will set the current task index state to sleep and set its associated
 * sleepTime array value to @p sleepTimeVal
 * 
 * @param sleepTimeVal Sets the amount of loop times the task will be in sleep state until ready
 * @return Returns nothing
 * @warning This function is not pre-emptive
 */
void sleep_474(int sleepTimeVal){
  
  sleepTime[task_index] = sleepTimeVal;
  stateIndex[task_index] = SLEEPING;
}

/**
 * @brief Flahes an LED on for 250ms, and off for 750ms
 *
 * task1_ISR() will keep track of how many times the ISR scheduler loop has run to
 * flash an LED at 250ms on and 750ms off based on an assumed loop time of 2ms
 *
 * @return Returns nothing
 * @warning This function is not pre-emptive
 */
void task1_ISR(){

  static long int timet1 = 0;  //tracks ISR loop time
  
  if(timet1 == 0) LED_REG |= bit_set(LED_BIT); // sets LED on at period begin
  timet1++;                                    //increment every 2ms
  if(timet1 >= LED_ON_PERIOD){
    sleep_474(LED_OFF_PERIOD);                 //sets task to halt state until sleep is over
    timet1 = 0;
    LED_REG &= bit_clr(LED_BIT);               //sets LED off at 250ms interval
  }
}

/**
 * @brief Plays the tone of "Close Encounters" with a 4 second pause
 *
 * task2_ISR() will keep track of how many times the ISR scheduler loop has run to
 * play the tune from "Close Encounters" with varying note lengths, and a 4 second pause
 *
 * @return Returns nothing
 * @warning This function is not pre-emptive
 */
void task2_ISR(){

  static long int timet2 = 0;                          //tracks ISR loop time
  static int toneState = 0;                            //tracks which note is playing for correct tone time length
  
  if(timet2 == 0){                                     //plays first 4 tones with 0.5s interval
    timer_set(4,notes[toneState]);
  }
  
  timet2++;                                            //increment on 2ms loop
  
  if(timet2 >= SPKR_ON_PERIOD && toneState < 4){       //plays fourth note for 0.5sec longer than previous
    toneState++;
    timet2=0;
  }
  if(timet2 >= SPKR_ON_PERIOD_LONG && toneState == 4){ //silence for 4 sec
    timer_set(4,0);
    sleep_474(SPKR_OFF_PERIOD);                        //sets task to halt state until sleep is over
    toneState = 0;
    timet2=0;
  }
}

/**
 * @brief Increments seven segment display every 100ms
 *
 * task3_ISR() will keep track of how many times the ISR scheduler loop has run to
 * increment its display number every 100ms, looping back to zero at 9999ms
 *
 * @return Returns nothing
 * @warning This function is not pre-emptive
 */
void task3_ISR(){

  static long int timet3 = 0;                   //tracks ISR loop time
  static long int displayNum = 0;               //tracks ms number to display

  if(displayNum > MAX_DISPLAY) displayNum = 0;  //resets counter after reaching max time
  
  if(timet3 == 0) breakdown(displayNum);        //creates new display number for each digit internally
  display_number();                             //displays the current display number

  timet3++;                                     // increment every 2ms
  if(timet3 >= SEGMENT_UPDATE){                 // increments the displayed number every 100ms and resets process
    displayNum++;
    timet3 = 0;
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