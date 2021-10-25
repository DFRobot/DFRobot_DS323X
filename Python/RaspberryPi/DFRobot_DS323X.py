#-*- coding: utf-8 -*-
'''!
  @file  DFRobot_DS323X.py
  @brief  Define the basic structure of class DFRobot_DS323X
  @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license  The MIT License (MIT)
  @author  [LuoYufeng](yufeng.luo@dfrobot.com)
  @maintainer  [qsjhyy](yihuan.huang@dfrobot.com)
  @version  V1.0
  @date  2021-10-25
  @url  https://github.com/DFRobot/DFRobot_DS323X
'''
import time
import sys
sys.path.append('../')
import smbus
import logging
from ctypes import*


logger = logging.getLogger()
#logger.setLevel(logging.INFO)   # Display all print information
logger.setLevel(logging.FATAL)   # If you want to only display print errors, please use this option
ph = logging.StreamHandler()
formatter = logging.Formatter("%(asctime)s - [%(filename)s %(funcName)s]:%(lineno)d - %(levelname)s: %(message)s")
ph.setFormatter(formatter) 
logger.addHandler(ph)


class DFRobot_DS323X(object):
    '''!
      @brief Define DFRobot_DS323X class
      @details Used to drive the DS323X clock module
    '''

    ## Not output square wave, enter interrupt mode
    SquareWave_OFF     = 0x1C
    ## 1Hz square wave
    SquareWave_1Hz     = 0x00
    ## 1kHz square wave
    SquareWave_1kHz    = 0x08
    ## 4kHz square wave
    SquareWave_4kHz    = 0x10
    ## 8kHz square wave
    SquareWave_8kHz    = 0x18

    H24hours            = 0
    H12hours            = 1

    NoTrigger = 0
    Alarm1Trigger = 1
    Alarm2Trigger = 2
    AllTrigger = 3

    ## repeat in every second
    EverySecond                  = 0
    ## repeat in every minute
    SecondsMatch                 = 1
    ## repeat in every hour
    SecondsMinutesMatch          = 2
    ## repeat in every day
    SecondsMinutesHoursMatch     = 3
    ## repeat in every month
    SecondsMinutesHoursDateMatch = 4
    ## repeat in every week
    SecondsMinutesHoursDayMatch  = 5
    UnknownAlarm1                = 6

    ## repeat in every minute
    EveryMinute                  = 0
    ## repeat in every hour
    MinutesMatch                 = 1
    ## repeat in every day
    MinutesHoursMatch            = 2
    ## repeat in every month
    MinutesHoursDateMatch        = 3
    ## repeat in every week
    MinutesHoursDayMatch         = 4
    UnknownAlarm2                = 5

    ## I2C ADDRESS
    _IIC_ADDRESS        = 0x68

    ## RTC Seconds Register
    _REG_RTC_SEC        = 0x00
    ## RTC Minutes Register
    _REG_RTC_MIN        = 0x01
    ## RTC Hours Register
    _REG_RTC_HOUR       = 0x02
    ## RTC Day Register
    _REG_RTC_DAY        = 0x03
    ## RTC Date Register
    _REG_RTC_DATE       = 0x04
    ## RTC Month/Century Register
    _REG_RTC_MONTH      = 0x05
    ## RTC Year Register
    _REG_RTC_YEAR       = 0x06
    ## Alarm 1 Seconds Register
    _REG_ALM1_SEC       = 0x07
    ## Alarm 1 Minutes Register
    _REG_ALM1_MIN       = 0x08
    ## Alarm 1 Hours Register
    _REG_ALM1_HOUR      = 0x09
    ## Alarm 1 Day Register
    _REG_ALM1_DAY       = 0x0A
    ## Alarm 2 Minutes Register
    _REG_ALM2_MIN       = 0x0B
    ## Alarm 2 Hours Register
    _REG_ALM2_HOUR      = 0x0C
    ## Alarm 2 Day Register
    _REG_ALM2_DAY       = 0x0D
    ## Control Register
    _REG_CONTROL        = 0x0E
    ## Status Register
    _REG_STATUS         = 0x0F
    ## Aging Offset Register
    _REG_AGE_OFFSET     = 0x10
    ## temperature1 Register
    _REG_TEMPERATURE1   = 0x11
    ## temperature2 Register
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
        '''!
          @brief Module IIC communication init
          @param bus IIC communication bus
        '''
        _deviceAddr = self._IIC_ADDRESS
        self.i2cbus=smbus.SMBus(bus)
        self.i2c_addr = self._IIC_ADDRESS
    
    def begin(self):
        '''!
          @brief Init chip
          @return True means IIC communication succeeds, false means it fails.
        '''
        if not self._scan():
            return False
        else:
            return True

    def date2days(self, y, m, d):
        '''!
          @brief Date transfer day
          @param y The year of the current date
          @param m The month of the current date
          @param d The days of the current date
          @return day, Since 2000 years
        '''
        if y >= 2000:
            y -= 2000
        days = d
        for i in range(1, m):
            days += self.days_in_month[i - 1]
        if (m > 2 and y % 4) == 0:
            ++days
        return days + 365 * y + int((y + 3) / 4) - 1

    def conv2d(self, p):
        '''!
          @brief Convolution computation function
          @param p Data to be calculated
          @return operation result
        '''
        v = 0
        if p >= '0' and p <= '9':
            v = p - '0'
        return 10 * v + ++p - '0'


    def bcd2bin(self, val):
        '''!
          @brief BCD code to BIN code
          @param val Input BCD code
          @return Return BIN code
        '''
        return val - 6 * (val >> 4)

    def bin2bcd (self, val):
        '''!
          @brief BIN code to BCD code
          @param val Input BIN code
          @return Return BCD code
        '''
        return val + 6 * int(val / 10)

    def read_sqw_pin_mode(self):        #Read the value of pin sqw
        '''!
          @brief Read the value of pin sqw
          @return mode SquareWave_OFF  = 0x1C # Not output square wave, enter interrupt mode
          @n           SquareWave_1Hz  = 0x00 # 1Hz square wave
          @n           SquareWave_1kHz = 0x08 # 1kHz square wave
          @n           SquareWave_4kHz = 0x10 # 4kHz square wave
          @n           SquareWave_8kHz = 0x18 # 8kHz square wave
        '''
        mode = self._read_reg(self._REG_CONTROL)
        mode &= 0x1C
        if (mode & 0x04):
            mode = SquareWave_OFF
        return mode

    def write_sqw_pin_mode(self, mode): #Set the vaule of pin sqw
        '''!
          @brief Set the vaule of pin sqw
          @param mode SquareWave_OFF  = 0x1C # Not output square wave, enter interrupt mode
          @n          SquareWave_1Hz  = 0x00 # 1Hz square wave
          @n          SquareWave_1kHz = 0x08 # 1kHz square wave
          @n          SquareWave_4kHz = 0x10 # 4kHz square wave
          @n          SquareWave_8kHz = 0x18 # 8kHz square wave
        '''
        ctrl = self._read_reg(self._REG_CONTROL)
        ctrl &= 0x04
        ctrl &= 0x18
        ctrl |= mode
        
        self._write_reg(self._REG_CONTROL, ctrl)

    def day_of_week(self):
        '''!
          @brief Convert days to weeks
          @return day of week
        '''
        day = self.date2days(self._y, self._m, self._d)
        return (day + 6) % 7

    def get_day_of_week(self):
        '''!
          @brief get day of week
          @return day of week
        '''
        return self.day_of_the_week[self.day_of_week()]

    def set_hour_system(self, mode = H24hours):
        '''!
          @brief Set mode of time
          @param mode H24hours, H12hours
        '''
        self._mode = mode

    def set_time(self, year, month, date, hour, minute, second):
        '''!
          @brief Set time into rtc and take effect immediately
          @param year 1900~2100
          @param month 1~12
          @param date 1~31
          @param hour 0~23
          @param minute 0~59
          @param second 0~59
        '''
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
                self.hh = 0x52
            elif hour >0 and hour < 12:
                self.hh = (0x40|self.bin2bcd(hour))
            elif hour == 12:
                self.hh = 0x72
            elif hour >12 and hour < 24:
                self.hh = (0x60|self.bin2bcd(hour - 12))
        self.mm = self.bin2bcd(minute)
        self.ss = self.bin2bcd(second)
        #data = [self.ss, self.mm, self.hh, self.day_of_week(), self.d, self.m, self.y]
        self._write_reg(self._REG_RTC_SEC, self.ss)
        self._write_reg(self._REG_RTC_MIN, self.mm)
        self._write_reg(self._REG_RTC_HOUR, self.hh)
        self._write_reg(self._REG_RTC_DATE, self.d)
        self._write_reg(self._REG_RTC_MONTH, self.m)
        self._write_reg(self._REG_RTC_YEAR, self.y)
        statreg = self._read_reg(self._REG_STATUS)
        statreg &= ~0x80
        self._write_reg(self._REG_STATUS, statreg)

    def get_AM_or_PM(self):
        '''!
          @brief output AM or PM of time 
          @return AM or PM, 24 hours mode return empty string
        '''
        buffer = self._read_reg(self._REG_RTC_HOUR)
        buffer = buffer & 0x60
        if (buffer >> 5) <= 1:
            return " "
        elif (buffer >> 5) == 2:
            return "AM"
        elif (buffer >> 5) == 3:
            return "PM"

    def get_year(self):
        '''!
          @brief get year of now time
          @return year
        '''
        year = self._read_reg(self._REG_RTC_YEAR)
        self._y = self.bcd2bin(year) + 1900
        century = self._read_reg(self._REG_RTC_MONTH)
        if century > 80:
            self._y += 100
        return self._y

    def get_month(self):
        '''!
          @brief get month of now time
          @return month
        '''
        month = self._read_reg(self._REG_RTC_MONTH)
        self._m = self.bcd2bin(month)
        if self._m > 80:
            self._m -= 80
        return self._m

    def get_date(self):
        '''!
          @brief get date of now time
          @return date
        '''
        date = self._read_reg(self._REG_RTC_DATE)
        self._d = self.bcd2bin(date)
        return self._d 

    def get_hour(self):
        '''!
          @brief get hour of now time
          @return hour
        '''
        hour = self._read_reg(self._REG_RTC_HOUR)
        if hour >> 6 == True:
            self._hh = self.bcd2bin(hour & 0x1F)
        else:
            self._hh = self.bcd2bin(hour & 0x3F)
        return self._hh

    def get_minute(self):
        '''!
          @brief get minute of now time
          @return minute
        '''
        minute = self._read_reg(self._REG_RTC_MIN)
        self._mm = self.bcd2bin(minute)
        return self._mm

    def get_second(self):
        '''!
          @brief get second of now time
          @return second
        '''
        second = self._read_reg(self._REG_RTC_SEC)
        self._ss = self.bcd2bin(second & 0x7F)
        return self._ss

    def get_temperature_C(self, mode=False):
        '''!
          @brief get temperature of sensor
          @param mode Select the sensor temperature measurement mode
          @n true Enable temperature conversion to measure and obtain the current temperature immediately
          @n false Disable temperature conversion and obtain the temperature measurement data within 64 seconds
          @n       (the sensor measures the temperature every 64 seconds by default)
          @return temperature
        '''
        if(mode):
            crtl = self._read_reg(self._REG_CONTROL)
            crtl |= 0x20
            self._write_reg(self._REG_CONTROL, crtl)

        buf1 = self._read_reg(self._REG_TEMPERATURE1)
        buf2 = self._read_reg(self._REG_TEMPERATURE2)
        return (buf1 + (buf2>>6)*0.25)

    def get_temperature_F(self, mode=False):
        '''!
          @brief get temperature of sensor
          @param mode Select the sensor temperature measurement mode
          @n true Enable temperature conversion to measure and obtain the current temperature immediately
          @n false Disable temperature conversion and obtain the temperature measurement data within 64 seconds
          @n       (the sensor measures the temperature every 64 seconds by default)
          @return temperature
        '''
        if(mode):
            crtl = self._read_reg(self._REG_CONTROL)
            crtl |= 0x20
            self._write_reg(self._REG_CONTROL, crtl)

        buf1 = self._read_reg(self._REG_TEMPERATURE1)
        buf2 = self._read_reg(self._REG_TEMPERATURE2)
        c = (buf1 + (buf2>>6)*0.25)
        return c * 9 / 5 + 32

    def is_lost_power(self):
        '''!
          @brief check if rtc has been lost power
          @return True means rtc has lost power before and need to 
          @n      reset time; False means rtc operates well
        '''
        status = self._read_reg(self._REG_STATUS)
        return status >> 7

    def set_alarm1(self, alarmType, date, hour, minute, second):
        '''!
          @brief Set alarm1 clock
          @param alarmType EverySecond, repeat in every second
          @n               SecondsMatch, repeat in every minute
          @n               SecondsMinutesMatch, repeat in every hour
          @n               SecondsMinutesHoursMatch, repeat in every day
          @n               SecondsMinutesHoursDateMatch, repeat in every month
          @n               SecondsMinutesHoursDayMatch, repeat in every week Alarm1
          @n               UnknownAlarm1
          @param date    1-31
          @param hour   0-23
          @param minute 0-59
          @param second 0-59
          @return None
        '''
        dates = self.bin2bcd(date)
        if self._mode == 0:
            hours = (self._mode << 5|self.bin2bcd(hour))
        else:
            if hour == 0:
                hours = 0x52
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
        self._write_reg(self._REG_ALM1_SEC, seconds)
        self._write_reg(self._REG_ALM1_MIN, minutes)
        self._write_reg(self._REG_ALM1_HOUR, hours)
        if alarmType == self.SecondsMinutesHoursDateMatch:
            self._write_reg(self._REG_ALM1_DAY, dates)
        else:
            self._write_reg(self._REG_ALM1_DAY, days)
        if alarmType < self.SecondsMinutesHoursDateMatch:
            buffer = self._read_reg(self._REG_ALM1_DAY)
            buffer |= 0x80
            self._write_reg(self._REG_ALM1_DAY, buffer)
        if alarmType < self.SecondsMinutesHoursMatch:
            buffer = self._read_reg(self._REG_ALM1_HOUR)
            buffer |= 0x80
            self._write_reg(self._REG_ALM1_HOUR, buffer)
        if alarmType < self.SecondsMinutesMatch:
            buffer = self._read_reg(self._REG_ALM1_MIN)
            buffer |= 0x80
            self._write_reg(self._REG_ALM1_MIN, buffer)
        if(alarmType == self.EverySecond):
            buffer = self._read_reg(self._REG_ALM1_SEC)
            buffer |= 0x80
            self._write_reg(self._REG_ALM1_SEC, buffer)
        if(alarmType == self.SecondsMinutesHoursDayMatch):
            buffer = self._read_reg(self._REG_ALM1_DAY)
            buffer |= 0x40
            self._write_reg(self._REG_ALM1_DAY, buffer)
        self.clear_alarm()
        return

    def set_alarm2(self, alarmType, date, hour, minute):
        '''!
          @brief Set alarm2 clock
          @param alarmType EveryMinute, repeat in every minute;
          @n               MinutesMatch, repeat in every hour;
          @n               MinutesHoursMatch, repeat in every day;
          @n               MinutesHoursDateMatch, repeat in every month;
          @n               MinutesHoursDayMatch, repeat in every week Alarm2;
          @n               UnknownAlarm2
          @param date    1-31
          @param hour   0-23
          @param minute 0-59
          @return None
        '''
        dates = self.bin2bcd(date)
        if self._mode == 0:
            hours = (self._mode << 5|self.bin2bcd(hour))
        else:
            if hour == 0:
                hours = 0x52
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
        self._write_reg(self._REG_ALM2_MIN, minutes)
        self._write_reg(self._REG_ALM2_HOUR, hours)
        if alarmType == self.MinutesHoursDateMatch:
            self._write_reg(self._REG_ALM2_DAY)
        elif alarmType == self.MinutesHoursDayMatch:
            days |= 0x80
            self._write_reg(self._REG_ALM2_DAY, days)
        if alarmType < self.MinutesHoursDateMatch:
            buffer = self._read_reg(self._REG_ALM2_DAY)
            buffer |= 0x80
            self._write_reg(self._REG_ALM2_DAY, buffer)
        if alarmType < self.MinutesHoursMatch:
            buffer = self._read_reg(self._REG_ALM2_HOUR)
            buffer |= 0x80
            self._write_reg(self._REG_ALM2_HOUR, buffer)
        if alarmType == self.EveryMinute:
            buffer = self._read_reg(self._REG_ALM2_MIN,)
            buffer |= 0x80
            self._write_reg(self._REG_ALM2_MIN, buffer)
        self.clear_alarm()
        return

    def enable_alarm1_int(self):
        '''!
          @brief enable the interrupt of alarm1
        '''
        conReg = self._read_reg(self._REG_CONTROL)
        conReg |= 0x01
        self._write_reg(self._REG_CONTROL, conReg)

    def disable_alarm1_int(self):
        '''!
          @brief disable the interrupt of alarm1
        '''
        conReg = self._read_reg(self._REG_CONTROL)
        conReg &= 0xFE
        self._write_reg(self._REG_CONTROL, conReg)

    def enable_alarm2_int(self):
        '''!
          @brief enable the interrupt of alarm2
        '''
        conReg = self._read_reg(self._REG_CONTROL)
        conReg |= 0x02
        self._write_reg(self._REG_CONTROL, conReg)

    def disable_alarm2_int(self):
        '''!
          @brief disable the interrupt of alarm2
        '''
        conReg = self._read_reg(self._REG_CONTROL)
        conReg &= 0xFD
        self._write_reg(self._REG_CONTROL, conReg)

    def is_alarm_trig(self):
        '''!
          @brief check if alarm flag has been triggered
          @return True, triggered; False, not triggered
        '''
        staReg = self._read_reg(self._REG_STATUS)
        return staReg&3

    def clear_alarm(self):
        '''!
          @brief clear alarm flag trigger
        '''
        staReg = self._read_reg(self._REG_STATUS)
        staReg &= 0xFC
        self._write_reg(self._REG_STATUS, staReg)

    def enable_32k(self):
        '''!
          @brief enable the 32k output
        '''
        staReg = self._read_reg(self._REG_STATUS)
        staReg|= 0x08
        self._write_reg(self._REG_STATUS, staReg)

    def disable_32k(self):
        '''!
          @brief disable the 32k output (default is enabled)
        '''
        staReg = self._read_reg(self._REG_STATUS)
        staReg &= 0xF7
        self._write_reg(self._REG_STATUS, staReg)

    def write_SRAM(self, addr, data):
        '''!
          @brief write data into the SRAM
          @param addr address of SRAM
          @param data Written data
        '''
        self._write_reg(addr, data)

    def read_SRAM(self, addr):
        '''!
          @brief read data of the SRAM
          @param addr address of SRAM
          @return Read the data
        '''
        return self._read_reg(addr)

    def clear_SRAM(self, addr):
        '''!
          @brief clear the SRAM
          @param addr address of SRAM
        '''
        self._write_reg(addr, 0x00)

    def _write_reg(self, reg, buff):
        '''!
          @brief writes data to a register
          @param reg register address
          @param buff written data
        '''
        self.i2cbus.write_byte_data(self.i2c_addr, reg, buff)
        

    def _read_reg(self, reg):
        '''!
          @brief read the data from the register
          @param reg register address
          @return Read the register data
        '''
        return self.i2cbus.read_byte_data(self.i2c_addr, reg) 

    def _scan(self):
        '''!
          @brief Scan IIC devices
          @return True means IIC communication succeeds, false means it fails.
        '''
        try:
            self.i2cbus.write_quick(self.i2c_addr)
            return True
        except:
            print("I2C init fail")
            return False
