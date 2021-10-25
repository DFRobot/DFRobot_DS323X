#-*- coding: utf-8 -*-
'''!
  @file  get_time_from_NTP.py
  @brief  Get the accurate time from NTP server
  @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license  The MIT License (MIT)
  @author  [LuoYufeng](yufeng.luo@dfrobot.com)
  @maintainer  [qsjhyy](yihuan.huang@dfrobot.com)
  @version  V1.0
  @date  2021-10-25
  @url  https://github.com/DFRobot/DFRobot_DS323X
'''
from __future__ import print_function
import sys
sys.path.append('../')
import time
import datetime
import ntplib
from DFRobot_DS323X import *

rtc = DFRobot_DS323X(bus=1)

#begin return True if succeed, otherwise return False
while not rtc.begin():
    time.sleep(2)

client = ntplib.NTPClient()
response = client.request('pool.ntp.org')
ntptime = datetime.datetime.fromtimestamp(response.tx_time)
year = ntptime.year     #Set the year from NTP server
month = ntptime.month   #Set the month from NTP server
date = ntptime.day      #Set the date from NTP server
hour = ntptime.hour     #Set the hour from NTP server
minute = ntptime.minute #Set the minute from NTP server
second = ntptime.second #Set the second from NTP server

rtc.set_time(year, month, date, hour, minute, second)


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
