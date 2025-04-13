#include "sys.h"

int32_t time_counter = 0;

void sim(Vmodule *dut, VerilatedFstC *tfp, std::ifstream &file) {
    // Initialize the CPU
    sys_init(dut, tfp);

    // Execute the simulation
    sys_exec(dut, tfp, file);

    // Finalize the simulation
    tfp->close();
    delete dut;
}