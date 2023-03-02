#include "AD56xx.h"


uint8_t createShiftRegisterMSB(uint8_t address,  enum CommandDefinition command)
{
    return ((command << 3) | address);
}

void setDACValue(uint8_t address, uint16_t value, enum DACVersion version)
{
    uint8_t buffer[3];
    buffer[0] = createShiftRegisterMSB(address, WriteToAndUpdateDAC);
    buffer[1] = (value >> 8);
    switch (version){
        case AD5623:
          buffer[2] = (value) & 0xF0;
          break;
        case AD5643:
          buffer[2] = (value) & 0xFC;
          break;
        case AD5663:
          buffer[2] = (value);
          break;
        default:
          buffer[2] = 0x00;
          break;
    }
    SPI_PutArray(buffer, 3);
    while (SPI_GetTxBufferSize());  
}