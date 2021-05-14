/*!
 * @file setAlarmInQuery.ino
 * @brief Set alarm clock and trigger it in query
 * @n Experiment phenomenon: set the alarm clock to trigger at a sepcified time
 * @n           Trigger when the set time of clock is the same with the RTC time
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
        Serial.println("failed to init chip, please check if the chip connection is fine");
        delay(1000);
    }
    //rtc.setHourSystem();//Set time mode, default in the 24 hours mode, e24hours, eAM, ePM.
    /*!
     *@brief Set alarm clock
     *@param alarmType Alarm clock working mode typedef enum{
     *@n                                  eEverySecond,                 //repeat in every second
     *@n                                  eSecondsMatch,                //repeat in every minute
     *@n                                  eSecondsMinutesMatch,         //repeat in every hour
     *@n                                  eSecondsMinutesHoursMatch,    //repeat in every day
     *@n                                  eSecondsMinutesHoursDateMatch,//repeat in every month
     *@n                                  eSecondsMinutesHoursDayMatch, //repeat in every week  //Alarm1
     *@n                                  eUnknownAlarm1
     *@n                                  }eAlarm1Types_t;
     *@param days    Alarm clock Day (day)
     *@param hours   Alarm clock Hour (hour)
     *@param minutes Alarm clock Minute (minute)
     *@param seconds Alarm clock Second (second)
     */
    rtc.setAlarm1(rtc.eSecondsMatch,/*date,0-30*/1,/*hour,0-23*/0,/*minute,0-59*/0,/*second,0-59*/10);//Alarm1
    /*!
     *@brief Set alarm clock
     *@param alarmType Alarm clock working mode typedef enum{
     *@n                                  eEveryMinute,                 //repeat in every minute
     *@n                                  eMinutesMatch,                //repeat in every hour
     *@n                                  eMinutesHoursMatch,           //repeat in every day
     *@n                                  eMinutesHoursDateMatch,       //repeat in every month
     *@n                                  eMinutesHoursDayMatch,        //repeat in every week  //Alarm2
     *@n                                  eUnknownAlarm2
     *@n                                  }eAlarm2Types_t;
     *@param days    Alarm clock Day (day)
     *@param hours   Alarm clock Hour (hour)
     *@param minutes Alarm clock Minute (minute)
     */
    rtc.setAlarm2(rtc.eEveryMinute,/*date,0-30*/1,/*hour,0-23*/0,/*minute,0-59*/0);//Alarm2
    if (rtc.isLostPower())
        rtc.setTime(/*year,1901-2099*/2021, /*mouth,1-12*/2, /*date,1-31*/28, /*hour,0-23*/23,/*minute,0-59*/59,\
                    /*second,0-59*/55);//Set Set initial time .
}
void loop() {
    /*!
     *@brief Judge if the alarm clock is triggered
     *@return eNoTrigger          // No alarm is triggered
     *@n      eAlarm1Trigger      // Alarm1 is triggered
     *@n      eAlarm2Trigger      // Alarm2 is triggered
     *@n      eAllTrigger         // All alarms are triggered
     */
    if (rtc.isAlarmTrig() == rtc.eAlarm1Trigger){ // If the alarm bit is set
        Serial.println("Alarm1 clock is triggered.");
        /*!
         *@brief Clear trigger flag
         */
        rtc.clearAlarm();
    }else if (rtc.isAlarmTrig() == rtc.eAlarm2Trigger){
        Serial.println("Alarm2 clock is triggered.");
        rtc.clearAlarm();
    }else if (rtc.isAlarmTrig() == rtc.eAllTrigger){
        Serial.println("Both Alarm clocks are triggered.");
        rtc.clearAlarm();
    }
    Serial.print(rtc.getYear(), DEC);
    Serial.print('/');
    Serial.print(rtc.getMonth(), DEC);
    Serial.print('/');
    Serial.print(rtc.getDate(), DEC);
    Serial.print(" (");
    Serial.print(rtc.getDayOfWeek());
    Serial.print(") ");
    Serial.print(rtc.getHour(), DEC);
    Serial.print(':');
    Serial.print(rtc.getMinute(), DEC);
    Serial.print(':');
    Serial.print(rtc.getSecond(), DEC);
    Serial.print(' ');
    /*if rtc works in 24hours mode,this function doesn't print anything*/
    Serial.print(rtc.getAMorPM());
    Serial.println();
    if (rtc.isLostPower()) {
        Serial.println("RTC lost power, please reset the time!");
    }
    delay(1000);
}
