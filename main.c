#include <stdint.h>
#include "MK20D7.h"
#include "libs/spi.h"

int main(){
    /*
    // disable watchdog
    WDOG->UNLOCK = 0xC520;
    WDOG->UNLOCK = 0xD928;
    // WDOG_STCTRLH: ?=0,DISTESTWDOG=0,BYTESEL=0,TESTSEL=0,TESTWDOG=0,?=0,STNDBYEN=1,WAITEN=1,STOPEN=1,DBGEN=0,ALLOWUPDATE=1,WINEN=0,IRQRSTEN=0,CLKSRC=1,WDOGEN=0
    WDOG->STCTRLH = 0x01D2;
    */
    
    // clock init...
    OSC->CR = OSC_CR_SC8P_MASK | OSC_CR_SC2P_MASK | OSC_CR_ERCLKEN_MASK;    // enable 8pf, 2pf, external reference
    
    MCG->C2 = MCG_C2_RANGE0(2) | MCG_C2_EREFS0_MASK;        // very high freq range, external oscillator
    
    MCG->C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(4);             // external reference clock, div/512
    while(!(MCG->S & MCG_S_OSCINIT0_MASK));                 // wait for crystal to start
    while((MCG->S & MCG_S_IREFST_MASK) == 0);               // wait FLL to use external clock
    while((MCG->S & MCG_S_CLKST_MASK) != MCG_S_CLKST(2));   // wait for external clock to be selected
    
    MCG->C5 = MCG_C5_PRDIV0(7);                             // PLL freq = 16MHz/8
    
    MCG->C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV0(12);          // select PLL output as mcg source, multiply by 36
    while(!(MCG->S & MCG_S_PLLST_MASK));                    // wait for PLLS clock to be PLL
    while(!(MCG->S & MCG_S_LOCK0_MASK));                    // wait for PLL to lock
    
    MCG->C1 = MCG_C1_FRDIV(4);                              // clear CLKS to select PLL as system clock
    while((MCG->S & MCG_S_CLKST_MASK) != MCG_S_CLKST(3));   // wait until PLL is selected to feed MCGOUTCLK
    
    
    SPIInit(SPI_MODE0, SPI_CLOCK_4MHZ);
    
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;                     // enable clock for port b
    PORTB->PCR[16] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;   // set as gpio, drive strength enable
    PTB->PDDR = (1 << 16);
    
    volatile uint32_t counter;
    while(1){
        //PTB->PTOR = (1 << 16);
        SPIWrite8(0xDE);
        SPIWrite8(0xAD);
        SPIWrite8(0xBE);
        SPIWrite8(0xEF);
        //for(counter = 0; counter < 1000000; counter++);
        PTB->PTOR = (1 << 16);
        for(counter = 0; counter < 1000000; counter++);
    }
}
