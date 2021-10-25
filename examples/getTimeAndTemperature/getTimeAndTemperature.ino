/*!
 * @file  getTimeAndTemperature.ino
 * @brief  Show current time and temperature of chip
 * @n  Experiment phenomenon: Set original time by users themselves 
 * @n  and get real time and temperature from the chip
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license  The MIT License (MIT)
 * @author  [yufeng](yufeng.luo@dfrobot.com)
 * @maintainer  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2021-10-25
 * @url  https://github.com/DFRobot/DFRobot_DS323X
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
     *@brief Set time output format
     *@param eHours_t:e24hours, e12hours. default is e24hours
     */
    rtc.setHourSystem(rtc.e12hours);
    rtc.setTime(/*year,1900-2100*/2021, /*mouth,1-12*/2, /*date,1-31*/28, /*hour,0-23*/23,/*minute,0-59*/59,/*second,0-59*/55);//Set Set initial time .

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
    Serial.print("Temperature: ");
    /*!
     *@brief Get sensor temperature
     *@param mode Select the sensor temperature measurement mode, default is false
     *@n true Enable temperature conversion to measure and obtain the current temperature immediately
     *@n false Disable temperature conversion and obtain the temperature measurement data within 64 seconds
     *@n       (the sensor measures the temperature every 64 seconds by default)
     *@return Sensor temperautre, unit: ℃ 
     */
    Serial.print(rtc.getTemperatureC(true));
    Serial.println(" C");
    delay(1000);

    /*!
     *@brief Judge if it is power-down
     *@return If retrun true, power down, needs to reset time; false, work well.
     */
    if (rtc.isLostPower()) {
        Serial.println("RTC lost power, please reset the time!");
    }
}
