/**
 * @file main.c
 * @brief Main function of the program.
 * @author Vitaliy Pisnya
 * @date July, 2016
 */

#include <stdio.h>
#include <stdlib.h>

#include "date.h"
#include "error.h"
#include "io.h"
#include "tasks.h"
#include "types.h"

/**
 * @brief Main function.
 * TODO: add detailed description of the main function.
 * @return EXIT_SUCCESS on success, or EXIT_FAILURE otherwise.
 */
int main(void)
{
        atexit(clear_scr);

        Tasks entry;
        init_tasks(&entry, destroy_task);

        FILE *last_entry_fp = fopen(LAST_ENTRY, "a+");
        CHECK(last_entry_fp, "Failed to create/open last_entry.txt.");

        bool ret = false;

        if (!file_is_empty(last_entry_fp)) {
                char date[DATESIZE];
                ret = get_last_entry_date(last_entry_fp, date);
                CHECK(ret, "Failed to get last entry date.");

                if (is_outdated(date)) {
                        ret = save_entry_to_history(last_entry_fp);
                        CHECK(ret, "Failed to save entry to history.");
                } else {
                        ret = read_entry_from_file(last_entry_fp, &entry);
                        CHECK(ret, "Failed to read entry form file.");
                }
        }

        CHECK(show_tasks(&entry), "Failed to show tasks.");

        fclose(last_entry_fp);
        last_entry_fp = NULL;

        char *options = get_valid_opts(&entry);
        char option = get_opt(&entry, options);
        long task_index = 0L;

        while (option != 'q') {
                switch (option) {
                        case 'a':
                                ret = add_task(&entry, NULL, UNDONE);
                                CHECK(ret, "Failed to add task.");
                                break;

                        case 'c':
                                task_index = get_index(&entry);
                                CHECK(task_index > -1L, "Failed to get index.");
                                ret = change_task(&entry, task_index);
                                CHECK(ret, "Failed to change task.");
                                break;

                        case 'd':
                                task_index = get_index(&entry);
                                CHECK(task_index > -1L, "Failed to get index.");
                                ret = delete_task(&entry, task_index);
                                CHECK(ret, "Failed to delete task.");
                                break;

                        case 'e':
                                ret = erase_history(&entry);
                                CHECK(ret, "Failed to erase history.");
                                break;

                        case 'D':
                                destroy_tasks(&entry);
                                init_tasks(&entry, destroy_task);
                                break;

                        case 'h':
                                show_opts();
                                break;

                        case 'l':
                                ret = show_history();
                                CHECK(ret, "Failed to show history.");
                                break;

                        case 's':
                                ret = search_history(&entry);
                                CHECK(ret, "Failed to search history.");
                                break;

                        case 'u':
                                task_index = get_index(&entry);
                                CHECK(task_index > -1L, "Failed to get index.");
                                ret = undo_task(&entry, task_index);
                                CHECK(ret, "Failed to undo task.");
                                break;

                        case 'U':
                                traverse_tasks(&entry, uncheck_done);
                                break;

                        case 'x':
                                task_index = get_index(&entry);
                                CHECK(task_index > -1L, "Failed to get index.");
                                ret = do_task(&entry, task_index);
                                CHECK(ret, "Failed to do task.");
                                break;

                        case 'X':
                                traverse_tasks(&entry, check_as_done);
                                break;

                        default:
                                WARNING("You shouldn't be here.");
                                goto error;
                }

                options = get_valid_opts(&entry);
                option = get_opt(&entry, options);
        }

        last_entry_fp = fopen(LAST_ENTRY, "w");
        CHECK(last_entry_fp, "Failed to create/open last_entry.txt.");

        if (tasks_size(&entry) > 0)
                write_entry_to_file(last_entry_fp, &entry);

        fclose(last_entry_fp);
        last_entry_fp = NULL;
        destroy_tasks(&entry);
        exit(EXIT_SUCCESS);

error:
        if (last_entry_fp)
                fclose(last_entry_fp);
        last_entry_fp = NULL;

        destroy_tasks(&entry);
        exit(EXIT_FAILURE);
}
