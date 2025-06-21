#include "csrc/sys.h"

Vmodule* dut = new Vmodule;

VerilatedFstC* tfp = new VerilatedFstC;

std::fstream logFile("cache.log");

int main(int argc, char** argv) {
    Verilated::traceEverOn(true);
    dut->trace(tfp, 0);
    tfp->open("wave.fst");

    sim(dut, tfp, argv[1]);

    return 0;
}