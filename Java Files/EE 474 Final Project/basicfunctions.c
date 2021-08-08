/*basicfunctions.c/h
@file   basicfunctions.c/h
 *   @author    Seth Richards
 *   @date      8-May-2021
 *   @brief   This c file contains functions to set and clear registers
 *   
 *  This c file contains basic functions to set and clear registers by inputting the desired bit
 *  to set
 */

#include "basicfunctions.h"

/**
 * @brief Gives the value to set a given bit
 *
 * bit_set() takes the given bit number and returns an int with the specified bit turned on
 * making it possible to set that bit in a register.
 * Example: <tt>PORTx |= bit_set(5); </tt> will set only the 5th bit to 1 (0010000)
 *
 * @param bit The position of the bit desired to clear
 * @return Returns int with only given bit on
 * @warning The code will exit if the given @p bit is outside of the 0th-8th bit range
 */
int bit_set(int bit){
    if(bit > 9 || bit < 0){  //exits if non-digit provided
      exit(0);
    }
    return 1<<bit;  //shifts one by bit to place
}

/**
 * @brief Gives the value to clear a given bit
 *
 * bit_clr() takes the given bit number and returns the compliment of that bits place,
 * making it possible to clear and port.
 * Example: <tt>PORTx &= bit_clr(5); </tt> will set only the 5th bit to 0 (1101111)
 *
 * @param bit The position of the bit desired to clear
 * @return Returns compliment of given bit position
 * @warning The code will exit if the given @p bit is outside of the 0th-8th bit range
 */
int bit_clr(int bit){
  if(bit > 9 || bit < 0){  //exits if non-digit provided
      exit(0);
    }
    return ~(1<<bit);  // shifts one by bit to place, and compliments
}







