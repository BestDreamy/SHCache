#include "verilated_fst_c.h"
#include "Vswitch.h"
#include "csrc/utils.h"

int main(int argc, char** argv) {
    Vswitch* dut = new Vswitch;

    VerilatedFstC* tfp = new VerilatedFstC;
    Verilated::traceEverOn(true);
    dut->trace(tfp, 0);
    tfp->open("wave.fst");

    uint time_counter = 0;
    int clk = 0;
    while (!Verilated::gotFinish() and time_counter < 50) {
        dut->clk = clk;
        dut->eval();

        tfp->dump(time_counter ++);
        // assert(dut->f == (a ^ b));

        clk = clk ^ 1;
    }
    tfp->close();
    delete dut;
    return 0;
}