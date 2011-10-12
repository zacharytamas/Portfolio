/**
 *  MenuLib.h
 *
 *  A library of C functions for simplifying typical 
 *  menu operations.
 *
 *  @version 1.0
 *  @author Zachary Jones <zacharytamas@gmail.com>
 *  @releasedate October 11, 2011
 */

#ifndef _Z_MENULIB_H_
#define _Z_MENULIB_H_

/**
 * Clears the input buffer of any trash that remains in it.
 */
void clearBuffer(void);

/**
 * Outputs a block of ANSI whitespace to clear the 
 * console screen of pre-existing text.
 */
void screenFlush(void);

/**
 * Prompts the user to press the Enter key to continue, 
 * blocking execution until they do.
 */
void pressEnterKey(void);

/**
 * Abstracts the collection of an integer from the user 
 * which represents a menu choice. Accepts an integer which 
 * is the total number of menu options, which is used to 
 * validate user input before returning it. Continously 
 * prompts the user until he enters a valid choice.
 */
int getMenuChoice(int numOfMenuOptions);

#endif
