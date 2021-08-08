/*basicfunctions.h/h
 *   @author    Diallo Wilson
 *   @author    Seth Richards
 *   @date      10-May-2021
 *   @brief   This header file contains definitions related to basic functions for setting registers
 *   
 *  This header file contains definitions for functions to set and clear registers with simple functions
 *  to improve code readability
 */

#ifndef basicfunctions.h
#define basicfunctions.h

//---------------------------------------------------------------------
	
#ifdef __cplusplus
extern "C" {
#endif

int bit_set(int bit);
int bit_clr(int bit);


#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------

#endif