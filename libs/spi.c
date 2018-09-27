/*
 * spi.c
 * Source file for SPI functions
 */

#include <stdint.h>
#include "spi.h"

void SPIInit(uint8_t mode, uint32_t speed){
    // configure pins
    SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
    PORTC->PCR[4] |= PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;   // CS
    PORTC->PCR[5] |= PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;   // SCK
    PORTC->PCR[6] |= PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;   // MOSI
    PORTC->PCR[7] |= PORT_PCR_MUX(2);                       // MISO
    
    SIM->SCGC6 |= SIM_SCGC6_SPI0_MASK;                  // enable clock to spi module
    SPI0->MCR = SPI_MCR_MDIS_MASK | SPI_MCR_HALT_MASK;  // allow disabling, stop transfers
    
    SPI0->CTAR[0] = speed | SPI_CTAR_FMSZ(7);           // 7+1 bits frame size
    
    // set spi mode
    switch(mode){
        case SPI_MODE0: // CPOL = 0, CPHA = 0
            SPI0->CTAR[0] &= ~SPI_CTAR_CPOL_MASK;
            SPI0->CTAR[0] &= ~SPI_CTAR_CPHA_MASK;
            break;
        case SPI_MODE1: // CPOL = 0, CPHA = 1
            SPI0->CTAR[0] &= ~SPI_CTAR_CPOL_MASK;
            SPI0->CTAR[0] |= SPI_CTAR_CPHA_MASK;
            break;
        case SPI_MODE2: // CPOL = 1, CPHA = 0
            SPI0->CTAR[0] |= SPI_CTAR_CPOL_MASK;
            SPI0->CTAR[0] &= ~SPI_CTAR_CPHA_MASK;
            break;
        case SPI_MODE3: // CPOL = 1, CPHA = 1
            SPI0->CTAR[0] |= SPI_CTAR_CPOL_MASK;
            SPI0->CTAR[0] |= SPI_CTAR_CPHA_MASK;
            break;
        default:        // mode 0, most common
            SPI0->CTAR[0] &= ~SPI_CTAR_CPOL_MASK;
            SPI0->CTAR[0] &= ~SPI_CTAR_CPHA_MASK;
            break;
    }
    
    SPI0->MCR = SPI_MCR_MSTR_MASK | SPI_MCR_PCSIS(0x1F);    // master mode, inactive is high
}

uint8_t SPIWrite8(uint8_t data){
    SPI0->SR |= SPI_SR_TCF_MASK;
    SPI0->PUSHR = SPI_PUSHR_PCS(0) | data;
    while(!(SPI0->SR & SPI_SR_TCF_MASK));
    
    return SPI0->POPR;
}
