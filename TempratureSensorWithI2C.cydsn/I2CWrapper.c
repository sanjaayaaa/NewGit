#include "I2CWrapper.h"

enum MasterStatus i2cStatus(void) 
{ 
    return (enum MasterStatus)I2C_MasterStatus(); 
}

enum MasterStatus i2cClearStatus(void) 
{ 
    return (enum MasterStatus) I2C_MasterClearStatus(); 
}

enum MasterAPIReturn i2cSendStart(uint8_t  i2cSlaveAddress, uint8_t rw) 
{
    return (enum MasterAPIReturn)I2C_MasterSendStart(i2cSlaveAddress, (uint32_t)rw);
}

enum MasterAPIReturn i2cSendRestart(uint8_t  i2cSlaveAddress, uint32_t bitRnW) 
{
    return (enum MasterAPIReturn) I2C_MasterSendRestart(i2cSlaveAddress, bitRnW);
}

enum MasterAPIReturn i2cSendStop(void) 
{ 
    return (enum MasterAPIReturn) I2C_MasterSendStop(); 
}

uint8_t  i2cReadByte(uint32_t acknak)
{
    return I2C_MasterReadByte(acknak);
}

uint8_t i2cReadReg(uint8_t  i2cSlaveAddress, uint8_t addr)
{
    i2cSendStart(i2cSlaveAddress, 0u) ;      /* for write */
    i2cWriteByte(addr) ; 
    i2cSendRestart(i2cSlaveAddress, 0x1u) ;  /* for read */
    CyDelay(1) ;
    uint8_t data = i2cReadByte(I2C_NAK_DATA) ;
    i2cSendStop() ;
    return data ;
}

void i2cReadRegs(uint8_t  i2cSlaveAddress, uint8_t addr, uint8_t *data, int len) 
{
    int i ;
    i2cSendStart(i2cSlaveAddress, 0u) ;      /* for write */
    i2cWriteByte(addr) ;
    i2cSendRestart(i2cSlaveAddress, 0x1u) ;  /* for read */
    for (i = 0 ; i < len-1 ; i++ ) {
        CyDelay(1) ;        
        data[i] = i2cReadByte(I2C_ACK_DATA) ;
    }
    data[i] = i2cReadByte(I2C_NAK_DATA) ;
    i2cSendStop() ;
}

enum MasterAPIReturn i2cWriteByte(uint8_t data) 
{
    return (enum MasterAPIReturn) I2C_MasterWriteByte(data) ;
}

void i2cWriteReg(uint8_t  i2cSlaveAddress, uint8_t addr, uint8_t data) 
{
    i2cSendStart(i2cSlaveAddress, 0u) ;      /* for write */
    i2cWriteByte(addr) ;    /* write register address */
    CyDelay(1) ;
    i2cWriteByte(data) ;
    i2cSendStop() ;
}

void i2cWriteRegs(uint8_t  i2cSlaveAddress, uint8_t addr, uint8_t *data, int len) 
{
    i2cSendStart(i2cSlaveAddress, 0u) ;      /* for write */
    i2cWriteByte(addr) ;
    i2cSendRestart(i2cSlaveAddress,0x0u) ;  /* for write */
    CyDelay(1) ;
    for (int i = 0 ; i < len ; i++ ) {
        i2cWriteByte(data[i]) ;
    }
    i2cSendStop() ;
}

