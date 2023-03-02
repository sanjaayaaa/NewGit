#include "I2CWrapper.h"
#include "SHT3x.h"

extern char str[] ;

/* Control Section */
float setTemperature = 30.0;
float proportionalConstants = 15;

float calculateRH(uint16_t srh)
{
    return( 100.0 * srh / 65535.0 ) ;
}

float calculateTempratureInCelsius(uint16_t sc)
{
    return( -45.0 + 175.0 * (float)sc / 65535.0 ) ;
}

float calculateTempratureInFahrenheit(uint16_t sf)
{
    return( -49.0 + 315.0 * (float)sf / 65535.0 ) ;
}

uint8_t calculateCRC(uint16_t data)
{
    uint8_t crc = 0xFF ;
    for (int j = 0; j < 2 ; j++) { 
        crc ^= ((uint8_t*)(&data))[1-j];
        for (int i = CRC_WIDTH ; i ; i--) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ POLYNOMIAL ;
            } else {
                crc <<= 1 ;
            }
        }
    }
    return crc;
}

enum MasterAPIReturn writeCommand(uint16_t command)
{
    enum MasterAPIReturn state = NoError ;
    uint8_t cmd_msb = (command >> 8) & 0xFF ;
    uint8_t cmd_lsb = command & 0xFF ;
    enum MasterAPIReturn result = i2cSendStart(SHT31_I2C_ADDRESS, 0) ;
    if (result == NoError) {
        state += 0x01 ;
        sprintf(str, "I2C Send Start: %d\n", result) ;
    }
    result = i2cWriteByte(cmd_msb) ;
    if (result == NoError) {
        state += 0x02 ;
        sprintf(str, "I2C Send CMD MSB: %d\n", result) ;
    }    
    result = i2cWriteByte(cmd_lsb) ;
    if (result == NoError) {
        state += 0x04 ;
        sprintf(str, "I2C Send CMD LSB: %d\n", result) ;
    }  
    return state;
}

enum MasterAPIReturn singleRead(uint16_t *temprature) 
{
    enum MasterAPIReturn result = 0 ;
    uint8_t data[6] ;
    result = writeCommand(SingleShotWithClockStretching_H) ;
    CyDelay(10) ;
    
    result = i2cSendRestart(SHT31_I2C_ADDRESS, 1) ;
    CyDelay(1) ;
    
    data[0] = i2cReadByte(I2C_ACK_DATA) ; /* temp MSB */
    data[1] = i2cReadByte(I2C_ACK_DATA) ; /* temp LSB */
    data[2] = i2cReadByte(I2C_ACK_DATA) ; /* temp CRC */
    data[3] = i2cReadByte(I2C_ACK_DATA) ; /* hume MSB */
    data[4] = i2cReadByte(I2C_ACK_DATA) ; /* hume LSB */
    data[5] = i2cReadByte(I2C_NAK_DATA) ; /* hume CRC */
    i2cSendStop() ;
    *temprature = (data[0] << 8) | data[1] ;
    if (data[2] == calculateCRC(*temprature)) {
        UART_PutString("OK") ;
    } else {
        UART_PutString("Error!\n") ;
        result |= 0x01 ;
    }
    return result;
}

enum MasterAPIReturn resetSHT3x(void) 
{
    enum MasterAPIReturn result = writeCommand(ResetCommand) ;
    if (result == NoError) {
        sprintf(str, "Reset Status: %d\n", result) ;
        UART_PutString(str) ;
    }
    i2cSendStop() ;
    return result;
}

enum MasterAPIReturn readSHT3xStatus(uint16_t *status) 
{
    uint8_t data[3] ;
    enum MasterAPIReturn result = writeCommand(ReadStatus) ;
    CyDelay(10) ;
    result = i2cSendRestart(SHT31_I2C_ADDRESS, 1) ;
    CyDelay(1) ;
    data[0] = i2cReadByte(I2C_ACK_DATA) ; /* status MSB */
    data[1] = i2cReadByte(I2C_ACK_DATA) ; /* status LSB */
    data[2] = i2cReadByte(I2C_NAK_DATA) ; /* status CRC */
    i2cSendStop() ;
    *status = (data[0] << 8) | data[1] ;
    if (data[2] == calculateCRC(*status)) {
        UART_PutString("OK") ;
    } else {
        UART_PutString("Error!\n") ;
        result++ ;
    }
    return result ;
}

enum MasterAPIReturn clearSHT3xStatus(void) 
{
    enum MasterAPIReturn result = writeCommand(ClearStatus) ;
    i2cSendStop() ;
    return result;
}

enum MasterAPIReturn heater(int enable) 
{
    if (enable) {
        return writeCommand(HeaterOn) ;
    }
    return writeCommand(HeaterOff) ;
}

float computePValue(float currentTemperature)
{
    return (proportionalConstants * (setTemperature - currentTemperature));
}