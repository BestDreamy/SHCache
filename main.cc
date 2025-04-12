#include "csrc/sys.h"

int main(int argc, char** argv) {
    Vmodule* dut = new Vmodule;

    VerilatedFstC* tfp = new VerilatedFstC;
    Verilated::traceEverOn(true);
    dut->trace(tfp, 0);
    tfp->open("wave.fst");

    std::ifstream file(argv[1]);
    sim(dut, tfp, file);

    return 0;
}