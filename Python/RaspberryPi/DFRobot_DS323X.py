#-*- coding: utf-8 -*-
""" 
@file DFRobot_DS323X.py

@brief Define the basic structure of class DFRobot_DS323X

@copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
@licence     The MIT License (MIT)
@author [LuoYufeng](yufeng.luo@dfrobot.com)
@version  V1.0
@date  2021-2-25
@get from https://www.dfrobot.com
@url https://github.com/DFRobot/DFRobot_DS323X
"""

import sys
sys.path.append('../')
import smbus 
from ctypes import*
import time


class DFRobot_DS323X:
    
    SquareWave_OFF     = 0x1C
    SquareWave_1Hz     = 0x00
    SquareWave_1kHz    = 0x08
    SquareWave_4kHz    = 0x10
    SquareWave_8kHz    = 0x18
    
    H24hours            = 0
    H12hours            = 1
    
    NoTrigger = 0
    Alarm1Trigger = 1
    Alarm2Trigger = 2
    AllTrigger = 3
    
    EverySecond                  = 0
    SecondsMatch                 = 1
    SecondsMinutesMatch          = 2
    SecondsMinutesHoursMatch     = 3
    SecondsMinutesHoursDateMatch = 4
    SecondsMinutesHoursDayMatch  = 5
    UnknownAlarm1                 = 6
    
    EveryMinute                  = 0
    MinutesMatch                 = 1
    MinutesHoursMatch            = 2
    MinutesHoursDateMatch        = 3
    MinutesHoursDayMatch         = 4

    UnknownAlarm2                 = 5

    _IIC_ADDRESS        = 0x68

    _REG_RTC_SEC        = 0X00
    _REG_RTC_MIN        = 0X01
    _REG_RTC_HOUR       = 0X02
    _REG_RTC_DAY        = 0X03
    _REG_RTC_DATE       = 0X04
    _REG_RTC_MONTH      = 0X05
    _REG_RTC_YEAR       = 0X06
    _REG_ALM1_SEC       = 0X07
    _REG_ALM1_MIN       = 0X08
    _REG_ALM1_HOUR      = 0X09
    _REG_ALM1_DAY       = 0X0A
    _REG_ALM2_MIN       = 0X0B
    _REG_ALM2_HOUR      = 0X0C
    _REG_ALM2_DAY       = 0X0D
    _REG_CONTROL        = 0x0E
    _REG_STATUS         = 0x0F
    _REG_AGE_OFFSET     = 0X10
    _REG_TEMPERATURE1   = 0x11
    _REG_TEMPERATURE2   = 0x12
    
    rtc_bcd = []
    bcd = []
    _d = 0
    _m = 0
    _y = 0
    _mode = 0
    ss = 0
    mm = 0
    hh = 0
    d = 0
    m = 0
    y = 0
    days_in_month = [31,28,31,30,31,30,31,31,30,31,30,31]
    day_of_the_week = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"]

    def __init__(self, bus):
        _deviceAddr = self._IIC_ADDRESS
        self.i2cbus=smbus.SMBus(bus)
        self.i2c_addr = self._IIC_ADDRESS
    
    def begin(self):
        if not self.scan():
            return False
        else:
            return True

    def date2days(self, y, m, d):
        if y >= 2000:
            y -= 2000
        days = d
        for i in range(1, m):
            days += self.days_in_month[i - 1]
        if (m > 2 and y % 4) == 0:
            ++days
        return days + 365 * y + int((y + 3) / 4) - 1

    def conv2d(self, p):
        v = 0
        if p >= '0' and p <= '9':
            v = p - '0'
        return 10 * v + ++p - '0'


    def bcd2bin(self, val):
        return val - 6 * (val >> 4)

    def bin2bcd (self, val):
        return val + 6 * int(val / 10)

    '''
    @brief Read the value of pin sqw
    @return mode SquareWave_OFF  = 0x1C # Not output square wave, enter interrupt mode
    @n           SquareWave_1Hz  = 0x00 # 1Hz square wave
    @n           SquareWave_1kHz = 0x08 # 1kHz square wave
    @n           SquareWave_4kHz = 0x10 # 4kHz square wave
    @n           SquareWave_8kHz = 0x18 # 8kHz square wave
    '''
    def read_sqw_pin_mode(self):        #Read the value of pin sqw
        mode = self.read_reg(self._REG_CONTROL)
        mode &= 0x1C;
        if (mode & 0x04):
            mode = SquareWave_OFF
        return mode

    '''
    @brief Set the vaule of pin sqw
    @param mode SquareWave_OFF  = 0x1C # Not output square wave, enter interrupt mode
    @n          SquareWave_1Hz  = 0x00 # 1Hz square wave
    @n          SquareWave_1kHz = 0x08 # 1kHz square wave
    @n          SquareWave_4kHz = 0x10 # 4kHz square wave
    @n          SquareWave_8kHz = 0x18 # 8kHz square wave
    '''
    def write_sqw_pin_mode(self, mode): #Set the vaule of pin sqw
        ctrl = self.read_reg(self._REG_CONTROL)
        ctrl &= 0x04
        ctrl &= 0x18
        ctrl |= mode
        
        self.write_reg(self._REG_CONTROL, ctrl);
    
    def day_of_week(self):
        day = self.date2days(self._y, self._m, self._d)
        return (day + 6) % 7
    
    '''
    @brief get day of week
    @return day of week
    '''
    def get_day_of_week(self):
        return self.day_of_the_week[self.day_of_week()]

    '''
    @brief Set mode of time
    @param mode H24hours, H12hours
    '''
    def set_hour_system(self, mode = H24hours):
        self._mode = mode

    '''
    @brief Set time into rtc and take effect immediately
    @param year, 1900~2100
    @param month, 1~12
    @param date, 1~31
    @param hour, 0~23
    @param minute, 0~59
    @param second, 0~59
    '''
    def set_time(self, year, month, date, hour, minute, second):
        if year >=2000:
            self.y = self.bin2bcd(year - 2000)
            self.m = self.bin2bcd(month + 80)
        else:
            self.y = self.bin2bcd(year - 1900)
            self.m = month
        self.d = self.bin2bcd(date)
        if self._mode == 0:
            self.hh = (self._mode << 5|self.bin2bcd(hour))
        else:
            if hour == 0:
                self.hh = 0x52;
            elif hour >0 and hour < 12:
                self.hh = (0x40|self.bin2bcd(hour))
            elif hour == 12:
                self.hh = 0x72
            elif hour >12 and hour < 24:
                self.hh = (0x60|self.bin2bcd(hour - 12));
        self.mm = self.bin2bcd(minute)
        self.ss = self.bin2bcd(second)
        #data = [self.ss, self.mm, self.hh, self.day_of_week(), self.d, self.m, self.y]
        self.write_reg(self._REG_RTC_SEC, self.ss)
        self.write_reg(self._REG_RTC_MIN, self.mm)
        self.write_reg(self._REG_RTC_HOUR, self.hh)
        self.write_reg(self._REG_RTC_DATE, self.d)
        self.write_reg(self._REG_RTC_MONTH, self.m)
        self.write_reg(self._REG_RTC_YEAR, self.y)
        statreg = self.read_reg(self._REG_STATUS)
        statreg &= ~0x80
        self.write_reg(self._REG_STATUS, statreg)
    
    '''
    @brief output AM or PM of time 
    @return AM or PM, 24 hours mode return empty string
    '''
    def get_AM_or_PM(self):
        buffer = self.read_reg(self._REG_RTC_HOUR)
        buffer = buffer & 0x60
        if (buffer >> 5) <= 1:
            return " "
        elif (buffer >> 5) == 2:
            return "AM"
        elif (buffer >> 5) == 3:
            return "PM"
    
    '''
    @brief get year of now time
    @return year
    '''
    def get_year(self):
        year = self.read_reg(self._REG_RTC_YEAR)
        self._y = self.bcd2bin(year) + 1900
        century = self.read_reg(self._REG_RTC_MONTH)
        if century > 80:
            self._y += 100
        return self._y
    
    '''
    @brief get month of now time
    @return month
    '''
    def get_month(self):
        month = self.read_reg(self._REG_RTC_MONTH)
        self._m = self.bcd2bin(month)
        if self._m > 80:
            self._m -= 80
        return self._m
    
    '''
    @brief get date of now time
    @return date
    '''
    def get_date(self):
        date = self.read_reg(self._REG_RTC_DATE)
        self._d = self.bcd2bin(date)
        return self._d 
    
    '''
    @brief get hour of now time
    @return hour
    '''
    def get_hour(self):
        hour = self.read_reg(self._REG_RTC_HOUR)
        if hour >> 6 == True:
            self._hh = self.bcd2bin(hour & 0x1F)
        else:
            self._hh = self.bcd2bin(hour & 0x3F)
        return self._hh
    
    '''
    @brief get minute of now time
    @return minute
    '''
    def get_minute(self):
        minute = self.read_reg(self._REG_RTC_MIN)
        self._mm = self.bcd2bin(minute)
        return self._mm
    
    '''
    @brief get second of now time
    @return second
    '''
    def get_second(self):
        second = self.read_reg(self._REG_RTC_SEC)
        self._ss = self.bcd2bin(second & 0x7F)
        return self._ss
    
    '''
    @brief get temperature of sensor
    @return temperature, unit:℃
    '''
    def get_temperature_C(self):
        buf1 = self.read_reg(self._REG_TEMPERATURE1)
        buf2 = self.read_reg(self._REG_TEMPERATURE2)
        return (buf1 + (buf2>>6)*0.25)
    
    '''
    @brief get temperature of sensor, unit:℉
    @return temperature, unit:℉
    '''
    def get_temperature_F(self):
        buf1 = self.read_reg(self._REG_TEMPERATURE1)
        buf2 = self.read_reg(self._REG_TEMPERATURE2)
        c = (buf1 + (buf2>>6)*0.25)
        return c * 9 / 5 + 32
    
    '''
    @brief check if rtc has been lost power
    @return True means rtc has lost power before and need to reset time; False means rtc operates well
    '''
    def is_lost_power(self):
        status = self.read_reg(self._REG_STATUS)
        return status >> 7

    '''
    @brief Set alarm1 clock
    @param alarmType:EverySecond,                   #repeat in every second
    @n               SecondsMatch,                  #repeat in every minute
    @n               SecondsMinutesMatch,           #repeat in every hour
    @n               SecondsMinutesHoursMatch,      #repeat in every day
    @n               SecondsMinutesHoursDateMatch,  #repeat in every month
    @n               SecondsMinutesHoursDayMatch,   #repeat in every week  #Alarm1
    @n               UnknownAlarm1
    @param days    1-31
    @param hours   0-23
    @param minutes 0-59
    @param seconds 0-59
    '''
    def set_alarm1(self, alarmType, date, hour, minute, second):
        dates = self.bin2bcd(date)
        if self._mode == 0:
            hours = (self._mode << 5|self.bin2bcd(hour))
        else:
            if hour == 0:
                hours = 0x52;
            elif hour >0 and hour < 12:
                hours = (0x40|self.bin2bcd(hour))
            elif hour == 12:
                hours = 0x72
            elif hour >12 and hour < 24:
                hours = (0x60|self.bin2bcd(hour - 12))
        minutes = self.bin2bcd(minute)
        seconds = self.bin2bcd(second)
        days = self.bin2bcd(self.day_of_week())
        if alarmType >= self.UnknownAlarm1:
            return
        self.write_reg(self._REG_ALM1_SEC, seconds)
        self.write_reg(self._REG_ALM1_MIN, minutes)
        self.write_reg(self._REG_ALM1_HOUR, hours)
        if alarmType == self.SecondsMinutesHoursDateMatch:
            self.write_reg(self._REG_ALM1_DAY, dates)
        else:
            self.write_reg(self._REG_ALM1_DAY, days);
        if alarmType < self.SecondsMinutesHoursDateMatch:
            buffer = self.read_reg(self._REG_ALM1_DAY)
            buffer |= 0x80
            self.write_reg(self._REG_ALM1_DAY, buffer)
        if alarmType < self.SecondsMinutesHoursMatch:
            buffer = self.read_reg(self._REG_ALM1_HOUR)
            buffer |= 0x80
            self.write_reg(self._REG_ALM1_HOUR, buffer)
        if alarmType < self.SecondsMinutesMatch:
            buffer = self.read_reg(self._REG_ALM1_MIN)
            buffer |= 0x80
            self.write_reg(self._REG_ALM1_MIN, buffer)
        if(alarmType == self.EverySecond):
            buffer = self.read_reg(self._REG_ALM1_SEC)
            buffer |= 0x80
            self.write_reg(self._REG_ALM1_SEC, buffer)
        if(alarmType == self.SecondsMinutesHoursDayMatch):
            buffer = self.read_reg(self._REG_ALM1_DAY)
            buffer |= 0x40
            self.write_reg(self._REG_ALM1_DAY, buffer)
        self.clear_alarm()
        return

    '''
    @brief Set alarm2 clock
    @param alarmType:EveryMinute,           //repeat in every minute
    @n               MinutesMatch,          //repeat in every hour
    @n               MinutesHoursMatch,     //repeat in every day
    @n               MinutesHoursDateMatch, //repeat in every month
    @n               MinutesHoursDayMatch,  //repeat in every week  #Alarm2
    @n               UnknownAlarm2
    @param days    1-31
    @param hours   0-23
    @param minutes 0-59
    '''
    def set_alarm2(self, alarmType, date, hour, minute):
        dates = self.bin2bcd(date)
        if self._mode == 0:
            hours = (self._mode << 5|self.bin2bcd(hour))
        else:
            if hour == 0:
                hours = 0x52;
            elif hour >0 and hour < 12:
                hours = (0x40|self.bin2bcd(hour))
            elif hour == 12:
                hours = 0x72
            elif hour >12 and hour < 24:
                hours = (0x60|self.bin2bcd(hour - 12))
        minutes = self.bin2bcd(minute)
        days = self.bin2bcd(self.day_of_week())
        if alarmType >= self.UnknownAlarm2:
            return
        self.write_reg(self._REG_ALM2_MIN, minutes)
        self.write_reg(self._REG_ALM2_HOUR, hours)
        if alarmType == self.MinutesHoursDateMatch:
            self.write_reg(self._REG_ALM2_DAY)
        elif alarmType == self.MinutesHoursDayMatch:
            days |= 0x80
            self.write_reg(self._REG_ALM2_DAY, days)
        if alarmType < self.MinutesHoursDateMatch:
            buffer = self.read_reg(self._REG_ALM2_DAY)
            buffer |= 0x80
            self.write_reg(self._REG_ALM2_DAY, buffer)
        if alarmType < self.MinutesHoursMatch:
            buffer = self.read_reg(self._REG_ALM2_HOUR)
            buffer |= 0x80
            self.write_reg(self._REG_ALM2_HOUR, buffer)
        if alarmType == self.EveryMinute:
            buffer = self.read_reg(self._REG_ALM2_MIN,)
            buffer |= 0x80
            self.write_reg(self._REG_ALM2_MIN, buffer)
        self.clear_alarm()
        return
    
    '''
    @brief enable the interrupt of alarm1
    '''
    def enable_alarm1_int(self):
        conReg = self.read_reg(self._REG_CONTROL)
        conReg |= 0x01
        self.write_reg(self._REG_CONTROL, conReg)
    
    '''
    @brief disable the interrupt of alarm1
    '''
    def disable_alarm1_int(self):
        conReg = self.read_reg(self._REG_CONTROL)
        conReg &= 0xFE
        self.write_reg(self._REG_CONTROL, conReg)
    
    '''
    @brief enable the interrupt of alarm2
    '''
    def enable_alarm2_int(self):
        conReg = self.read_reg(self._REG_CONTROL)
        conReg |= 0x02
        self.write_reg(self._REG_CONTROL, conReg)
    
    '''
    @brief disable the interrupt of alarm2
    '''
    def disable_alarm2_int(self):
        conReg = self.read_reg(self._REG_CONTROL)
        conReg &= 0xFD
        self.write_reg(self._REG_CONTROL, conReg)
    
    '''
    @brief check if alarm flag has been triggered
    @return True, triggered; False, not triggered 
    '''
    def is_alarm_trig(self):
        staReg = self.read_reg(self._REG_STATUS)
        return staReg&3
    
    '''
    @brief clear alarm flag trigger
    '''
    def clear_alarm(self):
        staReg = self.read_reg(self._REG_STATUS)
        staReg &= 0xFC
        self.write_reg(self._REG_STATUS, staReg)

    '''
    @brief enable the 32k output 
    '''
    def enable_32k(self):
        staReg = self.read_reg(self._REG_STATUS)
        staReg|= 0x08
        self.write_reg(self._REG_STATUS, staReg)
    
    '''
    @brief disable the 32k output (default is enabled)
    '''
    def disable_32k(self):
        staReg = self.read_reg(self._REG_STATUS)
        staReg &= 0xF7
        self.write_reg(self._REG_STATUS, staReg)

    '''
    @brief write data into the SRAM
    @param addr, address of SRAM
    @param data
    '''
    def write_SRAM(self, addr, data):
        self.write_reg(addr, data)

    '''
    @brief read data of the SRAM
    @param addr, address of SRAM
    '''
    def read_SRAM(self, addr):
        return self.read_reg(addr)

    '''
    @brief clear the SRAM
    @param addr, address of SRAM
    '''
    def clear_SRAM(self, addr):
        self.write_reg(addr, 0x00)

    def write_reg(self, reg, buff):
        self.i2cbus.write_byte_data(self.i2c_addr, reg, buff)
        

    def read_reg(self, reg):
        return self.i2cbus.read_byte_data(self.i2c_addr, reg) 

    def scan(self):
        try:
            self.i2cbus.write_quick(self.i2c_addr)
            return True
        except:
            print("I2C init fail")
            return False
