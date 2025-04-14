#include "sys.h"
#include "diff/difftest.h"
#include "include/dbg.h"

int32_t time_counter = 0;

void sim(Vmodule *dut, VerilatedFstC *tfp, const char *filepath) {
    std::ifstream file(filepath);

    // Initialize the CPU
    sys_init(dut, tfp);

    // Execute the simulation
    sys_exec(dut, tfp, file);

    // Check for any errors
    PRINT_RESULT(check_file(filepath));

    // Finalize the simulation
    tfp->close();
    delete dut;
}