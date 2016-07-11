/**
 * @file tasks.h
 * @brief Interface for tasks.
 * @author Vitaliy Pisnya
 * @date July, 2016
 */

#ifndef TASKS_H
#define TASKS_H

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

#include "date.h"
#include "error.h"
#include "io.h"
#include "types.h"

/**
 * Macro for a task status.
 */
#define DONE   true

/**
 * Macro for a task status.
 */
#define UNDONE false

/**
 * @brief Appends task to the tasklist.
 *
 * If @p subject is not NULL, appends the task to the tail of the tasklist
 * specified by @p entry. If @p subject is NULL, allocates space for
 * a new subject, asks for description and then appends the task. 
 *
 * @param[in,out] entry Pointer to tasklist.
 * @param[in] subject Depending on a situation can be a string for a task
 *            description with the size of SUBJSIZE, which is defined in
 *            types.h, or NULL.
 * @param[in] status Boolean value with the task status (done or undone).
 * @return True on success, or false otherwise.
 */
bool add_task(Tasks *entry, char *subject, bool status);

/**
 * @brief Changes description of the existing task.
 *
 * Searches for a task with the index specified by @p index and if it's in
 * the tasklist specified by @p entry, prompts the user for a new description,
 * which substitutes description in the previously found task.
 *
 * @param[in,out] entry Pointer to the tasklist.
 * @param[in] index Long int with the index of the task which is to be
 *            substituted.
 * @return True on success, or false otherwise.
 */
bool change_task(Tasks *entry, long index);

/**
 * @brief Changes task status to done.
 *
 * Searches for a task with the index specified by @p index and if it's in
 * the list specified by @p entry, calls check_as_done(), which changes it's
 * status to done.
 *
 * @param[in,out] entry Pointer to the tasklist.
 * @param[in] index Long int to the index of a task which status is to be
 *            changed.
 * @return True on success, or false otherwise.
 */
bool do_task(Tasks *entry, long index);

/**
 * @brief Changes task status to undone.
 *
 * Searches for a task with the index specified by @p index and if it's in
 * the list specified by @p entry, calls uncheck_done(), which changes it's
 * status to undone.
 *
 * @param[in,out] entry Pointer to the tasklist.
 * @param[in] index Long int with index of a task which status is to be
 *            changed.
 * @return True on success, or false otherwise.
 */
bool undo_task(Tasks *entry, long index);

/**
 * @brief Changes status to done.
 *
 * Takes task specified by @p task and changes its status to done.
 *
 * @param[in,out] task Pointer to the task, which status is to be changed.
 * @return Nothing.
 */
void check_as_done(Task *task);

/**
 * @brief Changes status to undone.
 *
 * Takes task specified by @p task and changes its status to undone.
 *
 * @param[in,out] task Pointer to the task, which status is to be changed.
 * @return Nothing.
 */
void uncheck_done(Task *task);

/**
 * @brief Deletes task from the tasklist.
 *
 * Searches for a task with index specified by @p index, and if it's in the
 * list, removes it from the tasklist specified by @p entry.
 *
 * @param[in,out] entry Pointer to tasklist.
 * @param[in] index Long int with index of the task is to be removed.
 * @return True on success, or false otherwise.
 */
bool delete_task(Tasks *entry, long index);

/**
 * @brief Applies routine to every element of the tasklist.
 *
 * Takes tasklist specified by @p entry and applies function specified by
 * @p func to every element.
 *
 * @param[in,out] entry Pointer to the tasklist.
 * @param[in] func Pointer to a user-specified function.
 * @return Nothing.
 */
void traverse_tasks(Tasks *entry, void (*func)(Task *task));

/**
 * @brief Constructs task.
 *
 * Takes index, date, task status, subject and creates a task.
 *
 * @param[in] index Long int with the index of the task.
 * @param[in] date String with the task date.
 * @param[in] status Boolean value with the task status.
 * @param[in] subject String with the task subject.
 * @return Pointer to created task.
 */
Task *set_task(long index, char *date, bool status, char *subject);

/**
 * @brief Destroys list element's data.
 *
 * User-specified destroy function for the list element's data. Required by
 * Doubly-Linked List interface.
 *
 * @param[in,out] data Void pointer to the list element's data.
 * @return Nothing.
 */
void destroy_task(void *data);

#endif
