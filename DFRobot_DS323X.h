/*!
 * @file  DFRobot_DS323X.h
 * @brief  Define the basic structure of class DFRobot_DS323X
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license  The MIT License (MIT)
 * @author  [yufeng](yufeng.luo@dfrobot.com)
 * @maintainer  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2021-10-25
 * @url  https://github.com/DFRobot/DFRobot_DS323X
 */
#ifndef _DFRobot_DS323X_H
#define _DFRobot_DS323X_H

#include "Arduino.h"
#include <Wire.h>

// #define ENABLE_DBG   //!< Enable this macro to see the detailed running process of the program 
#ifdef ENABLE_DBG
  #define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
  #define DBG(...)
#endif

#define DS323X_IIC_ADDRESS          0x68   ///< I2C ADDRESS

#define DS323X_REG_RTC_SEC          0x00   ///< RTC Seconds Register
#define DS323X_REG_RTC_MIN          0x01   ///< RTC Minutes Register
#define DS323X_REG_RTC_HOUR         0x02   ///< RTC Hours Register
#define DS323X_REG_RTC_DAY          0x03   ///< RTC Day Register
#define DS323X_REG_RTC_DATE         0x04   ///< RTC Date Register
#define DS323X_REG_RTC_MONTH        0x05   ///< RTC Month/Century Register
#define DS323X_REG_RTC_YEAR         0x06   ///< RTC Year Register
#define DS323X_REG_ALM1_SEC         0x07   ///< Alarm 1 Seconds Register
#define DS323X_REG_ALM1_MIN         0x08   ///< Alarm 1 Minutes Register
#define DS323X_REG_ALM1_HOUR        0x09   ///< Alarm 1 Hours Register
#define DS323X_REG_ALM1_DAY         0x0A   ///< Alarm 1 Day Register
#define DS323X_REG_ALM2_MIN         0x0B   ///< Alarm 2 Minutes Register
#define DS323X_REG_ALM2_HOUR        0x0C   ///< Alarm 2 Hours Register
#define DS323X_REG_ALM2_DAY         0x0D   ///< Alarm 2 Day Register
#define DS323X_REG_CONTROL          0x0E   ///< Control Register
#define DS323X_REG_STATUS           0x0F   ///< Status Register
#define DS323X_REG_AGE_OFFSET       0x10   ///< Aging Offset Register
#define DS323X_REG_TEMPERATURE      0x11   ///< temperature Register


class DFRobot_DS323X
{
public:

    /**
     * @enum  eSqwPinMode_t
     * @brief  Set the output square wave mode
     */
    typedef enum{
        eSquareWave_OFF  = 0x1C,   /**< Not output square wave, enter interrupt mode */
        eSquareWave_1Hz  = 0x00,   /**< 1Hz square wave */
        eSquareWave_1kHz = 0x08,   /**< 1kHz square wave */
        eSquareWave_4kHz = 0x10,   /**< 4kHz square wave */
        eSquareWave_8kHz = 0x18    /**< 8kHz square wave */
    }eSqwPinMode_t;

    /**
     * @enum  eHours_t
     * @brief  e24hours, e12hours
     */
    typedef enum{
        e24hours = 0,
        e12hours = 1
    }eHours_t;

    /**
     * @enum  eTrigger_t
     * @brief  Alarm Trigger Flag
     */
    typedef enum{
        eNoTrigger = 0,
        eAlarm1Trigger = 1,
        eAlarm2Trigger = 2,
        eAllTrigger = 3
    }eTrigger_t;

    /**
     * @enum  eAlarm1Types_t
     * @brief  Trigger mode of alarm 1
     */
    typedef enum{
        eEverySecond,                    /**< repeat in every second */
        eSecondsMatch,                   /**< repeat in every minute */
        eSecondsMinutesMatch,            /**< repeat in every hour */
        eSecondsMinutesHoursMatch,       /**< repeat in every day */
        eSecondsMinutesHoursDateMatch,   /**< repeat in every month */
        eSecondsMinutesHoursDayMatch,    /**< repeat in every week */
        //Alarm1
        eUnknownAlarm1
    }eAlarm1Types_t;

    /**
     * @enum  eAlarm2Types_t
     * @brief  Trigger mode of alarm 2
     */
    typedef enum{
        eEveryMinute,             /**< repeat in every minute */
        eMinutesMatch,            /**< repeat in every hour */
        eMinutesHoursMatch,       /**< repeat in every day */
        eMinutesHoursDateMatch,   /**< repeat in every month */
        eMinutesHoursDayMatch,    /**< repeat in every week */
        //Alarm2
        eUnknownAlarm2
    }eAlarm2Types_t;

    /**
     * @fn DFRobot_DS323X
     * @brief Constructor
     * @param pWire Wire address
     * @return None
     */
    DFRobot_DS323X(TwoWire *pWire = &Wire){_pWire = pWire;};

    /**
     * @fn ~DFRobot_DS323X
     * @brief Destructor
     * @return None
     */
    ~DFRobot_DS323X(){};

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
    void setHourSystem(eHours_t mode) { _mode = mode; };

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


protected:
    /**
     * @fn getCompileTime
     * @brief Set the last compiled time as the current time
     * @param comSec Compensation time, the value obtained by subtracting the PC system time from the serial port printing time after the first upload，unit: s.
     * @return None
     */
    void getCompileTime(uint8_t comSec = 0);

    /**
     * @fn writeReg
     * @brief write register values through IIC bus
     * @param reg  register address 8bits
     * @param pBuf to write data storage and buffer
     * @param size to write data length
     * @return true means write is successful, false means write is failed
     */
    virtual bool writeReg(uint8_t reg, const void* pBuf, size_t size);

    /**
   * @fn readReg
   * @brief read register values through IIC bus
   * @param reg  register address 8bits
   * @param pBuf to read data storage and buffer
   * @param size to read data length
   * @return return read length, returning 0 means read failure
     */
    virtual uint8_t readReg(uint8_t reg, const void* pBuf, size_t size);

    /**
     * @fn bcd2bin(uint8_t val)
     * @brief BCD code to BIN code
     * @param val Input BCD code
     * @return Return BIN code
     */
    uint8_t bcd2bin(uint8_t val);

    /**
     * @fn bin2bcd(uint8_t val)
     * @brief BIN code to BCD code
     * @param val Input BIN code
     * @return Return BCD code
     */
    uint8_t bin2bcd(uint8_t val);

    uint8_t y,    ///< Year Offset
            m,    ///< Months
            d,    ///< Days
            hh,   ///< Hours
            mm,   ///< Minutes
            ss;   ///< Seconds

private:
    /**
     * @fn dayOfWeek
     * @brief Convert days to weeks
     * @return day of week
     */
    uint8_t  dayOfWeek() const ;
    TwoWire *_pWire;
    uint8_t _deviceAddr = DS323X_IIC_ADDRESS;
    uint8_t rtc_bcd[7];
    uint8_t bcd[7];
    eHours_t _mode = e24hours;
    uint8_t  _ss,_mm,_hh,_d,_m;
    uint16_t _y;
    String daysOfTheWeek[7] = {"Sun", "Mon", "Tues", "Wed", "Thur", "Fri", "Sat"}; 
};

#endif
