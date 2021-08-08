/*demo1.c/h
@file   demo1.c/h
 *   @author    Seth Richards
 *   @date      11-May-2021
 *   @brief   This c file contains code for Lab 3 Demo 1
 *   
 *  This file calls Lab 3 tasks 1 & 2 using a round robin scheduler
 *  to flash and LED and play a tone on the speaker
 */

#include "timerlib.h"
#include "basicfunctions.h"

#define LED_REG       PORTA ///< PORTA for the LED pin output value (A0 = pin 22)
#define LED_OUT_REG   DDRA ///< DDRA for LED pin output mode

#define LED_ON_PERIOD   2500 ///< LED On time for 250ms, converted to 100us
#define LED_OFF_PERIOD  7500 ///< LED Off time for 750ms, converted to 100us
#define LED_BIT 0              ///< Bit number of LED in port

#define ROUND_ROBIN_TIMER 100  ///< Round robin sync time of 100us

#define SPKR_ON_PERIOD        5000  ///< Speaker On time for 0.5s, converted to 100us
#define SPKR_ON_PERIOD_LONG   10000 ///< Speaker On long time for 1s, converted to 100us
#define SPKR_OFF_PERIOD       40000 ///< Speaker On time for 4s, converted to 100us


#define D4 293  ///< Frequency of D4, 293 Hz
#define E4 329  ///< Frequency of E4, 329 Hz
#define C4 261  ///< Frequency of C4, 261 Hz
#define C3 130  ///< Frequency of C3, 130 Hz
#define G3 196  ///< Frequency of G3, 196 Hz

int notes[] = {D4*2, E4*2, C4*2, C3*2, G3*2,0}; ///< Order of notes played in song

void setup() {
  
  LED_OUT_REG = bit_set(LED_BIT);  // sets pin 22 to output mode
  timer_setup(4,E4*2,1);           // sets up timer 4 to output mode
}

void loop() {
    task1RR();    
    task2RR();
    delayMicroseconds(ROUND_ROBIN_TIMER);  //allows for tasks to operate under RR conditions
}

/**
 * @brief Flahes an LED on for 250ms, and off for 750ms
 *
 * task1RR() will keep track of how many times the RR scheduler loop has run to
 * flash an LED at 250ms on and 750ms off based on an assumed loop time of 100us
 *
 * @return Returns nothing
 * @warning This function is not pre-emptive
 */
void task1RR(){

  static long int timet1 = 0;  //tracks RR loop time
  
  if(timet1 == 0) LED_REG |= bit_set(LED_BIT); // sets LED on at period begin
  timet1++;  //increment on 100us loop
  if(timet1 >= LED_ON_PERIOD) LED_REG &= bit_clr(LED_BIT);  //sets LED off at 250ms interval
  if(timet1 >= LED_OFF_PERIOD) timet1 = 0; //resets to begin new period after 1sec
  
}

/**
 * @brief Plays the tone of "Close Encounters" with a 4 second pause
 *
 * task2RR() will keep track of how many times the RR scheduler loop has run to
 * play the tune from "Close Encounters" with varying note lengths, and a 4 second pause
 *
 * @return Returns nothing
 * @warning This function is not pre-emptive
 */
void task2RR(){

  static long int timet2 = 0; //tracks RR loop time
  static int toneState = 0;  //tracks which note is playing for correct tone time length
  
  if(timet2 == 0){  //plays first 4 tones with 0.5s interval
    timer_set(4,notes[toneState]);
  }
  
  timet2++;  //increment on 100us loop
  
  if(timet2 >= SPKR_ON_PERIOD && toneState < 4){ //plays fifth note for 0.5sec longer than previous
    toneState++;
    timet2=0;
  }
  if(timet2 >= SPKR_ON_PERIOD_LONG && toneState == 4){ //silence for 4 sec
    toneState++;
    timet2=0;
  }
  if(timet2 >= SPKR_OFF_PERIOD&& toneState == 5){ //resets variables for next tone period to begin
    toneState = 0;
    timet2=0;
  }
  

}