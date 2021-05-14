/*!
 * @file setSqwAnd32k.ino
 * @brief Set the mode of pin sqw and 32k output
 *
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [LuoYufeng](yufeng.luo@dfrobot.com)
 * @version  V0.1
 * @date  2021-2-23
 * @url https://github.com/DFRobot/DFRobot_DS323X
 */
#include "DFRobot_DS323X.h"

DFRobot_DS323X rtc;


void setup(void)
{
    Serial.begin(9600);
    /*Wait for the chip to be initialized completely, and then exit*/
    while(rtc.begin() != true){
        Serial.println("Failed to init chip, please check if the chip connection is fine. ");
        delay(1000);
    }
    /*!
    *@brief Set the vaule of pin sqw
    *@param mode eSquareWave_OFF  = 0x1C // Not output square wave, enter interrupt mode
    *@n          eSquareWave_1Hz  = 0x00 // 1Hz square wave
    *@n          eSquareWave_1kHz = 0x08 // 1kHz square wave
    *@n          eSquareWave_4kHz = 0x10 // 4kHz square wave
    *@n          eSquareWave_8kHz = 0x18 // 8kHz square wave
    */
    rtc.writeSqwPinMode(rtc.eSquareWave_1kHz);
    /*!
    *@brief Read the value of pin sqw
    *@return eOFF             = 0x1C // Not output square wave, enter interrupt mode
    *@n      eSquareWave_1Hz  = 0x00 // 1Hz square wave
    *@n      eSquareWave_1kHz = 0x08 // 1kHz square wave
    *@n      eSquareWave_4kHz = 0x10 // 4kHz square wave
    *@n      eSquareWave_8kHz = 0x18 // 8kHz square wave
    */
    Serial.println(rtc.readSqwPinMode());
    /*!
     *@brief enable the 32k output 
     */
    rtc.enable32k();
    /*!
     *@brief disable the 32k output 
     */
    //rtc.disable32k();

}

void loop() {
}