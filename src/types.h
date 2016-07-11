/**
 * @file types.h
 * @brief Contains constants, macros, and type definitions shared
 *        between other modules.
 * @author Vitaliy Pisnya
 * @date July, 2016
 */

#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

#include "dlist.h"

/** String length for a line, which read from or written to file. */
#define LINESIZE    128

/** String length for a date. */
#define DATESIZE    11

/** String length for a task status. */
#define STATSIZE    2

/** String length for a task subject. */
#define SUBJSIZE    76

/**
 * Offset which defines that the date sits between 0 and DATEOFFSET
 * characters in the read line.
 */
#define DATEOFFSET  10

/** 
 * Offset which defines that the status sits under STATOFFSET
 * index in the read line.
 */
#define STATOFFSET  11

/** Offset which defines, where the subject starts in the read line. */
#define SUBJOFFSET  13

/** Address and name of the file which contains the last entry. */
#define LAST_ENTRY  "./txt/last_entry.txt"

/** Address and name of the file which contains tasks history. */
#define HISTORY     "./txt/history.txt"

/**
 * @brief Custom macro for strings comparison.
 * @param a String 1 for comparison.
 * @param R Comparison operator (<, >, or ==).
 * @return True if expression evaluates to true, False otherwise.
 */
#define STRCMP(a, R, b) (strcmp(a, b) R 0)

/** Type definition for task. */
typedef struct Task_tag {
        long index; ///< Task index of type long.
        char *date; ///< Pointer to task date string.
        char *subject; ///< Pointer to subject string.
        bool status; ///< Boolean value for a task status.
} Task;

/** Type definition for element in task list. */
typedef DListElmt TasksElmt;

/** Type definition for a task list. */
typedef DList Tasks;

/** @see dlist_size */
#define tasks_size      dlist_size

/** @see dlist_head */
#define tasks_head      dlist_head

/** @see dlist_tail */
#define tasks_tail      dlist_tail

/** @see dlist_next */
#define next_elmt       dlist_next

/** @see dlist_prev */
#define prev_elmt       dlist_prev

/** @see dlist_data */
#define elmt_data       dlist_data

/** @see dlist_init */
#define init_tasks      dlist_init

/** @see dlist_destroy */
#define destroy_tasks   dlist_destroy

/** @see dlist_ins_nest */
#define ins_task_after  dlist_ins_next

/** @see dlist_ins_prev */
#define ins_task_before dlist_ins_prev

/** @see dlist_remove */
#define remove_elmt     dlist_remove

#endif
