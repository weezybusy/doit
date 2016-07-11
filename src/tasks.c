/**
 * @file tasks.c
 * @brief Function definitions for working with tasks.
 * @author Vitaliy Pisnya
 * @date July, 2016
 */

#include "tasks.h"

/**
 * @brief Makes a string copy.
 * @param[in] src Source string.
 * @param[in] size size_t with the length of the source string.
 * @return Poiner to string copy.
 */
static char *copy_str(char *src, size_t size);

/**
 * @brief Searches for a task by index.
 * @param[in] entry Pointer to a read-only task list.
 * @param[in] long with the task index.
 * @return Pointer to found task on success, or NULL otherwise.
 */
static Task *find_task(const Tasks *entry, long index);

/**
 * @brief Searches for a list element by index.
 * @param[in] entry Pointer to a read-only task list.
 * @param[int] index long with the task index.
 * @return Pointer to found element on success, or NULL otherwise.
 */
static TasksElmt *find_list_elmt(const Tasks *entry, long index);

/**
 * @brief Extracts task from the list element.
 * @param[in] el Pointer to the list element.
 * @return Pointer to extracted task.
 */
static Task *extract_task(TasksElmt *el);

/**
 * @brief Reindexes tasks in a task list.
 * @param[in,out] entry Pointer to a task list.
 * @return Nothing.
 */
static void reindex_tasks(Tasks *entry);

bool add_task(Tasks *entry, char *subject, bool status)
{
        /** Subject parameter can be NULL */

        if (entry == NULL) {
                WARNING("Bad parameter -> entry == NULL.");
                return false;
        }

        char tmp[SUBJSIZE] = { 0 };
        char date[DATESIZE] = { 0 };

        show_tasks(entry);
        printf("task: ");

        if (!subject) {
                subject = tmp;
                if (!get_str(subject, SUBJSIZE, stdin))
                        return false;
        }

        /* Capitalize first letter of the subject. */
        subject[0] = toupper(subject[0]);

        get_curr_date(date);

        Task *task = set_task(tasks_size(entry) + 1, date, status, subject);

        if (task == NULL) {
                WARNING("Failed to make task.");
                return false;
        }

        return ins_task_after(entry, tasks_tail(entry), task) == 0;
}

bool change_task(Tasks *entry, long index)
{
        if (entry == NULL) {
                WARNING("Bad parameter -> entry == NULL.");
                return false;
        }

        char subject[SUBJSIZE] = { 0 };
        char *new_subject = NULL;

        Task *task = find_task(entry, index);

        if (task != NULL) {
                free(task->subject);
                task->subject = NULL;
                printf("new task: ");

                if (get_str(subject, SUBJSIZE, stdin)) {
                        new_subject = copy_str(subject, strlen(subject) + 1);

                        if (new_subject != NULL) {
                                task->subject = new_subject;
                                return true;
                        }
                }
        }

        return false;
}

bool do_task(Tasks *entry, long index)
{
        if (entry == NULL) {
                WARNING("Bad parameter -> entry == NULL.");
                return false;
        }

        TasksElmt *el = find_list_elmt(entry, index);

        if (el != NULL) {
                Task *task = extract_task(el);
                check_as_done(task);
                return true;
        }

        return false;
}

void check_as_done(Task *task)
{
        if (task == NULL) {
                WARNING("Bad parameter -> task == NULL.");
                return;
        }

        task->status = DONE;
}

bool undo_task(Tasks *entry, long index)
{
        if (entry == NULL) {
                WARNING("Bad parameter -> entry == NULL.");
                return false;
        }

        TasksElmt *el = find_list_elmt(entry, index);

        if (el != NULL) {
                Task *task = extract_task(el);
                uncheck_done(task);
                return true;
        }

        return false;
}

void uncheck_done(Task *task)
{
        if (task == NULL) {
                WARNING("Bad parameter -> task == NULL.");
                return;
        }

        task->status = UNDONE;
}

bool delete_task(Tasks *entry, long index)
{
        if (entry == NULL) {
                WARNING("Bad parameter -> entry == NULL.");
                return false;
        }

        TasksElmt *el = find_list_elmt(entry, index);

        if (el != NULL) {

                Task *task = extract_task(el);
                destroy_task(task);

                if (remove_elmt(entry, el, &(el->data)) < 0) {
                        WARNING("Failed to remove element.");
                        return false;
                }

                reindex_tasks(entry);
                return true;
        }

        return false;
}

void traverse_tasks(Tasks *entry, void (*func)(Task *task))
{
        if (entry == NULL) {
                WARNING("Bad parameter -> entry == NULL.");
                return;
        }

        if (func == NULL) {
                WARNING("Bad parameter -> func == NULL.");
                return;
        }

        for (TasksElmt *el = tasks_head(entry); el != NULL; el = next_elmt(el)) {
                Task *task = extract_task(el);

                if (task == NULL) {
                        WARNING("Failed to extract task.");
                        return;
                }

                func(task);
        }
}

Task *set_task(long index, char *date, bool status, char *subject)
{
        if (date == NULL) {
                WARNING("Bad parameter -> date == NULL.");
                return NULL;
        }

        if (subject == NULL) {
                WARNING("Bad parameter -> subject == NULL.");
                return NULL;
        }

        Task *new_task = malloc(sizeof(Task));

        if (new_task == NULL) {
                WARNING("Out of memory.");
                return NULL;
        }

        new_task->index = index;
        new_task->date = copy_str(date, DATESIZE);

        if (new_task->date == NULL) {
                WARNING("Bad value -> date == NULL.");
                return NULL;
        }

        new_task->status = status;
        new_task->subject = copy_str(subject, SUBJSIZE);

        if (new_task->subject == NULL) {
                WARNING("Bad value -> subject == NULL.");
                return NULL;
        }

        return new_task;
}

static char *copy_str(char *src, size_t size)
{
        if (src == NULL) {
                WARNING("Bad parameter -> src == NULL.");
                return NULL;
        }

        char *dest = malloc(size);

        if (dest == NULL) {
                WARNING("Failed to allocate memory.");
                return NULL;
        }

        strncpy(dest, src, size);

        return dest;
}

static Task *find_task(Tasks const *entry, long index)
{
        if (entry == NULL) {
                WARNING("Bad parameter -> entry == NULL.");
                return NULL;
        }

        TasksElmt *el = find_list_elmt(entry, index);

        if (el != NULL) {
                Task *task = extract_task(el);

                if (task == NULL) {
                        WARNING("Failed to extract task.");
                        return NULL;
                }

                return task;
        }

        return NULL;
}

static TasksElmt *find_list_elmt(Tasks const *entry, long index)
{
        if (entry == NULL) {
                WARNING("Bad parameter -> entry == NULL.");
                return NULL;
        }

        for (TasksElmt *el = tasks_head(entry); el != NULL; el = next_elmt(el)) {
                Task *task = extract_task(el);

                if (task == NULL) {
                        WARNING("Failed to extract task.");
                        return NULL;
                }

                if (task->index == index)
                        return el;
        }

        return NULL;
}

static void reindex_tasks(Tasks *entry)
{
        if (entry == NULL) {
                WARNING("Bad parameter -> entry == NULL.");
                return;
        }

        long i = 1;
        for (TasksElmt *el = tasks_head(entry); el != NULL; el = next_elmt(el)) {
                Task *task = extract_task(el);

                if (task == NULL) {
                        WARNING("Failed to extract task.");
                        return;
                }

                task->index = i++;
        }
}

static Task *extract_task(TasksElmt *el)
{
        if (el == NULL) {
                WARNING("Bad parameter -> el == NULL.");
                return NULL;
        }

        Task *task = (Task *) el->data;

        return task;
}

/* application-specific destroy function (required for dlist interface) */
void destroy_task(void *data)
{
        if (data != NULL) {
                Task *tmp = (Task *) data;
                free(tmp->date);
                tmp->date = NULL;
                free(tmp->subject);
                tmp->subject = NULL;
                free(tmp);
                tmp = NULL;
        }
}
