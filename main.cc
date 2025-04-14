#include "csrc/sys.h"

int main(int argc, char** argv) {
    Vmodule* dut = new Vmodule;

    VerilatedFstC* tfp = new VerilatedFstC;
    Verilated::traceEverOn(true);
    dut->trace(tfp, 0);
    tfp->open("wave.fst");

    sim(dut, tfp, argv[1]);

    return 0;
}