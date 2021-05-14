#-*- coding: utf-8 -*-
'''
@file get_time_and_temp.py

@brief Get the time and temperature of chip

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
@brief Set mode of time
@param mode H24hours, H12hours
'''
rtc.set_hour_system(rtc.H12hours)

'''
@brief Set time into rtc and take effect immediately
@param year, 1900~2100
@param month, 1~12
@param date, 1~31
@param hour, 0~23
@param minute, 0~59
@param second, 0~59
'''
rtc.set_time(year=2021,month=2,date=28,hour=11,minute=59,second=55)

def main():
    while True:
        temp = rtc.get_temperature_C()
        #temp = rtc.get_temperature_F()
        if rtc.is_lost_power() == 1:
            print("RTC lost power, please reset the time!")
        print("{0}/{1}/{2},{3},{4}:{5}:{6} {7}".format(rtc.get_year(),rtc.get_month(),rtc.get_date(),\
        rtc.get_day_of_week(),rtc.get_hour(),rtc.get_minute(),rtc.get_second(),rtc.get_AM_or_PM()))#print now time
        
        print(temp)
        print(" ")
        time.sleep(1)

if __name__ == "__main__":
    main()

