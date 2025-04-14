#ifndef DBG_H
#define DBG_H

#define dbg(a) std::cout << #a << " : " << (a) << std::endl

#define PRINT_RESULT(condition) \
    do { \
        if (condition) { \
            std::cout << "\033[1;32mPASS\033[0m" << std::endl; /* Green */ \
        } else { \
            std::cout << "\033[1;31mFAIL\033[0m" << std::endl; /* Red */ \
        } \
    } while (0)

#endif