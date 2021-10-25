#-*- coding: utf-8 -*-
'''!
  @file  set_sqw_and_32k.py
  @brief  Set the mode of sqw pin and 32k output pin
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
from DFRobot_DS323X import *

rtc = DFRobot_DS323X(bus=1)

#begin return True if succeed, otherwise return False
while not rtc.begin():
    time.sleep(2)

'''
@brief Set the vaule of pin sqw
@param mode SquareWave_OFF  = 0x1C # Not output square wave, enter interrupt mode
@n          SquareWave_1Hz  = 0x00 # 1Hz square wave
@n          SquareWave_1kHz = 0x08 # 1kHz square wave
@n          SquareWave_4kHz = 0x10 # 4kHz square wave
@n          SquareWave_8kHz = 0x18 # 8kHz square wave
'''
rtc.write_sqw_pin_mode(rtc.SquareWave_1Hz)
'''
@brief Read the value of pin sqw
@return mode SquareWave_OFF  = 0x1C # Not output square wave, enter interrupt mode
@n           SquareWave_1Hz  = 0x00 # 1Hz square wave
@n           SquareWave_1kHz = 0x08 # 1kHz square wave
@n           SquareWave_4kHz = 0x10 # 4kHz square wave
@n           SquareWave_8kHz = 0x18 # 8kHz square wave
'''
print(rtc.read_sqw_pin_mode())

#rtc.disable_32k();#disable the 32k output (default is enable)

rtc.enable_32k();#enable the 32k output 


