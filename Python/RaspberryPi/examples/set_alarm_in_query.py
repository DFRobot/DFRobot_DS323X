#-*- coding: utf-8 -*-
'''
file set_alarm_in_query.py

@brief Set alarm clock and trigger in query

@Copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
@licence   The MIT License (MIT)

@author [LuoYufeng](yufeng.luo@dfrobot.com)
@url https://github.com/DFRobot/DFRobot_DS323X
@version  V1.0
@date  2021-3-4
'''
import sys
sys.path.append('../')
import time
from DFRobot_DS323X import *

rtc = DFRobot_DS323X(bus=1)

#begin return True if succeed, otherwise return False
while not rtc.begin():
    time.sleep(2)

'''
@brief Set the last compiled time as the current time
'''
rtc.set_time(2021,2,28,23,59,55)

'''
@brief Set alarm1 clock
@param alarmType:EverySecond,                   #repeat in every second
@n               SecondsMatch,                  #repeat in every minute
@n               SecondsMinutesMatch,           #repeat in every hour
@n               SecondsMinutesHoursMatch,      #repeat in every day
@n               SecondsMinutesHoursDateMatch,  #repeat in every month
@n               SecondsMinutesHoursDayMatch,   #repeat in every week  #Alarm1
@n               UnknownAlarm1
@param days      Alarm clock Day (0-31)
@param hours     Alarm clock Hour (0-23)
@param minutes   Alarm clock Minute (0-59)
@param seconds   Alarm clock Second (0-59)
'''
rtc.set_alarm1(alarmType=rtc.SecondsMatch,date=1,hour=0,minute=0,second=5)
'''
@brief Set alarm2 clock
@param alarmType:EveryMinute,                   #repeat in every minute
@n               MinutesMatch,                  #repeat in every hour
@n               MinutesHoursMatch,             #repeat in every day
@n               MinutesHoursDateMatch,         #repeat in every month
@n               MinutesHoursDayMatch,          #repeat in every week  #Alarm2
@n               UnknownAlarm2
@param days      Alarm clock Day (0-31)
@param hours     Alarm clock Hour (0-23)
@param minutes   Alarm clock Minute(0-59)
'''
rtc.set_alarm2(alarmType=rtc.MinutesHoursDayMatch,date=1,hour=0,minute=0)

def main():
    while True:
        if rtc.is_lost_power() == 1:
            print("RTC lost power, please reset the time!")
        print("{0}/{1}/{2},{3},{4}:{5}:{6} {7}".format(rtc.get_year(),rtc.get_month(),rtc.get_date(),\
        rtc.get_day_of_week(),rtc.get_hour(),rtc.get_minute(),rtc.get_second(),rtc.get_AM_or_PM()))#print now time
        if rtc.is_alarm_trig() == 1:
            print("Alarm1 clock is triggered.")
            rtc.clear_alarm()
        elif rtc.is_alarm_trig() == 2:
            print("Alarm2 clock is triggered.")
            rtc.clear_alarm()
        elif rtc.is_alarm_trig() == 3:
            print("All Alarms clock are triggered.")
            rtc.clear_alarm()
        print(" ")
        time.sleep(1)

if __name__ == "__main__":
    main()