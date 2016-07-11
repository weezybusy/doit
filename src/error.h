/**
 * @file error.h
 * @brief Macros for handling errors, warnings and info messaged.
 * @author Vitaliy Pisnya
 * @date July, 2016
 */

#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <errno.h>
#include <string.h>

/**
 * @brief Macro that provides errno output in a more convenient form.
 */
#define CLEAN_ERRNO() (errno == 0 ? "no errno" : strerror(errno))

/**
 * @brief Macro that shows detailed information about current error.
 *
 * Provided information contains:
 *      - error description
 *      - file name
 *      - line number
 *      - function name
 *      - errno message
 *
 * @param[in] M Message describing the cause of an error.
 */
#define ERROR(M) \
{ \
        fprintf(stderr, "\n[ERROR] %s\n(%s:%d:%s(): %s)\n", \
                        M, __FILE__, __LINE__, __func__, CLEAN_ERRNO()); \
        printf("\nPress <Enter> to exit the program..."); \
        while (getchar() != '\n') continue; \
}

/**
 * @brief Macro that shows warning with detailed information.
 *
 * Provided information contains:
 *      - warning description
 *      - file name
 *      - line number
 *      - function name
 *      - errno message
 *
 * @param[in] M Warning message.
 */
#define WARNING(M) \
{ \
        fprintf(stderr, "\n[WARNING] %s\n(%s:%d:%s(): %s)\n", \
                        M, __FILE__, __LINE__, __func__, CLEAN_ERRNO()); \
        printf("\nPress <Enter> to continue..."); \
        while (getchar() != '\n') continue; \
}

/**
 * @brief Macro that provides user with useful information.
 *
 * Provided information contains:
 *      - info message
 *      - file name
 *      - line number
 *      - function name
 *
 * @param[in] M Informative message.
 */
#define INFO(M) \
{ \
        fprintf(stderr, "\n[INFO] %s\n(%s:%d:%s())\n", M, __FILE__, __LINE__, \
                        __func__); \
        printf("\nPress <Enter> to continue..."); \
        while (getchar() != '\n') continue; \
}

/**
 * @brief Macro that checks if expression for validity.
 *
 * If expression is invalid, macro for error will be called and the program
 * flow will be passed to instructions defined below goto label at the end of
 * the function.
 *
 * @param[in] A Is an expression to be checked.
 * @param[in] M Message for error macro.
 */
#define CHECK(A, M) \
{ \
        if (!(A)) { \
                ERROR(M); \
                errno = 0; \
                goto error; \
        } \
}

#endif
