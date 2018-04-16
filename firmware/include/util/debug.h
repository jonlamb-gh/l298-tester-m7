/**
 * @file debug.h
 * @brief TODO.
 *
 */

#ifndef DEBUG_H
#define DEBUG_H

#ifdef BUILD_TYPE_DEBUG
    #include <stdio.h>
    #define debug_init() do{ \
            setvbuf(stdin, NULL, _IONBF, 0); \
            setvbuf(stdout, NULL, _IONBF, 0); \
            setvbuf(stderr, NULL, _IONBF, 0); } \
            while(0)
    #define debug_puts(s) puts(s "\r")
    #define debug_printf(f, ...) printf(f, ##__VA_ARGS__)
#else
    #define debug_init()
    #define debug_puts(s)
    #define debug_printf(f, ...)
#endif

#endif /* DEBUG_H */
