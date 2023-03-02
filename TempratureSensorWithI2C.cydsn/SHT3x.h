#pragma once
#include "project.h"

/* 
   if ADDR (pin2) = LOW  
   if ADDR (pin2) = HIGH 
   #define SHT3X_I2C_ADDRESS 0x45
*/
#define SHT31_I2C_ADDRESS 0x44

#define CRC_WIDTH 8
#define POLYNOMIAL 0x31 


enum SingleShotCommands{
    SingleShotWithClockStretching_H =  0x2C06,
    SingleShotWithClockStretching_M =  0x2C0D,
    SingleShotWithClockStretching_L =  0x2C10,
    SingleShotWithoutClockStretching_H =  0x2400,
    SingleShotWithoutClockStretching_M =  0x240B,
    SingleShotWithoutClockStretching_L =  0x2416,
};

enum Command{
   ARTCommand = 0x2B32,
   BreakCommand = 0x3090,
   ResetCommand = 0x30A2,
};

enum HeaterCommand{
   HeaterOn = 0x306D,
   HeaterOff = 0x3066,
};

enum StatusRegister{
   ReadStatus = 0xF32D,
   ClearStatus = 0x3041,
};

float calculateRH(uint16_t srh) ;
float calculateTempratureInCelsius(uint16_t sc) ;
float calculateTempratureInFahrenheit(uint16_t sf) ;
uint8_t calculateCRC(uint16_t data) ;

enum MasterAPIReturn singleRead(uint16_t *temprature) ;
enum MasterAPIReturn resetSHT3x(void) ;
enum MasterAPIReturn readSHT3xStatus(uint16_t *status) ;
enum MasterAPIReturn clearSHT3xStatus(void) ;
enum MasterAPIReturn heater(int enable) ;

float computePValue(float currentTemperature);


