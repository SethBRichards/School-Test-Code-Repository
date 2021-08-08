/*sevenseg.c/h
@file   sevenseg.c/h
 *   @author    Seth Richards
 *   @date      8-May-2021
 *   @brief   This c file contains functions to control the seven segment display
 *   
 *  This c file contains definitions for each pin connected to the seven segment display,
 *  and the pins required to form segments into recognizable digits
 */

#include <Arduino.h>
#include "sevenseg.h"
#include "basicfunctions.h"

int *digits[] = {zero,one,two,three,four,five,six,seven,eight,nine,smile3,smile21,smile0};                                                                                                            ///< Array of pointers for each digit in order of digit
int digitSize[] = {sizeof(zero)/2,sizeof(one)/2,sizeof(two)/2,sizeof(three)/2,sizeof(four)/2,sizeof(five)/2,sizeof(six)/2,sizeof(seven)/2,sizeof(eight)/2,sizeof(nine)/2,sizeof(smile3)/2,sizeof(smile21)/2,sizeof(smile0)/2};      ///< The number of segments in each digit
int diodes[] = {D1,D2,D3,D4};   ///< Port Bit number of each digit placement diode in order                                                                                                                                                ///< Diode port number array with positioning opposite to the display output ie. [0] is the right side

int display_digit[4];  ///<Internal stored/updated value of digit pattern to place and display

/**
 * @brief Clears all outputs on segment display
 *
 * clr_display() sets all corresponding registers to display nothing on the segment display.
 *
 * @return Returns nothing
 */
void clr_display(){

      DISPLAY_DIODES = ~0;
      DISPLAY_7_SEG = 0;
}

/**
 * @brief Sets the given digit place on the display to the given digit number
 *
 * set_display() takes the given placement of the number on the display in @p place, and
 * lights up all of the segments as described in the associated pattern for @p number in that given place.
 * Example: set_display(2,6) will light up a six in the second placement(starting at 0 from the right).
 *
 * @param place The position of the number on the 4 digit display (0 being rightside)
 * @param number The digit to be lit up in segments on the display
 * @return Returns nothing
 * @warning The @p place is based on a predefined array position, and is not related to direct pin relationships. @see diodes for more
 * @warning The @p number is based on a predefined array position, not the actual digit. Digits past 9 may not be as expected @see digits for more
 */
void set_display(int place, int number){
      
      DISPLAY_7_SEG = 0;
      DISPLAY_DIODES = ~bit_set(diodes[place]);

      for(int i = 0;i < digitSize[number];i++){
          DISPLAY_7_SEG |= bit_set(digits[number][i]);
      }
}

/**
 * @brief Displays 4 digit number stored internally
 *
 * display_number() will quickly display each digit of its stored number broken down
 * @see display_digit @see breakdown @see pushSmile
 *
 * @return Returns nothing
 */
void display_number(){
  for(int i =0;i < DIODES;i++){        // Loops through each digit place
    set_display(i,display_digit[i]);   // Sets each digit to stored value in display_digit
    delayMicroseconds(DIGIT_DELAY);    // Pauses to allow for human eyes to see flash of each number
  }
  
}

/**
 * @brief Puts any 4 digits max number into each digit to store
 *
 * breakdown() takes the given @p number and uses math to put each digit in descending order
 * internal array for later use. Program will exit if number is too many digits
 * Example: breakdown(264) will put digits 2, 6, 4 into an internal array @see display_digit
 *
 * @param number Number to be broken into each digit
 * @return Returns nothing
 * @warning The breakdown function cannot handle a @p number larger than 4 digits and will exit if attempted
 */
void breakdown(int number){
  if(number > DIGIT_5) exit(0);       //exits program if number too large
  int mod_num = number;
  for(int i =0;i < DIODES;i++){       //loops through each digit place for display
    display_digit[i] = mod_num % 10;  // x mod 10 returns least significant digit
    mod_num = mod_num/10;             // discards least significant digit place for next iteration
  }
}  

/**
 * @brief Puts a smile into the display register
 *
 * pushSmile() will put the smile pattern into the internal register to display
 *
 * @return Returns nothing
 */
void pushSmile(){
  int i = 0;
  display_digit[i] = SMILE_INDEX_A;
  i++;
  display_digit[i] = SMILE_INDEX_B;
  i++;
  display_digit[i] = SMILE_INDEX_B;
  i++;
  display_digit[i] = SMILE_INDEX_C;
}

/**
 * @brief Sets up pins related to seven segment display
 *
 * segment_setup() will set each predefined pin for the seven segment display to 
 * output mode
 *
 * @return Returns nothing
 */
void segment_setup(){

    for(int i =1; i <= SEGMENTS ;i++){  // sets segment pins to output
      DISPLAY_7_SEG_INIT |= bit_set(i);
    }
    for(int i =1; i <= DIODES ;i++){    // sets digit placement diode pins to output
      DISPLAY_DIODES_INIT |= bit_set(i);
    }
  
}
