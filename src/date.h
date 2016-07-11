/**
 * @file date.h
 * @brief prototypes of routines for working with dates.
 * @author Vitaliy Pisnya
 * @date June, 2016
 */

#ifndef DATE_H
#define DATE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "error.h"
#include "types.h"

/**
 * @brief Gets current system time.
 *
 * @p date must not be NULL. It's responsibility of the caller to manage
 * the storage for the date string. Size of the date string must be equal to
 * DATESIZE, which is defined in types.h. 
 *
 * @param[in,out] date String, where date is going to be stored.
 * @return True on success or false otherwise.
 */
bool get_curr_date(char *date);

/**
 * @brief Gets last entry date from file.
 *
 * Reads last entry date from last_entry.txt. Both parameters must
 * not be NULL. It's the responsibility of the caller to manage the
 * storage associated with the @p date and provide valid pointer to file.
 * Size of date string must be equal to DATESIZE, which is defined in types.h.
 * Uses function date_is_valid() to check if obtained date is valid.
 *
 * @param[in] fp Pointer to last_entry.txt file from where the date will be
 *            taken.
 * @param[in,out] date String, where entry date will be stored.
 * @return True on success or false otherwise.
 */
bool get_last_entry_date(FILE *fp, char *date);

/**
 * @brief Checks if date is valid.
 *
 * Takes string with the date and checks if it's valid for a further usage.
 * @p date must not be NULL. It's responsibility of the caller to manage the
 * storage associated with it. Size of the date string must be equal to
 * DATESIZE, which is defined in types.h.
 *
 * @param[in] date String containing the date to be checked.
 * @return True on success or false otherwise.
 */
bool date_is_valid(char *date);

/**
 * @brief Checks if date is current.
 *
 * Takes string with the date and checks if it equals to the current system
 * date. @p date must not be NULL. It's the responsibility of the caller to
 * manage the storage associated with the date string. Size of the date string
 * must be equal to DATESIZE, which is defined in types.h. Uses
 * get_current_date() to obtain current system time.
 *
 * @param[in] String containing the date.
 * @return True if date is outdated or false otherwise.
 */
bool is_outdated(char *date);

/**
 * @brief Shows current date.
 *
 * Gets current date by calling get_current_date() and printing it.
 *
 * @param None.
 * @return Nothing.
 */
void show_curr_date(void);

#endif
