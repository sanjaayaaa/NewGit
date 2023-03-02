#pragma once
#include <project.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

enum MasterStatus{
    ClearStatusValue = 0x00u,
    ReadComplete = 0x01u,
    WriteComplete = 0x02u,
    MasterTransferInProgress = 0x04u,
    TransferHalted =  0x08u,
    MaskForAllErrors = 0xF0u,
    MasterNAKedBeforePacketEnd = 0x10u,
    SlaveNotACK = 0x20u,
    MasterLostArbitrationDuringCommunication = 0x40u,
    ErrorDuringTransfer = 0x80u,
};

enum MasterAPIReturn{
    NoError = 0x00u,
    BusIsBusy = 0x01u,
    MasterNotReady = 0x02u,
    LastByteNaked = 0x03u,
    ArbitrationLostError = 0x04u,
    AbortAsSlaveAddressedBeforeStartGen = 0x05u,
}; 

enum MasterStatus i2cStatus(void);
enum MasterStatus i2cClearStatus(void) ;

enum MasterAPIReturn i2cSendStart(uint8_t  i2cSlaveAddress, uint8_t readwrite) ;
enum MasterAPIReturn i2cSendRestart(uint8_t  i2cSlaveAddress, uint32_t bitRnW) ;
enum MasterAPIReturn i2cSendStop(void) ;
enum MasterAPIReturn i2cWriteByte(uint8_t data) ;

uint8_t  i2cReadByte(uint32_t acknak) ;
uint8_t  i2cReadReg(uint8_t  i2cSlaveAddress, uint8_t addr) ;

void  i2cWriteReg(uint8_t  i2cSlaveAddress, uint8_t addr, uint8_t data) ;
void  i2cReadRegs(uint8_t  i2cSlaveAddress, uint8_t addr, uint8_t *data, int len) ;
void  i2cWriteRegs(uint8_t  i2cSlaveAddress, uint8_t addr, uint8_t *data, int len) ;
