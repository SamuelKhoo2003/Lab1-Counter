#include "Vtop.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp" 

#define MAX_SYM_CYC 100000

int main(int argc, char **argv, char **env){
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);

    // init top verilog instance
    Vtop* top = new Vtop;

    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("top.vcd");

    if (vbdOpen() != 1) return (-1);
    vbdHeader("L1T4: BCD numbers");
    //

    // initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 0;
    // top->v = 1;

    for (i = 0; i < MAX_SYM_CYC; i++) {
        
        // dump variables into VCD file and toggle clock
        for (clk = 0; clk < 2; clk++){
            tfp->dump (2*i + clk); // unit is in ps!!!
            top->clk = !top->clk;
            top->eval();
        }
        
        // Output count values to 7-seg display every cycle
        vbdHex(4, (int(top->bcd) >> 16) & 0xF);
        vbdHex(3, (int(top->bcd) >> 8) & 0xF);
        vbdHex(2, (int(top->bcd) >> 4) & 0xF);
        vbdHex(1, int(top->bcd) & 0xF);

        vbdCycle(i + 1);
        // ---- end of Vbuddy output section

        // Edit 
        top->rst = (i < 2) || (i == 15);
        top->en = vbdFlag();

        if (Verilated::gotFinish()) exit(0);
    }

    vbdClose(); // housekeeping
    tfp->close();
    exit(0);
}