#pragma once
#include "project.h"


enum CommandDefinition{
    WriteToInputRegister = 0x00,
    UpdateDAC = 0x01,
    SoftwareLDAC = 0x02,
    WriteToAndUpdateDAC = 0x03,
    PowerDown = 0x04,
    Reset = 0x05,
    LDACRegisterSetup = 0x06,
    InternalReferenceSetup = 0x07,
}; 

enum DACVersion{
    AD5623 = 0x00,
    AD5643 = 0x01,
    AD5663 = 0x02,
}; 

uint8_t createShiftRegisterMSB(uint8_t address, enum CommandDefinition command);
void setDACValue(uint8_t address, uint16_t value, enum DACVersion version);