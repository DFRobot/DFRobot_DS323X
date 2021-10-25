# DFRobot_DS323X
* [中文版](./README_CN.md)

DS323X is a low-cost, extremely accurate, I2C real-time clock(RTC) module. It can provide ±5%ppm accuracy (no more than 0.432s error) within the whole temperature range and lifespan, and is compatible with 2.5-5.5V wide voltage range. The current comsumption of the module is as low as 2uA when powered by batteries. This module can be used to measure ambient temperature with ±3℃ accuracy. and it has 25 times seismic resistance of the latter as well as better temperature stability.<br>

![产品实物图](./resources/images/DS3232.png)


## Product Link (https://www.dfrobot.com/search-DS323.html)
    SKU: DFR0641/DFR0819/DFR0821


## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)


## Summary

* Get real-time, the initial time need to be set by users, we can get compiled time, NTP time or set it by ourselves, the minimum unit: s <br>
* Set the alarm clock, users can set two alarm clocks, can get the falling edge pulse at the interrupt pin to trigger the alarm clock <br>
* Measure the chip temperature, error: ±0.5℃ <br>
* Read and write 236 bytes of SRAM inside DS3232 chip<br>


## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.


## Methods

```C++

    /**
     * @fn begin
     * @brief Init chip
     * @return True means IIC communication succeeds, false means it fails.
     */
    bool begin(void);

    /**
     * @fn getYear
     * @brief Get year of now time
     * @return Year, uint16_t
     */
    uint16_t getYear();

    /**
     * @fn getMonth
     * @brief Get month of now time
     * @return Month, uint8_t
     */
    uint8_t  getMonth();

    /**
     * @fn getDate
     * @brief Get date of now time
     * @return Date, uint8_t
     */
    uint8_t  getDate(); 

    /**
     * @fn getHour
     * @brief Get hour of now time
     * @return Hour, uint8_t
     */
    uint8_t  getHour();

    /**
     * @fn getMinute
     * @brief Get minute of now time
     * @return Minute, uint8_t
     */
    uint8_t  getMinute();

    /**
     * @fn getSecond
     * @brief Get second of now time
     * @return Second, uint8_t
     */
    uint8_t  getSecond();

    /**
     * @fn setHourSystem
     * @brief Set hour system of time
     * @param mode: e24hours, e12hours. default is e24hours
     * @return None
     */
    void setHourSystem(eHours_t mode);

    /**
     * @fn setTime
     * @brief Set time into rtc and take effect immediately
     * @param year 1900~2100
     * @param month 1~12
     * @param date 1~31
     * @param hour 0~23
     * @param minute 0~59
     * @param second 0~59
     * @return None
     */
    void setTime(uint16_t year, uint8_t month, uint8_t date, uint8_t hour, uint8_t minute, uint8_t second);

    /**
     * @fn  getDayOfWeek
     * @brief  get day of week
     * @return  day of week, String
     */
    String getDayOfWeek();

    /**
     * @fn getTemperatureC
     * @brief Get sensor temperature
     * @param mode Select the sensor temperature measurement mode
     * @n true Enable temperature conversion to measure and obtain the current temperature immediately
     * @n false Disable temperature conversion and obtain the temperature measurement data within 64 seconds
     * @n       (the sensor measures the temperature every 64 seconds by default)
     * @return temperature, unit:℃
     */
    float getTemperatureC(bool mode=false);

    /**
     * @fn isLostPower
     * @brief Judge if it is power-down 
     * @return true True means rtc has lost power before and needs to reset time;False means rtc operates well
     */
    bool isLostPower(void);

    /**
     * @fn readSqwPinMode
     * @brief Read the value of pin sqw
     * @return Explanation of the readings in enumeration variable eSqwPinMode_t
     */
    eSqwPinMode_t readSqwPinMode();

    /**
     * @fn writeSqwPinMode
     * @brief Set the vaule of pin sqw
     * @param mode Explanation of the witten value in enumeration variable eSqwPinMode_t
     * @return None
     */
    void writeSqwPinMode(eSqwPinMode_t mode);

    /**
     * @fn setAlarm1
     * @brief Set alarm1 clock 
     * @param alarmType Alarm working mode
     * @param days     (1-31)
     * @param hours    (0-23)
     * @param minutes  (0-59)
     * @param seconds  (0-59)
     * @return None
     */
    void setAlarm1(eAlarm1Types_t alarmType,int16_t days,int8_t hours,int8_t minutes,int8_t seconds);

    /**
     * @fn setAlarm2
     * @brief Set alarm2 clock 
     * @param alarmType Alarm working mode
     * @param days     (1-31)
     * @param hours    (0-23)
     * @param minutes  (0-59)
     * @return None
     */
    void setAlarm2(eAlarm2Types_t alarmType,int16_t days,int8_t hours,int8_t minutes);

    /**
     * @fn enableAlarm1Int
     * @brief enable or disable the interrupt of alarm1
     * @return None
     */
    void enableAlarm1Int();

    /**
     * @fn disableAlarm1Int
     * @brief enable or disable the interrupt of alarm1
     * @return None
     */
    void disableAlarm1Int();

    /**
     * @fn disableAlarm1Int
     * @brief enable or disable the interrupt of alarm2
     * @return None
     */
    void enableAlarm2Int();

    /**
     * @fn disableAlarm2Int
     * @brief enable or disable the interrupt of alarm2
     * @return None
     */
    void disableAlarm2Int();

    /**
     * @fn getAMorPM
     * @brief output AM or PM of time
     * @return AM or PM, 24 hours mode return empty string
     */
    String getAMorPM();

    /**
     * @fn isAlarmTrig
     * @brief Judge if the alarm clock is triggered
     * @return Explanation of the readings in enumeration variable eTrigger_t
     */
    eTrigger_t isAlarmTrig();

    /**
     * @fn clearAlarm
     * @brief Clear alarm flag
     * @return None
     */
    void clearAlarm();

    /**
     * @fn enable32k
     * @brief enable the 32k output 
     * @return None
     */
    void enable32k();

    /**
     * @fn disable32k
     * @brief disable the 32k output
     * @return None
     */
    void disable32k();

    /**
     * @fn writeSRAM
     * @brief write the SRAM
     * @param addr 0x14~0xFF
     * @param data uint8_t HEX
     * @return true means write is successful, false means write is failed
     */
    bool writeSRAM(uint8_t addr, uint8_t data);

    /**
     * @fn readSRAM
     * @brief readthe SRAM
     * @param addr 0x14~0xFF
     * @return data store in the SRAM
     */
    uint8_t readSRAM(uint8_t addr);

    /**
     * @fn clearSRAM
     * @brief clear the SRAM
     * @param addr 0x14~0xFF
     * @return true means clear is successful, false means clear is failed
     */
    bool clearSRAM(uint8_t addr);

```

## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | :----:
Arduino Uno        |      √       |              |             |
Arduino MEGA2560   |      √       |              |             |
Arduino Leonardo   |      √       |              |             |
FireBeetle-ESP8266 |      √       |              |             |
FireBeetle-ESP32   |      √       |              |             |
FireBeetle-M0      |      √       |              |             |
Micro:bit          |      √       |              |             |


## History

- 2021/10/25 - Version 1.0.0 released.


## Credits

Written by(yufeng.luo@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))

