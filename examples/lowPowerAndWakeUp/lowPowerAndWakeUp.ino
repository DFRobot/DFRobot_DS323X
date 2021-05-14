/*!
 * @file lowPowerAndWakeUp.ino
 * @brief Set alarm, and use interrput pin to trigger MCU wake up
 * @n Experiment phenomenon: Set the alarm clock to trigger at a specified time.
 * @n                        Connect SQW pin with DIGITALPIN2.
 * @n                        Print information on serial port after the alarm clock is triggered.
 * @n                        This demo only works on avr
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [LuoYufeng](yufeng.luo@dfrobot.com)
 * @version  V0.1
 * @date  2021-2-23
 * @url https://github.com/DFRobot/DFRobot_DS323X
 */
#include "DFRobot_DS323X.h"
#include <avr/sleep.h>

volatile  int8_t alarmFlag = 0;

DFRobot_DS323X rtc;
int t = 0;

void setup(void)
{
    Serial.begin(9600);
    /*Wait for the chip to be initialized completely, and then exit*/
    while(rtc.begin() != true){
        Serial.println("failed to init chip, please check if the chip connection is correct. ");
        delay(1000);
    }
    /*!
     *@brief Set the value of pin sqw
     *@param mode eSquareWave_OFF  = 0x1C // Not output square wave, enter interrupt mode
     *@n          eSquareWave_1Hz  = 0x00 // 1Hz square wave
     *@n          eSquareWave_1kHz = 0x08 // 1kHz square wave
     *@n          eSquareWave_4kHz = 0x10 // 4kHz square wave
     *@n          eSquareWave_8kHz = 0x18 // 8kHz square wave
     */
    rtc.writeSqwPinMode(rtc.eSquareWave_OFF);
    rtc.enableAlarm1Int();
    //rtc.disableAlarm1Int();
    //rtc.enableAlarm2Int();
    rtc.disableAlarm2Int();
    //rtc.setHourSystem();    //set hour system of time, e24hours,e12hours, default is the 24 hours mode.
    /*!
     *@brief Set alarm clock
     *@param alarmType Alarm clock working mode typedef enum{
     *@n                                  eEverySecond,
     *@n                                  eSecondsMatch,
     *@n                                  eSecondsMinutesMatch,
     *@n                                  eSecondsMinutesHoursMatch,
     *@n                                  eSecondsMinutesHoursDateMatch,
     *@n                                  eSecondsMinutesHoursDayMatch, //Alarm1
     *@n                                  eUnknownAlarm1
     *@n                                  }eAlarm1Types_t;
     *@param days    Alarm clock Day (day)
     *@param hours   Alarm clock Hour (hour)
     *@param minutes Alarm clock Minute (minute)
     *@param seconds Alarm clock Second (second)
     */
    rtc.setAlarm1(rtc.eSecondsMatch,/*date,0-30*/1,/*hour,0-23*/0,/*minute,0-59*/0,/*second,0-59*/15);//Alarm1
    /*!
     *@brief Set alarm clock
     *@param alarmType Alarm clock working mode typedef enum{
     *@n                                  eEveryMinute,
     *@n                                  eMinutesMatch,
     *@n                                  eMinutesHoursMatch,
     *@n                                  eMinutesHoursDateMatch,
     *@n                                  eMinutesHoursDayMatch,        //Alarm2
     *@n                                  eUnknownAlarm2
     *@n                                  }eAlarm2Types_t;
     *@param days    Alarm clock Day (day)
     *@param hours   Alarm clock Hour (hour)
     *@param minutes Alarm clock Minute (minute)
     */
    //rtc.setAlarm2(rtc.eEveryMinute,/*date,0-30*/1,/*hour,1-12 in 12hours,0-23 in 24hours*/0,/*minute,0-59*/0);//Alarm2
    
    rtc.setTime(/*year,1900-2100*/2021, /*mouth,1-12*/2, /*date,1-31*/28, /*hour,0-23*/23,/*minute,0-59*/59,/*second,0-59*/55);//Set Set initial time .
    #if defined(ESP32) || defined(ESP8266)||defined(ARDUINO_SAM_ZERO)
    attachInterrupt(digitalPinToInterrupt(D6)/*Query the interrupt number of the D6 pin*/,interrupt,FALLING);
    #else
    /*    The Correspondence Table of AVR Series Arduino Interrupt Pins And Terminal Numbers
    * ---------------------------------------------------------------------------------------
    * |                                        |  DigitalPin  | 2  | 3  |                   |
    * |    Uno, Nano, Mini, other 328-based    |--------------------------------------------|
    * |                                        | Interrupt No | 0  | 1  |                   |
    * |-------------------------------------------------------------------------------------|
    * |                                        |    Pin       | 2  | 3  | 21 | 20 | 19 | 18 |
    * |               Mega2560                 |--------------------------------------------|
    * |                                        | Interrupt No | 0  | 1  | 2  | 3  | 4  | 5  |
    * |-------------------------------------------------------------------------------------|
    * |                                        |    Pin       | 3  | 2  | 0  | 1  | 7  |    |
    * |    Leonardo, other 32u4-based          |--------------------------------------------|
    * |                                        | Interrupt No | 0  | 1  | 2  | 3  | 4  |    |
    * |--------------------------------------------------------------------------------------
    */
    /*                      The Correspondence Table of micro:bit Interrupt Pins And Terminal Numbers
    * ---------------------------------------------------------------------------------------------------------------------------------------------
    * |             micro:bit                       | DigitalPin |P0-P20 can be used as an external interrupt                                     |
    * |  (When using as an external interrupt,      |---------------------------------------------------------------------------------------------|
    * |no need to set it to input mode with pinMode)|Interrupt No|Interrupt number is a pin digital value, such as P0 interrupt number 0, P1 is 1 |
    * |-------------------------------------------------------------------------------------------------------------------------------------------|
    */
    attachInterrupt(/*Interrupt No*/0,interrupt,FALLING);//Enable the external interrupt 0, connect INT1/2 to the digital pin of the main control: 
    //UNO(2), Mega2560(2), Leonardo(3), microbit(P0).
    #endif
    
    /*!
     *@brief Set avr sleep mode
     */
    set_sleep_mode (SLEEP_MODE_PWR_DOWN);
    sleep_cpu();
    sleep_enable();
    
}
void loop() {
    if(alarmFlag == 1){
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
        alarmFlag = 0;
        while (t < 10){
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
            delay(1000);
            t = t + 1;
        }
        t = 0;
    }
    else
        delay(1000);
    /*!
     *@brief Judge if it is power-down 
     *@return if return true, power-down, time needs to reset; false, work well
     */
    if (rtc.isLostPower()) {
        Serial.println("RTC lost power, please reset the time!");
    }
    sleep_enable();
    //energy.PowerDown();
}


void interrupt(){
    alarmFlag = 1;
    sleep_disable();
}
