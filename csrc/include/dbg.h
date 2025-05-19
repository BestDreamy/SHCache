#ifndef DBG_H
#define DBG_H
#include <stdio.h>
#include "../include/utils.h"

#define RESET_TXT "\033[0m"
#define RED_TXT "\033[1;31m"
#define GREEN_TXT "\033[1;32m"
#define BLUE_TXT "\033[1;34m"
#define YELLOW_TXT "\033[1;33m"

#define dbg(a) std::cout << #a << " : " << (a) << std::endl

#define PRINT_RESULT(condition) \
    do { \
        if (condition) { \
            std::cout << "\033[1;32mPASS\033[0m" << std::endl; /* Green */ \
        } else { \
            std::cout << "\033[1;31mFAIL\033[0m" << std::endl; /* Red */ \
        } \
    } while (0)

#define Log(format, ...) \
    do { \
        printf(ANSI_FMT("[%s:%d %s] " format, BLUE_TXT) "\n", \
        __FILE__, __LINE__, __func__, ## __VA_ARGS__); \
    } while(0)

inline void assert_fail_msg(const char* msg) {
    printf(RED_TXT "%s\n" RESET_TXT, msg);
}

#define Assert(cond, msg) \
    do { \
        if (!(cond)) { \
            assert_fail_msg(msg); \
        } \
        assert(cond); \
    } while(0)
    
#define Exit(cond, msg) \
    do { \
        if (!(cond)) { \
            assert_fail_msg(msg); \
            tfp -> close(); \
        } \
        if (!(cond)) exit(0); \
    } while(0)
#endif