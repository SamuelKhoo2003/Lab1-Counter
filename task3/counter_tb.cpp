#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env) {
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);
    Vcounter* top = new Vcounter;
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("counter.vcd");

    if (vbdOpen() != 1) return(-1);
    vbdHeader("Lab 1: Counter");

    top->clk = 1;
    top->rst = 0;
    vbdSetMode(1);
    // this is the value to preload

    for(i=0; i<300;) {
        tfp->dump (2*i+clk);
        top->clk = vbdFlag();
        if (top->clk == 1) {
            i++;
        }
        // this is me controlling the i increment
        // this gives me warnings though so not a perfect solution
        top->eval();
        

        // sending the count value to the Vbuddy
        vbdHex(4, (int(top->count) >> 16) & 0xF); 
        vbdHex(3, (int(top->count) >> 8) & 0xF); 
        vbdHex(2, (int(top->count) >> 4) & 0xF); 
        vbdHex(1, int(top->count) & 0xF); 
        vbdPlot(int(top->count), 0, 255);
        vbdCycle(i+1); 

        if (Verilated::gotFinish()) exit(0);
    }

    vbdClose();
    tfp->close();
    exit(0);
}