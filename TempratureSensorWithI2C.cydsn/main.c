#include "project.h"
#include <stdio.h>
#include <string.h>
#include "I2CWrapper.h"
#include "SHT3x.h"
#include "AD56xx.h"
char str[80] ;

enum AddressCommand{
    DAC_A  = 0x00,
    DAC_B = 0x01,
    AllDAC = 0x07,
}; 
#define DAC_Version_AD5623 0x00

void initHardware(void)
{
    CyGlobalIntEnable;
    UART_Start() ;
    I2C_Start() ;
    SPI_Start();
    SPI_ClearTxBuffer();
    CyDelay(1) ;
}

int main(void)
{
    initHardware() ;
    resetSHT3x() ;
    clearSHT3xStatus() ;
    heater(0) ;
    uint16_t temprature, status ;
            
    for(;;) {
        singleRead(&temprature) ;
        float floatTemprature = calculateTempratureInCelsius(temprature) ;
        CyDelay(1) ;
        readSHT3xStatus(&status) ;
        sprintf(str, " tem = %X.%X  ", (int)floatTemprature, (int)(floatTemprature * 100) % 100) ;
        UART_PutString(str) ;
        float PValue = computePValue(floatTemprature) ;
        sprintf(str, " PValue = %X.%01X  ", (int)PValue, (int)(PValue * 100) % 100) ;
        UART_PutString(str) ;
        if (status) {
            clearSHT3xStatus() ;
        }
        setDACValue(DAC_A, 0xFFFF, DAC_Version_AD5623); 
    }
}



