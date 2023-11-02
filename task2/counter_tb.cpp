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
    top->rst = 1;

    for(i=0; i<300;i++) {
        for(clk=0;clk<2;clk++) {
            tfp->dump (2*i+clk);
            top->clk = !top->clk;
            top->eval ();
        }

        if (!vbdFlag()) {
            top->count -= 1;
            vbdHeader("Count Down");
        } else {
            top->count += 1;
            vbdHeader("Count Up");
        }
        // this is if i directly modify the count, bypassing the en function

        // sending the count value to the Vbuddy
        vbdHex(4, (int(top->count) >> 16) & 0xF); 
        vbdHex(3, (int(top->count) >> 8) & 0xF); 
        vbdHex(2, (int(top->count) >> 4) & 0xF); 
        vbdHex(1, int(top->count) & 0xF); 
        vbdCycle(i+1); 
        // end of Vbuddy output section 

        // vbdPlot(int(top->count), 0, 255);
        // this plots it instead of displaying it as 7 segement

        top->rst = (i<2) | (i==15);
        // top->en = (i>4); // no longer use this as we want to enable/disable the counter using the  rotary push-button switch
        if(Verilated::gotFinish()) exit(0);
    }

    vbdClose();
    tfp->close();
    exit(0);
}
