/**
 * @file io.h
 * @brief Function declarations and macros for io.c.
 * @author Vitaliy Pisnya
 * @date July, 2016
 */

#ifndef IO_H
#define IO_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "date.h"
#include "error.h"
#include "tasks.h"
#include "types.h"

/**
 * Constant with available options for an empty list.
 */
#define OPTIONS1     "aehlqs"

/**
 * Constant with available options for a list which is not empty.
 */
#define OPTIONS2     "acdDehlqsuUxX"

/**
 * Macro for drawing separator.
 */
#define SEPARATOR()  printf("----------\n")

/**
 * @brief Uses ASCII sequences to clear console screen and place cursor at
 *        the top left corner.
 */
void clear_scr(void);

/**
 * @brief Clears standard I/O buffer.
 */
void clear_buf(void);

/**
 * @brief Shows all available options to manipulate tasks.
 */
void show_opts(void);

/**
 * @brief Returns set of available options.
 *
 * Checks if a list specified by @p entry is empty and returns a set of
 * available options appropriate to the result.
 *
 * @param[in] entry Pointer to a read-only task list.
 * @return String with a set of available options.
 */
char *get_valid_opts(const Tasks *entry);

/**
 * @brief Gets character from the standard input.
 * @return Integer with a character.
 */
int  get_char(void);

/**
 * @brief Prompts caller for an option letter.
 *
 * Asks caller to enter an option letter until it's valid.
 *
 * @param[in] entry Pointer to the task list.
 * @param[in] opts Read-only string with valid options.
 * @return Integer with the valid option letter.
 */
int  get_opt(Tasks *entry, const char *opts);

/**
 * @brief Checks if an option is valid.
 *
 * Checks if the option specified by @p ch is present in the string of
 * valid options specified by @p opts.
 *
 * @param[in] opts Read-only string with available options.
 * @param[in] ch Option which is to be checked.
 * @return True if option is valid, or false otherwise.
 */
bool opt_is_valid(const char *opts, char ch);

/**
 * @brief Gets number of type long.
 *
 * Takes string as an input and tries to convert it into long.
 * Length of the string is limited to 3 characters (including '\0')
 * because of the limit of tasks in one entry, which is 99.
 *
 * return long from 1 to 99 on success, or -1 otherwise.
 */
long get_long(void);

/**
 * @brief Gets task index.
 *
 * Calls get_long() until receives a valid task index. Validity
 * is checked by index_is_valid() routine.
 *
 * @param entry Pointer to task list.
 * @return Task index of type long, or -1L on NULL parameter.
 */
long get_index(Tasks *entry);

/**
 * @brief Checks if task index is valid.
 *
 * Takes index as an argument and checks if it's valid for a further use.
 *
 * @param index Task index of type long.
 * @param entry Pointer to read-only task list.
 * @return True if index is valid, or false otherwise.
 */
bool index_is_valid(long index, const Tasks *entry);

/**
 * @brief Gets string.
 *
 * Gets string of size specified by @p size. @p str must not be NULL.
 * It's responsibility of the caller to provide memory for string.
 *
 * @param str String, where input will be stored.
 * @param size Integer with the size limit of the taken string.
 * @param stream File pointer with stream.
 * @return True on success, or false otherwise.
 */
bool get_str(char *str, int size, FILE *stream);

/**
 * @brief Gets string with date.
 *
 * Asks for a date string until it's valid and saves it to a variable
 * specified by @p str. @p str must not be NULL. It's responsibility
 * of the caller to provide memory for it. Date string size is specified
 * by @p size.
 *
 * @param[in] entry Pointer to the task list.
 * @param[in,out] str String, where the date will be stored.
 * @param[in] size Size of the string, where the date will be stored.
 * @return True on success, or false otherwise.
 */
bool get_date(Tasks *entry, char *str, int size);

/**
 * @brief Parses a line from a file with tasks.
 *
 * Takes string read from file with tasks and places read values into
 * the variables they belong to. All parameters must not be NULL.
 * It's responsibility of the caller to privide memory for them.
 *
 * @param[in] line String, which is to be parsed.
 * @param[in,out] date String where the date is to be stored.
 * @param[in,out] status Bool variable, where task status is to be stored.
 * @param[in,out] subject String where task subject string is to be stored.
 * @return True on success, or false otherwise.
 */
bool parse_line(char *line, char *date, bool *status, char *subject);

/**
 * @brief Checks if task status is valid.
 *
 * Checks if status represented by character is valid.
 * Status done must be equal to + and status undone must
 * be equal to -.
 *
 * @param[in] status Status representer by char.
 * @return True if status is valid, or false otherwise.
 */
bool stat_is_valid(char status);

/**
 * @brief Checks if file is empty.
 * @param[in] fp File pointer.
 * @return True if file is empty, or false otherwise.
 */
bool file_is_empty(FILE *fp);

/**
 * @brief Reads entry with tasks from file.
 *
 * Reads file specified by @p fp task by task and adds them to
 * the task list specified by @p entry.
 *
 * @param[in] fp File Pointer to file is to be read.
 * @param[in,out] entry Pointer to task list where entry is to be read.
 * @return True on success, or false otherwise.
 */
bool read_entry_from_file(FILE *fp, Tasks *entry);

/**
 * @brief Writes entry to file.
 *
 * Traverses task list specified by @p entry and writes all the
 * tasks into file specified by @p fp.
 *
 * @param[in,out] fp File pointer to a file where entry is to be written.
 * @param[in] entry Pointer to the task list from where entry is to be taken.
 * @return True on success, or false otherwise.
 */
bool write_entry_to_file(FILE *fp, Tasks *entry);

/**
 * @brief Saves entry to history file.
 *
 * Opens file specified by @p fp with entry and appends it to
 * history.txt. If history.txt is not existing, it will be created.
 * 
 * @param[in] fp File pointer to the file with entry is to be copied.
 * @return True on success, or False otherwise.
 */
bool save_entry_to_history(FILE *fp);

/**
 * @brief Prints task history.
 *
 * Opens history.txt, specified by @p fp, reads its content,
 * and prints it in convenient way.
 *
 * @return True on success, or false otherwise.
 */
bool show_history(void);

/**
 * @brief Searches history by date.
 *
 * Asks caller for a date and, if it's valid and there is an
 * entry in the history with this date, prints it.
 *
 * @param[in] entry Pointer to the task list.
 * @return True on success, or false otherwise.
 */
bool search_history(Tasks *entry);

/**
 * @brief Erases history.
 *
 * Opens history file history.txt and erases its content.
 *
 * @param[in] entry Pointer to the task list.
 * @return True on success, or false otherwise.
 */
bool erase_history(Tasks *entry);

/**
 * @brief Prints entry tasks.
 *
 * Traverses task list and prints found tasks.
 *
 * @param[in] entry Pointer to the task list.
 * @return True on success, or false otherwise.
 */
bool show_tasks(Tasks *entry);

/**
 * @brief Prints task.
 *
 * Prints single task specified by @p task.
 *
 * @param[in] task Pointer to the task.
 */
void show_task(Task *task);

#endif
