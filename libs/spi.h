/*
 * spi.h
 * Header file for SPI functions
 */

#ifndef _SPI_H
#define _SPI_H

#include "MK20D7.h"

// constants
#define SPI_MODE0 0x00  // CPOL = 0, CPHA = 0
#define SPI_MODE1 0x04  // CPOL = 0, CPHA = 1
#define SPI_MODE2 0x08  // CPOL = 1, CPHA = 0
#define SPI_MODE3 0x0C  // CPOL = 1, CPHA = 1

#define SPI_CLOCK_24MHZ (SPI_CTAR_DBR_MASK | SPI_CTAR_PBR(1) | SPI_CTAR_BR(0))  //(72 / 3) * ((1+1)/2)
#define SPI_CLOCK_18MHZ (SPI_CTAR_PBR(0) | SPI_CTAR_BR(0))                      //(72 / 2) * ((1+0)/2)
#define SPI_CLOCK_12MHZ (SPI_CTAR_DBR_MASK | SPI_CTAR_PBR(0) | SPI_CTAR_BR(2))  //(72 / 2) * ((1+1)/6)
#define SPI_CLOCK_8MHZ  (SPI_CTAR_DBR_MASK | SPI_CTAR_PBR(1) | SPI_CTAR_BR(2))  //(72 / 3) * ((1+1)/6)
#define SPI_CLOCK_6MHZ  (SPI_CTAR_PBR(0) | SPI_CTAR_BR(2))                      //(72 / 2) * ((1+0)/6)
#define SPI_CLOCK_4MHZ  (SPI_CTAR_PBR(1) | SPI_CTAR_BR(2)) 		                //(72 / 3) * ((1+0)/6)


// function prototypes
void SPIInit(uint8_t mode, uint32_t speed);
uint8_t SPIWrite8(uint8_t data);

#endif
