/**
 * @file dlist.h
 * @brief Interface for Doubly-Linked List.
 *
 * This Interface was taken from "Mastering Algorithms with C" book
 * written by Kyle Loudon.
 *
 * @author Kyle Loudon
 * @date 2009
 */

#ifndef DLIST_H
#define DLIST_H

#include <stdlib.h>
#include <string.h>

/**
 * @brief Structure definition for doubly-linked list elements.
 */
typedef struct DListElmt {
        void             *data; ///< pointer to the data.
        struct DListElmt *prev; ///< pointer to the previous element in list.
        struct DListElmt *next; ///< pointer to the next element in list.
} DListElmt;

/**
 * @brief Structure definition for doubly-linked lists.
 */
typedef struct DList {
        int       size; ///< Number of elements in list.
        void      (*destroy)(void *); ///< User defined destroy function.
        DListElmt *head; ///< Pointer to the first element in list.
        DListElmt *tail; ///< Pointer to the last element in list.
} DList;

/**
 * @brief Gets number of elements in the list.
 *
 * Macro that evaluates to the number of elements in the doubly-linked list
 * specified by @p list.
 *
 * @param[in] list Pointer to a read-only doubly-linked list.
 * @return Integer with the number of elements in the list.
 */
#define dlist_size(list) ((list)->size)

/**
 * @brief Gets pointer to the first element in the list.
 *
 * Macro that evaluates to the element at the head of the doubly-linked list
 * specified by @p list.
 *
 * @param[in] list Pointer to a read-only doubly-linked list.
 * @return Pointer to the element at the head of the list.
 */
#define dlist_head(list) ((list)->head)

/**
 * @brief Gets pointer to the last element in the list.
 *
 * Macro that evaluates to the element at the tail of the doubly-linked list
 * specified by @p list.
 *
 * @param[in] list Pointer to a read-only doubly-linked list.
 * @return Pointer to the element at the tail of the list.
 */
#define dlist_tail(list) ((list)->tail)

/**
 * @brief Checks if element is at the head of the list.
 *
 * Macro that determines whether the element specified by @p element is at the
 * head of a doubly-linked list.
 *
 * @param[in] element Pointer to a read-only doubly-linked list element.
 * @return 1 if element is at the head of the list, or 0 otherwise.
 */
#define dlist_is_head(element) ((element)->prev == NULL ? 1 : 0)

/**
 * @brief Checks if element is at the tail of the list.
 *
 * Macro that determines whether the element specified by @p element is at the
 * tail of a doubly-linked list.
 *
 * @param[in] element Pointer to a read-only doubly-linked list element.
 * @return 1 if element is at the tail of the list, or 0 otherwise.
 */
#define dlist_is_tail(element) ((element)->next == NULL ? 1 : 0)

/**
 * @brief Gets data stored in the element.
 *
 * Macro that evaluates to the data stored in the element of a doubly-linked
 * list specified by @p element.
 *
 * @param[in] element Pointer to a read-only doubly-linked list element.
 * @return Void pointer to the data stored in the element.
 */
#define dlist_data(element) ((element)->data)

/**
 * @brief Gets next element.
 *
 * Macro that evaluated to the element of a doubly-linked list following the
 * element specified by @p element.
 *
 * @param[in] element Pointer to a read-only doubly-linked list element.
 * @return Pointer to the element following the specified element.
 */
#define dlist_next(element) ((element)->next)

/**
 * @brief Gets previous element.
 *
 * Macro that evaluated to the element of a doubly-linked list preceding the
 * element specified by @p element.
 *
 * @param[in] element Pointer to a read-only doubly-linked list element.
 * @return Element preceding the specified element.
 */
#define dlist_prev(element) ((element)->prev)

/**
 * @brief Doubly-linked list initialization function.
 *
 * Initializes the doubly-linked list specified by @p list. This operation
 * must be called for a doubly-linked list before the list can be used with
 * any other operation.
 *
 * @param[in,out] list Pointer to the list to be initialized.
 * @param[in] destroy Provides a way to free dynamically allocated data when
 *            dlist_destroy() is called. For a doubly-linked list containing
 *            data that should not be freed, @p destroy should be set to NULL.
 * @return Nothing.
 */
void dlist_init(DList *list, void (*destroy)(void *data));

/**
 * @brief Destroys doubly-linked list.
 *
 * Destroys doubly-linked list specified by @p list. No other
 * operations are permitted after calling dlist_destroy() unless
 * dlist_init() is called again. The dlist_destroy() removes all
 * elements from a doubly-linked list and calls the function passed as
 * destroy to dlist_init() once for each element as it is removed,
 * provided destroy was not set to NULL.
 *
 * @param[in,out] list Pointer to the list is to be destroyed.
 * @return Nothing.
 */
void dlist_destroy(DList *list);

/**
 * @brief Inserts an element after the element specified by the argument.
 *
 * Inserts an element just after the element specified by @p element in the
 * doubly-linked list specified by @p list. When inserting into
 * an empty list, @p element may point anywhere, but should be
 * NULL to avoid confusion. The new element contains a pointer to data,
 * so the memory referenced by @p data should remain valid as long as the
 * element remains in the list. It's the responsibility of the caller
 * to manage the storage associated with data.
 *
 * @param[in,out] list Pointer to the list where the element is to be inserted.
 * @param[in] element Pointer to the list element next to which the new
 *            element will be inserted.
 * @param[in] data Void pointer to the data for the new element.
 * @return 0 if insertion is successful, or -1 otherwise.
 */
int  dlist_ins_next(DList *list, DListElmt *element, const void *data);

/**
 * @brief Inserts an element before the element specified by the argument.
 *
 * Inserts an element just before the element specified by @p element in the
 * doubly-linked list specified by @p list. When inserting into an empty list,
 * @p element may point anywhere, but should be NULL to avoid confusion.
 * The new element contains a pointer to data, so the memory referenced by
 * @p data should remain valid as long as the element remains in the list.
 * It's responsibility of the caller to manage the storage associated with data.
 *
 * @param[in,out] list Pointer to the list where the element is to be inserted.
 * @param[in] element Pointer to the element in front of which the new element
 *            will be inserted.
 * @param[in] data Void pointer to the data for the new element.
 * @return 0 if insertion is successful, or -1 otherwise.
 */
int  dlist_ins_prev(DList *list, DListElmt *element, const void *data); int  dlist_remove(DList *list, DListElmt *element, void **data);

#endif
