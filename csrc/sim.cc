#include "sys.h"
#include "diff/difftest.h"
#include "include/dbg.h"

int32_t time_counter = 0;

void sim(const char *filepath) {
    std::ifstream file(filepath);

    // Initialize the CPU
    sys_init();

    // Execute the simulation
    sys_exec(file);

    // Check for any errors
    PRINT_RESULT(check_file(filepath));
}