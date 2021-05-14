/*!
 * @file setAlarmInterrupt.ino
 * @brief Set alarm, and use interrput pin to trigger it
 * @n Experiment phenomenon: set the alarm clock to trigger at a specified time 
 * @n                        connect int pin with DIGITALPIN2
 * @n                        print information on serial port after the alarm clock is triggered.
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [LuoYufeng](yufeng.luo@dfrobot.com)
 * @version  V0.1
 * @date  2021-2-23
 * @url https://github.com/DFRobot/DFRobot_DS323X
 */
#include "DFRobot_DS323X.h"

volatile  int8_t alarmFlag = 0;

DFRobot_DS323X rtc;

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
    
    /*!
     *@brief enable Alarm1 interrupt
     */
    rtc.enableAlarm1Int();
    
    /*!
     *@brief disable Alarm1 interrupt
     */
    //rtc.disableAlarm1Int();
    
    /*!
     *@brief enable Alarm2 interrupt
     */
    rtc.enableAlarm2Int();
    
    /*!
     *@brief disable Alarm2 interrupt
     */
    //rtc.disableAlarm2Int();
    
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
    rtc.setAlarm1(rtc.eSecondsMatch,/*date,0-31*/1,/*hour,0-23*/0,/*minute,0-59*/0,/*second,0-59*/10);//Alarm1
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
    rtc.setAlarm2(rtc.eEveryMinute,/*date,0-31*/1,/*hour,0-23*/0,/*minute,0-59*/0);//Alarm2
    /*!
     *@brief Judge if it is power-down 
     *@return if return true, power-down, time needs to reset; false, work well
     */
    if (rtc.isLostPower())
        rtc.setTime(/*year,1900-2099*/2021, /*mouth,1-12*/2, /*date,1-31*/28, /*hour,0-23*/23,/*minute,0-59*/59,/*second,0-59*/55);//Set Set initial time .
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
    attachInterrupt(/*Interrupt No*/0,interrupt,FALLING);//Open the external interrupt 0, connect INT1/2 to the digital pin of the main control: 
    //UNO(2), Mega2560(2), Leonardo(3), microbit(P0).
    #endif
}
void loop() {
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
    if(alarmFlag == 1){
        alarmFlag = 0;
        Serial.println("Alarm clock is triggered.");
        delay(1000);
        rtc.clearAlarm();
    }
    else
        delay(1000);
    if (rtc.isLostPower()) {
        Serial.println("RTC lost power, please reset the time!");
    }
}

void interrupt(){
  alarmFlag = 1;
}
