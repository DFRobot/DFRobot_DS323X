/*!
 * @file getTimeFromPC.ino
 * @brief get time when the PC compile. Pass the value obtained by subtracting the PC system time from the serial port printing time after the first upload into the
 * @n function as the compensation time, upload again and write the compiled time into the module.
 * @n Experiment phenomenon: You need to close the IDE and recompile, otherwise the correct system time cannot be obtained。 
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
    //rtc.setHourSystem();//Set time mode, default in the 24 hours mode, e24hours, eAM, ePM.
    /*!
     *@brief Set the last compiled time as the current time
     *@param comSec Compensation time, the value obtained by subtracting the PC system time from the serial port printing time after the first upload, unit: s.
     */
    rtc.getCompileTime(/*comSec*/0);//Set the last compiled time as the current time

}

void loop() {
    Serial.print(rtc.getYear(), DEC);//year
    Serial.print('/');
    Serial.print(rtc.getMonth(), DEC);//month
    Serial.print('/');
    Serial.print(rtc.getDate(), DEC);//date
    Serial.print(" (");
    Serial.print(rtc.getDayOfWeek());//day of week
    Serial.print(") ");
    Serial.print(rtc.getHour(), DEC);//hour
    Serial.print(':');
    Serial.print(rtc.getMinute(), DEC);//minute
    Serial.print(':');
    Serial.print(rtc.getSecond(), DEC);//second
    Serial.print(' ');
    /*if rtc works in 24hours mode,this function doesn't print anything*/
    Serial.print(rtc.getAMorPM());
    Serial.println();
    delay(1000);
    /*!
     *@brief Judge if it is power-down
     *@return If retrun true, power down, needs to reset time; false, work well.
     */
    if (rtc.isLostPower()) {
        Serial.println("RTC lost power, plrase reset the time!");
    }
}
