#-*- coding: utf-8 -*-
'''
@file set_alarm_interrupt.py

@brief Set alarm, and use interrput pin to trigger it

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
import threading
import RPi.GPIO as GPIO
from DFRobot_DS323X import *

rtc = DFRobot_DS323X(bus=1)

GPIO.setmode(GPIO.BCM)


#begin return True if succeed, otherwise return False
while not rtc.begin():
    time.sleep(2)

'''
@brief Set the vaule of pin sqw
@param mode SquareWave_OFF  = 0x01 # Not output square wave, enter interrupt mode
@n          SquareWave_1Hz  = 0x00 # 1Hz square wave
@n          SquareWave_1kHz = 0x08 # 1kHz square wave
@n          SquareWave_4kHz = 0x10 # 4kHz square wave
@n          SquareWave_8kHz = 0x18 # 8kHz square wave
'''
rtc.write_sqw_pin_mode(rtc.SquareWave_OFF)

'''
@brief enable the interrupt of alarm
'''
rtc.enable_alarm1_int();#@enable Alarm1 interrupt
#rtc.disable_alarm1_int();#@disable Alarm1 interrupt
rtc.enable_alarm2_int();#@enable Alarm2 interrupt
#rtc.disable_alarm2_int();#@disable Alarm2 interrupt

'''
@brief Set the last compiled time as the current time
'''
#rtc.set_hour_system(rtc.H12hours)
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

IO1 = 21#set interrupt pin

def IO1_callback(channel):#callback function
    global rtc
    rtc.clear_alarm()
    print("Alarm clock is triggered.")
 
GPIO.setup(IO1, GPIO.IN)
'''
@brief When IO1 changes to high level from low level and a FALLING level transition occurs, way to run IO1_callback 
'''
GPIO.add_event_detect(IO1, GPIO.FALLING, callback = IO1_callback)

def main():
    while True:
        if rtc.is_lost_power() == 1:
            print("RTC lost power, please reset the time!")
        print("{0}/{1}/{2},{3},{4}:{5}:{6} {7}".format(rtc.get_year(),rtc.get_month(),rtc.get_date(),\
        rtc.get_day_of_week(),rtc.get_hour(),rtc.get_minute(),rtc.get_second(),rtc.get_AM_or_PM()))#print now time
        
        print(" ")
        time.sleep(1)

if __name__ == "__main__":
    main()
