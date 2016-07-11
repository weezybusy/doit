/**
 * @file io.c
 * @brief Function definitions for working with I/O.
 * @author Vitaliy Pisnya
 * @date July, 2016
 */

#include "io.h"

static void print_taskline(long index, bool status, char *subject);

void clear_scr(void)
{
        printf("\033[2J");
        printf("\033[1;1H");
}

void clear_buf(void)
{
        while (getchar() != '\n')
                continue;
}

void show_opts(void)
{
        clear_scr();
        do {
                printf("available options:\n"
                                "------------------\n"
                                " a: add task\n"
                                " c: change task\n"
                                " d: delete task\n"
                                " D: delete all tasks\n"
                                " e: erase history\n"
                                " h: help\n"
                                " l: list history\n"
                                " q: quit the program\n"
                                " s: search history by date\n"
                                " u: undo task\n"
                                " U: undo all tasks\n"
                                " x: do task\n"
                                " X: do all tasks\n"
                                "------------------\n"
                                "press <Enter> to go back...");
        } while (getchar() != '\n');
}

char *get_valid_opts(const Tasks *entry)
{
        return tasks_size(entry) == 0 ? OPTIONS1 : OPTIONS2;
}

int get_char(void)
{
        int ch = getchar();

        if (ch == '\n')
                ungetc(ch, stdin);

        clear_buf();

        return ch;
}

int get_opt(Tasks *entry, const char *opts)
{
        if (entry == NULL) {
                WARNING("Bad parameter -> entry == NULL.");
                return -1;
        }

        if (opts == NULL) {
                WARNING("Bad parameter -> opts == NULL.");
                return -1;
        }

        int option = 0;

        do {
                show_tasks(entry);

                if (STRCMP(opts, ==, "yn"))
                        printf("Are you sure? <y/n>: ");
                else
                        printf("action: ");

                option = get_char();
        } while (!opt_is_valid(opts, option));

        return option;
}

bool opt_is_valid(const char *opts, char opt)
{
        if (opts == NULL) {
                WARNING("Bad parameter -> opts == NULL.");
                return false;
        }

        return strchr(opts, opt) != NULL;
}

long get_long(void)
{
        char str[3];

        if (get_str(str, sizeof(str), stdin))
                return strtol(str, NULL, 10);
        else
                return -1L;
}

long get_index(Tasks *entry)
{
        if (entry == NULL) {
                WARNING("Bad parameter -> entry == NULL.");
                return -1L;
        }

        long task_index = 0L;

        do {
                show_tasks(entry);
                printf("index: ");
                task_index = get_long();
        } while (!index_is_valid(task_index, entry));

        show_tasks(entry);
        return task_index;
}

bool index_is_valid(long index, const Tasks *entry)
{
        if (entry == NULL) {
                WARNING("Bad parameter -> entry == NULL.");
                return false;
        }

        return (index > 0) && (index <= tasks_size(entry));
}

bool get_str(char *str, int size, FILE *stream)
{
        if (str == NULL) {
                WARNING("Bad parameter -> str == NULL.");
                return false;
        }

        if (size < 2) {
                WARNING("Bad parameter -> size < 2.");
                return false;
        }

        if (stream == NULL) {
                WARNING("Bad parameter -> stream == NULL.");
                return false;
        }

        if (fgets(str, size, stream) == NULL)
                return false;

        char *newline = strchr(str, '\n');

        if (newline)
                *newline = '\0';
        else
                clear_buf();

        return true;
}

bool get_date(Tasks *entry, char *str, int size)
{
        if (entry == NULL) {
                WARNING("Bad parameter -> entry == NULL.");
                return false;
        }

        if (str == NULL) {
                WARNING("Bad parameter -> str == NULL.");
                return false;
        }

        if (size < 2) {
                WARNING("Bad parameter -> size < 2.");
                return false;
        }

        do {
                show_tasks(entry);
                printf("date: dd.mm.yyyy\b\b\b\b\b\b\b\b\b\b");

                if (!get_str(str, size, stdin))
                        continue;

        } while (!date_is_valid(str));

        return true;
}

bool stat_is_valid(char status)
{
        if ((status != '+') && (status != '-')) {
                WARNING("Invalid task status.");
                return false;
        }

        return true;
}

bool file_is_empty(FILE *fp)
{
        if (fp == NULL) {
                WARNING("Bad parameter -> fp == NULL.");
                return false;
        }

        fseek(fp, 0L, SEEK_END);

        if (ftell(fp) == 0) {
                rewind(fp);
                return true;
        } else {
                rewind(fp);
                return false;
        }
}

bool read_entry_from_file(FILE *fp, Tasks *entry)
{
        if (fp == NULL) {
                WARNING("Bad parameter -> fp == NULL.");
                return false;
        }

        if (entry == NULL) {
                WARNING("Bad parameter -> entry == NULL.");
                return false;
        }

        char line[LINESIZE] = { 0 };

        while (get_str(line, LINESIZE, fp)) {

                char date[DATESIZE] = { 0 };
                char subject[SUBJSIZE] = { 0 };
                bool status = false;

                parse_line(line, date, &status, subject);

                if (!add_task(entry, subject, status)) {
                        WARNING("Failed to add task.");
                        return false;
                }
        }
        return true;
}

bool write_entry_to_file(FILE *fp, Tasks *entry)
{
        if (fp == NULL) {
                WARNING("Bad parameter -> fp == NULL.");
                return false;
        }

        if (entry == NULL) {
                WARNING("Bad parameter -> entry == NULL.");
                return false;
        }

        for (TasksElmt *el = tasks_head(entry); el != NULL; el = next_elmt(el)) {
                Task *task = (Task *) el->data;
                fprintf(fp, "%s %s %s\n",
                                task->date, task->status ? "+" : "-",
                                task->subject);
        }

        return true;
}

bool save_entry_to_history(FILE *fp)
{
        if (fp == NULL) {
                WARNING("fp == NULL.");
                return false;
        }

        FILE *history_fp = NULL;
        char line[LINESIZE] = { 0 };

        history_fp = fopen(HISTORY, "a+");
        if (history_fp == NULL) {
                WARNING("Failed to create/open history.txt.");
                return false;
        }

        while (fgets(line, LINESIZE, fp))
                fputs(line, history_fp);

        fclose(history_fp);
        history_fp = NULL;
        return true;
}

bool show_history(void)
{
        FILE *history_fp = fopen(HISTORY, "a+");

        if (history_fp == NULL) {
                WARNING("Failed to open history.txt.");
                return false;
        }

        if (file_is_empty(history_fp)) {
                fclose(history_fp);
                history_fp = NULL;
                return true;
        }

        clear_scr();
        rewind(history_fp);

        char line[LINESIZE] = { 0 };
        char prev_date[DATESIZE] = { 0 };
        int  entries_sum = 0;
        int  tasks_sum = 0;

        while (fgets(line, LINESIZE, history_fp)) {

                char curr_date[DATESIZE] = { 0 };
                char subject[SUBJSIZE] = { 0 };
                bool status = false;

                parse_line(line, curr_date, &status, subject);
                if (!STRCMP(prev_date, ==, curr_date)) {

                        ++entries_sum;

                        if (entries_sum > 1)
                                putchar('\n');

                        tasks_sum = 0;

                        printf("%s\n", curr_date);
                        SEPARATOR();
                }

                ++tasks_sum;
                print_taskline(tasks_sum, status, subject);
                strncpy(prev_date, curr_date, DATESIZE);
        }

        printf("\n---------------------\n");
        printf("Total sum of entries: %d\n", entries_sum);
        printf("\npress <Enter> to go back...");
        clear_buf();

        fclose(history_fp);
        history_fp = NULL;
        return true;
}

bool search_history(Tasks *entry)
{
        if (entry == NULL) {
                WARNING("Bad parameter -> entry == NULL.");
                return false;
        }

        FILE *history_fp = fopen(HISTORY, "r");
        if (history_fp == NULL) {
                WARNING("Failed to open history.txt.");
                return false;
        }

        if (file_is_empty(history_fp)) {
                fclose(history_fp);
                history_fp = NULL;
                return true;
        }

        char search_date[DATESIZE] = { 0 };
        get_date(entry, search_date, DATESIZE);

        clear_scr();

        printf("%s\n", search_date);
        SEPARATOR();

        char line[LINESIZE] = { 0 };
        long count = 0L;

        while (fgets(line, LINESIZE, history_fp)) {

                char tmp_date[DATESIZE] = { 0 };
                char subject[SUBJSIZE] = { 0 };
                bool status = false;

                parse_line(line, tmp_date, &status, subject);

                if (STRCMP(search_date, ==, tmp_date)) {
                        Task *task = set_task(count + 1, search_date, status,
                                        subject);
                        if (task == NULL) {
                                WARNING("Failed to setup task.");
                                fclose(history_fp);
                                history_fp = NULL;
                                return false;
                        }

                        show_task(task);
                        destroy_task(task);
                        count++;
                }

                if (STRCMP(tmp_date, >, search_date))
                        break;
        }

        if (count == 0)
                printf(" no match\n");

        SEPARATOR();
        printf("Press <Enter> to go back...");
        clear_buf();

        fclose(history_fp);
        history_fp = NULL;
        return true;

}

bool erase_history(Tasks *entry)
{
        if (entry == NULL) {
                WARNING("Bad parameter -> entry == NULL.");
                return false;
        }

        FILE *history_fp = fopen(HISTORY, "r");
        if (history_fp == NULL) {
                WARNING("Failed to open history.txt.");
                return false;
        }

        if (file_is_empty(history_fp)) {
                fclose(history_fp);
                history_fp = NULL;
                return true;
        }

        fclose(history_fp);
        history_fp = NULL;

        char choice = get_opt(entry, "yn");
        if (choice == 'n')
                return true;

        history_fp = fopen(HISTORY, "w");
        if (history_fp == NULL) {
                WARNING("Failed to open history.txt.");
                return false;
        }

        fclose(history_fp);
        history_fp = NULL;
        return true;
}

bool show_tasks(Tasks *entry)
{
        if (entry == NULL) {
                WARNING("Bad parameter -> entry == NULL.");
                return false;
        }

        clear_scr();
        show_curr_date();
        SEPARATOR();
        if (tasks_size(entry) == 0)
                printf(" no tasks\n");
        else
                traverse_tasks(entry, show_task);
        SEPARATOR();

        return true;
}

void show_task(Task *task)
{
        if (task == NULL) {
                WARNING("Bad parameter -> task == NULL.");
                return;
        }

        print_taskline(task->index, task->status, task->subject);
}

bool parse_line(char *line, char *date, bool *status, char *subject)
{
        if (line == NULL) {
                WARNING("Bad parameter -> line == NULL.");
                return false;
        }

        if (date == NULL) {
                WARNING("Bad parameter -> date == NULL.");
                return false;
        }

        if (status == NULL) {
                WARNING("Bad parameter -> status == NULL.");
                return false;
        }

        if (subject == NULL) {
                WARNING("Bad parameter -> subject == NULL.");
                return false;
        }

        strncpy(date, line, DATEOFFSET);
        date[DATEOFFSET] = '\0';

        if (!date_is_valid(date)) {
                WARNING("Date is not valid.");
                return false;
        }

        if(!stat_is_valid(line[STATOFFSET])) {
                WARNING("Task status isn't valid.");
                return false;
        }

        *status = line[STATOFFSET] == '+';

        strncpy(subject, line + SUBJOFFSET, SUBJSIZE);
        char *new_line = strchr(subject, '\n');

        if (new_line)
                *new_line = '\0';

        return true;
}

static void print_taskline(long index, bool status, char *subject)
{
        if (subject == NULL) {
                WARNING("Bad parameter -> subject == NULL.");
                return;
        }

        printf(" %ld [%c] %s\n", index, status ? 'X' : ' ', subject);
}
