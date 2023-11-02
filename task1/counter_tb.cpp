#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

int main(int argc, char **argv, char **env) {
    int i;
    int clk;
    int tmp;

    Verilated::commandArgs(argc, argv);
    Vcounter* top = new Vcounter;
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("counter.vcd");

    top->clk = 1;
    top->rst = 0;
    top->en = 1;

    bool continueCounting = true;
    
    for (i = 0; i < 300; i++) {
        for (clk = 0; clk < 2; clk++) {
            tfp->dump(2 * i + clk);
            top->clk = !top->clk;
            top->eval();
        }

        // Check if the counter has reached 0x9
        if (top->count == 0x9 and top->en == 1) {
            tmp = i;
            top->en = 0;
        }

        if (top->en == 0 and i == tmp + 3) {
                top->en = 1;
        }

        if (Verilated::gotFinish()) exit(0);
    }
    tfp->close();
    exit(0);
}