/**
 * @file date.c
 * @brief definitions of routines for working with dates.
 * @author Vitaliy Pisnya
 * @date June, 2016
 */

#include "date.h"

bool get_curr_date(char *date)
{
        if (date == NULL) {
                WARNING("Bad parameter -> date == NULL.");
                return false;
        }

        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        strftime(date, DATESIZE, "%d.%m.%Y", t);

        return true;
}

bool get_last_entry_date(FILE *fp, char *date)
{
        if (fp == NULL) {
                WARNING("Bad parameter -> fp == NULL.");
                return false;
        }

        if (date == NULL) {
                WARNING("Bad parameter -> date == NULL.");
                return false;
        }

        char line[LINESIZE] = { 0 };

        if (fgets(line, DATESIZE, fp) == NULL) {
                WARNING("Failed to read line form a file.");
                rewind(fp);
                return false;
        }

        strncpy(date, line, DATEOFFSET);
        date[DATESIZE - 1] = '\0';

        if (!date_is_valid(date)) {
                WARNING("Date is not valid.");
                rewind(fp);
                return false;
        }

        rewind(fp);
        return true;
}

bool date_is_valid(char *date)
{
        if (date == NULL) {
                WARNING("Bad parameter -> date == NULL.");
                return false;
        }

        char *tmp = malloc(DATESIZE);

        if (tmp == NULL) {
                WARNING("Out of memory.");
                return false;
        }

        strncpy(tmp, date, DATESIZE);

        char *delim = ".";
        char *d = strtok(tmp, delim);

        if (d == NULL)
                goto fail;

        char *m = strtok(NULL, delim);

        if (m == NULL)
                goto fail;

        char *y = strtok(NULL, delim);

        if (y == NULL)
                goto fail;

        long day = strtol(d, NULL, 10);
        long month = strtol(m, NULL, 10);
        long year = strtol(y, NULL, 10);

        if ((day < 1) || (day > 31))
                goto fail;

        if ((month < 1) || (month > 12))
                goto fail;

        if (year < 2016 || year > 2060)
                goto fail;

        free(tmp);
        tmp = NULL;
        return true;

fail:
        free(tmp);
        tmp = NULL;
        return false;
}

bool is_outdated(char *date)
{
        if (date == NULL) {
                WARNING("Bad parameter -> date == NULL.");
                return false;
        }

        char curr_date[DATESIZE] = { 0 };

        if (!get_curr_date(curr_date)) {
                WARNING("Failed to get current date.");
                return false;
        }

        return !STRCMP(date, ==, curr_date);
}

void show_curr_date(void)
{
        char date[DATESIZE] = { 0 };
        if (!get_curr_date(date)) {
                WARNING("Failed to get current date.");
                return;
        }

        printf("%s\n", date);
}
