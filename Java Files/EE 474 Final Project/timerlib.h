/*timerlib.h/h
@file   timerlib.h/h
 *   @author    Seth Richards
 *   @date      10-May-2021
 *   @brief   This header file contains definitions related 
 *   
 *  This header file contains definitions for functions to
 */

#ifndef timerlib.h
#define timerlib.h

//---------------------------------------------------------------------
	
#define  CLK_FREQ  16*1000000  ///< 16MHz Arduino Clock Frequency
#define PRESCALER  8           ///< 8 is our default prescaler factor for frequency division
 

#ifdef __cplusplus
extern "C" {
#endif

void timer_setup(int timer, int frequency,int outconfig);
void timer_set(int timer, int frequency);
void timer_ISR_set(int timer);

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------

#endif
