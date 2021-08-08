/*timerlib.c/h
@file   timerlib.c/h
 *   @author    Seth Richards
 *   @date      10-May-2021
 *   @brief   This c file contains functions to 
 *   
 *  This c file contains basic functions to 
 */

#include <Arduino.h>
#include "timerlib.h"
#include "basicfunctions.h"


/**
 * @brief Sets up specified timer to have frequency and output mode
 *
 * timer_setup() sets the given @p timer (3,4, or 5) to output a square wave at the given @p frequency in CTC mode
 * and will set the timer to output at its predefined pin given a 1 for @p outconfig, 0 to set off.
 * Example: <tt>timer_setup(3,100,1)</tt> will set timer 3 to output at 100Hz at its output pin, pin 6
 *
 * @param timer The timer to be set up
 * @param frequency The frequency in Hz the timer will output
 * @param outconfig Whether or not the timer will output to its pin
 * @return Returns nothing
 * @warning The @p timer must be 3,4,5 or the program will exit
 * @warning The @p frequency must be non-negative, no exit condition defined
 * @warning The @p outconfig must be 1 or 0, no exit condition defined
 */
void timer_setup(int timer, int frequency,int outconfig){
    
    switch(timer) {
      case 3 :  //pin 5
          DDRE |= bit_set(3);  // set PE3 to output (pin 6) (pinmode)
  	      TCCR3A = bit_set(6) * outconfig;  //TCCR4A set bit 6 to 1 for COMA0 normal output, bits 1 and 0 are wgm 41 and 40 (0 and 0 for CTC)
	        TCCR3B = bit_set(3) | bit_set(1);
	             //TCCR4B bits 4 and 3 are wgm 3 and 2 (0 and 1 for CTC mode)
 	             //TCCR4B bits 2 and 1 and 0 are scaling (0 and 1 and 0 for prescale 8 division)
         break;
      case 4 :  //pin 6
	       DDRH  |= bit_set(3);  // set PH3 to output (pin 6) (pinmode)
  	     TCCR4A = bit_set(6)* outconfig;  //TCCR4A set bit 6 to 1 for COMA0 normal output, bits 1 and 0 are wgm 41 and 40 (0 and 0 for CTC)
	       TCCR4B = bit_set(3) | bit_set(1);
	          //TCCR4B bits 4 and 3 are wgm 3 and 2 (0 and 1 for CTC mode)
 	          //TCCR4B bits 2 and 1 and 0 are scaling (0 and 1 and 0 for prescale 8 division)
         break;
      case 5 :  //pin 46
          DDRL |= bit_set(3);  // set PL3 to output (pin 6) (pinmode)
  	      TCCR5A = bit_set(6)* outconfig;  //TCCR4A set bit 6 to 1 for COMA0 normal output, bits 1 and 0 are wgm 41 and 40 (0 and 0 for CTC)
	        TCCR5B = bit_set(3) | bit_set(1);
	              //TCCR4B bits 4 and 3 are wgm 3 and 2 (0 and 1 for CTC mode)
 	              //TCCR4B bits 2 and 1 and 0 are scaling (0 and 1 and 0 for prescale 8 division)
	  
         break;
      default :
         exit(0);
   }
   timer_set(timer, frequency); 
}

/**
 * @brief Sets up specified timer to have frequency and output mode
 *
 * timer_set() sets the given @p timer (3,4, or 5) to output a square wave at the given @p frequency in CTC mode
 * Example: <tt>timer_set(3,100)</tt> will set timer 3 to output at 100Hz
 *
 * @param timer The timer to be set up
 * @param frequency The frequency in Hz the timer will output
 * @return Returns nothing
 * @warning The @p timer must be 3,4,5 or the program will exit
 * @warning The @p frequency must be non-negative, no exit condition defined
 */
void timer_set(int timer, int frequency){
    
    int ocr_value = 0;
    if(frequency != 0){
        ocr_value = ((CLK_FREQ)/(2*PRESCALER*frequency))-1; // CTC mode function to set OCR register for desired output frequency
    }
    
    
    switch(timer) {  //switch for correct timer set
      case 3 :
         OCR3A = ocr_value; 
         break;
      case 4 :
	       OCR4A = ocr_value;
         break;
      case 5 :
         OCR5A = ocr_value;
         break;
      default :
         exit(0);
   }
}

/**
 * @brief Sets up specified timer ISR to be enabled
 *
 * timer_ISR_set() sets the given @p timer (3,4, or 5) to output its ISR to the flag register
 * Example: <tt>timer_ISR_set(3)</tt> will set timer 3 ISR to occur
 *
 * @param timer The timer to be set up
 * @return Returns nothing
 * @warning The @p timer must be 3,4,5 or the program will exit
 */
void timer_ISR_set(int timer){
      
    switch(timer) {  //switch for correct timer set
      case 3 :
         TIMSK3 = bit_set(1); //bit 1 is output compare for Timer nA
         break;
      case 4 :
         TIMSK4 = bit_set(1);
         break;
      case 5 :
         TIMSK5 = bit_set(1);
         break;
      default :
         exit(0);
   }
   SREG |= bit_set(7);  //globally enables all interrupts

}
