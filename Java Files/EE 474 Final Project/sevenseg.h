/*sevenseg.h/h
@file   sevenseg.h/h
 *   @author    Seth Richards
 *   @date      8-May-2021
 *   @brief   This header file contains definitions related to the seven segment display
 *   
 *  This header file contains definitions for each pin connected to the seven segment display,
 *  and the pins required to form segments into recognizable digits
 */

#include <Arduino.h>
#include "basicfunctions.h"

#ifndef sevenseg.h
#define sevenseg.h

//---------------------------------------------------------------------
#define DISPLAY_7_SEG       PORTC   ///<PORTC for the display segments
#define DISPLAY_7_SEG_INIT  DDRC
#define DISPLAY_DIODES      PORTL  ///<PORTL for the display segments
#define DISPLAY_DIODES_INIT DDRL  ///<PORTL for the display segments

#define SEGMENTS       7 ///< Number of segments in the display
#define DIODES         4 ///< Number of digits and diodes in the display

#define MIDDLE         1 ///< middle line on display bit
#define BOTTOM_RIGHT   2 ///< bottom right line on display bit
#define BOTTOM         3 ///< bottom line on display bit
#define BOTTOM_LEFT    4 ///< bottom left line on display bit
#define TOP_LEFT       5 ///< top left line on display bit
#define TOP            6 ///< top line on display bit
#define TOP_RIGHT      7 ///< top right line on display bit

#define D1 4  ///< The bit of the 1st number on the display
#define D2 3  ///< The bit of the 2nd number on the display
#define D3 2  ///< The bit of the 3rd number on the display
#define D4 1  ///< The bit of the 4th number on the display

#define DIGIT_5 10000    ///< This is the smallest integer the 7 segment can't display
#define DIGIT_DELAY 300  ///< Delay between flashing each digit on the display (Allows for human eyes to see update)

#define SMILE_INDEX_A  12   ///<
#define SMILE_INDEX_B  11
#define SMILE_INDEX_C  10


const int zero[] = {BOTTOM_RIGHT,BOTTOM,BOTTOM_LEFT,TOP_LEFT,TOP,TOP_RIGHT};             ///< Segments for digit 0
const int one[] = {BOTTOM_RIGHT,TOP_RIGHT};                                              ///< Segments for digit 1
const int two[] = {TOP,TOP_RIGHT,MIDDLE,BOTTOM_LEFT,BOTTOM};                             ///< Segments for digit 2
const int three[] = {BOTTOM,BOTTOM_RIGHT,MIDDLE,TOP_RIGHT,TOP};                          ///< Segments for digit 3
const int four[] = {TOP_LEFT,MIDDLE,TOP_RIGHT,BOTTOM_RIGHT};                             ///< Segments for digit 4
const int five[] = {TOP,TOP_LEFT,MIDDLE,BOTTOM_RIGHT,BOTTOM};                            ///< Segments for digit 5
const int six[] = {TOP,TOP_LEFT,MIDDLE,BOTTOM_RIGHT,BOTTOM,BOTTOM_LEFT};                 ///< Segments for digit 6
const int seven[] = {TOP,TOP_RIGHT,BOTTOM_RIGHT};                                        ///< Segments for digit 7
const int eight[] = {MIDDLE,BOTTOM_RIGHT,BOTTOM,BOTTOM_LEFT,TOP_LEFT,TOP,TOP_RIGHT};     ///< Segments for digit 8
const int nine[] = {MIDDLE,BOTTOM_RIGHT,BOTTOM,TOP_LEFT,TOP,TOP_RIGHT};                  ///< Segments for digit 9
const int smile3[] = {BOTTOM_RIGHT,TOP_LEFT,MIDDLE};                                     ///< Segments for smile pattern 3rd digit
const int smile21[] = {BOTTOM,TOP};                                                      ///< Segments for smile pattern 2nd and 1st digit
const int smile0[] = {MIDDLE,BOTTOM_LEFT,TOP_RIGHT};                                     ///< Segments for smile pattern 0th digit

  
#ifdef __cplusplus
extern "C" {
#endif

void set_display(int place, int number);
void clr_display();
void segment_setup();
void display_number();
void breakdown(int number);
void pushSmile();

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------

#endif
