/*!
 * @file DFRobot_DS323X.cpp
 * @brief Define the basic structure of class DFRobot_DS323X
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [yufeng](yufeng.luo@dfrobot.com)
 * @version  V1.0
 * @date  2021-2-23
 * @url https://github.com/DFRobot/DFRobot_DS323X
 */

#include <DFRobot_DS323X.h>
const uint8_t daysInMonth [] PROGMEM={31,28,31,30,31,30,31,31,30,31,30,31};

static uint16_t date2days(uint16_t y, uint8_t m, uint8_t d) {
    if (y >= 2000)
        y -= 2000;                              // Remove year offset
    uint16_t days = d;                          // Store numbers of days
    for (uint8_t i = 1; i < m; ++i){
        days += pgm_read_byte(daysInMonth + i - 1); // Add number of days for each month
    } if (m > 2 && y % 4 == 0)
        ++days;                                 // Deal with leap years
    return days + 365 * y + (y + 3) / 4 - 1;    // Return computed value
}

static uint8_t conv2d(const char* p) {
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9')
        v = *p - '0';
    return 10 * v + *++p - '0';
}

DFRobot_DS323X::~DFRobot_DS323X() {}

bool DFRobot_DS323X::begin(void)
{
    _pWire->begin();
    delay(100);
    _pWire->beginTransmission(_deviceAddr);
    if(_pWire->endTransmission() == 0){
        uint8_t buf[7] = {0,0,0,0,0,0,0};
        writeReg(DS323X_REG_ALM1_SEC, buf, 7);
        return true;
    }
    else{
        return false;
    }
}

/*!
 *@brief BCD code to BIN code
 *@param val Input BCD code
 *@return Return BIN code
 */
uint8_t DFRobot_DS323X::bcd2bin(uint8_t val){
    return val - 6 * (val >> 4);
}

/*!
 *@brief BIN code to BCD code
 *@param val Input BIN code
 *@return Return BCD code
 */
uint8_t DFRobot_DS323X::bin2bcd (uint8_t val){
    return val + 6 * (val / 10);
}

/*!
 *@brief Read the value of pin sqw
 *@return eSquareWave_OFF  = 0x1C // Not output square wave, enter interrupt mode 
 *@n      eSquareWave_1Hz  = 0x00 // 1Hz square wave
 *@n      eSquareWave_1kHz = 0x08 // 1kHz square wave
 *@n      eSquareWave_4kHz = 0x10 // 4kHz square wave
 *@n      eSquareWave_8kHz = 0x18 // 8kHz square wave
 */
DFRobot_DS323X::eSqwPinMode_t DFRobot_DS323X::readSqwPinMode(){
    uint8_t mode;
    readReg(DS323X_REG_CONTROL, &mode, 1);
    mode &= 0x1C;
    if (mode & 0x04){
        mode = eSquareWave_OFF;
    }
    return static_cast<eSqwPinMode_t>(mode);
}

/*!
 *@brief Set the vaule of pin sqw
 *@param mode eSquareWave_OFF  = 0x1C // Not output square wave, enter interrupt mode 
 *@n          eSquareWave_1Hz  = 0x00 // 1Hz square wave
 *@n          eSquareWave_1kHz = 0x08 // 1kHz square wave
 *@n          eSquareWave_4kHz = 0x10 // 4kHz square wave
 *@n          eSquareWave_8kHz = 0x18 // 8kHz square wave
 */
void DFRobot_DS323X::writeSqwPinMode(eSqwPinMode_t mode){
    uint8_t ctrl;
    readReg(DS323X_REG_CONTROL, &ctrl, 1);
    ctrl &= ~0x04;
    ctrl &= ~0x18;
    ctrl |= mode;
    writeReg(DS323X_REG_CONTROL, &ctrl, 1);
}

/*!
 *@brief Set the last compiled time as the current time
 *@param comSec Compensation time，the value obtained by subtracting the PC system time from the serial port printing time after the first upload, unit：s
 */
void DFRobot_DS323X::getCompileTime (uint8_t comTime){
    char buff[11];
    uint8_t min = 0;
    memcpy_P(buff, F(__DATE__), 11);
    y = conv2d(buff + 9);
    // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
    switch (buff[0]) {
        case 'J': m = (buff[1] == 'a') ? 1 : ((buff[2] == 'n') ? 6 : 7); break;
        case 'F': m = 2; break;
        case 'A': m = buff[2] == 'r' ? 4 : 8; break;
        case 'M': m = buff[2] == 'r' ? 3 : 5; break;
        case 'S': m = 9; break;
        case 'O': m = 10; break;
        case 'N': m = 11; break;
        case 'D': m = 12; break;
    }
    m += 80;
    d = conv2d(buff + 4);
    memcpy_P(buff, F(__TIME__), 8);
    hh = bin2bcd(conv2d(buff));
    ss = conv2d(buff + 6) + comTime;
    while (ss >60){
        ss -= 60;
        min += 1;
    }
    mm = conv2d(buff + 3) + min;
    uint8_t buffer[] = {bin2bcd(ss),bin2bcd(mm),hh,dayOfWeek(),bin2bcd(d),bin2bcd(m),bin2bcd(y)};
    writeReg(DS323X_REG_RTC_SEC, buffer, 7);
    uint8_t statreg;
    readReg(DS323X_REG_STATUS, &statreg, 1);
    statreg &= ~0x80; // flip OSF bit
    writeReg(DS323X_REG_STATUS, &statreg, 1);
}

uint8_t DFRobot_DS323X::dayOfWeek() const {
  uint16_t day = date2days(_y, _m, _d); // compute the number of days
  return (day + 6) % 7;                 // Jan 1, 2000 is a Saturday
} 

/*!
 *@brief get day of week
 *@return day of week
 */
String DFRobot_DS323X::getDayOfWeek(){
    return daysOfTheWeek[dayOfWeek()];
}

/*!
 *@brief Set time into rtc and take effect immediately
 *@param year, 1900~2100
 *@param month, 1~12
 *@param date, 1~31
 *@param hour: 0~23
 *@param minute, 0~59
 *@param second, 0~59
 */
void DFRobot_DS323X::setTime(uint16_t year, uint8_t month, uint8_t date, uint8_t hour, uint8_t minute, uint8_t second){
    if (year >=2000){
        y = year - 2000;
        m = month + 80;
    }
    else{
        y = year - 1900;
        m = month;
    }
    d = date;
    if (_mode == 0 ){
        hh = (_mode << 5|bin2bcd(hour));
    }else{
        if (hour == 0){
            hh = 0x52;
        }else if (hour >0 && hour < 12){
            hh = (0x40|bin2bcd(hour));
        }else if (hour == 12){
            hh = 0x72;
        }else if (hour >12 && hour < 24){
            hh = (0x60|bin2bcd(hour - 12));
        }
    }
    mm = minute;
    ss = second;
    uint8_t buffer[] = {bin2bcd(ss),bin2bcd(mm),hh,dayOfWeek(),bin2bcd(d),bin2bcd(m),bin2bcd(y)};
    writeReg(DS323X_REG_RTC_SEC, buffer, 7);
    uint8_t statreg;
    readReg(DS323X_REG_STATUS, &statreg, 1);
    statreg &= ~0x80; // flip OSF bit
    writeReg(DS323X_REG_STATUS, &statreg, 1);
}

/*!
 *@brief output AM or PM of time 
 *@return AM or PM, 24 hours mode return empty string 
 */
String DFRobot_DS323X::getAMorPM(){
    uint8_t buffer;
    readReg(DS323X_REG_RTC_HOUR, &buffer, 1);
    buffer = buffer << 1;
    buffer = buffer >> 6;
    switch (buffer) {
        case 0: return ""; break;
        case 1: return ""; break;
        case 2: return "AM"; break;
        case 3: return "PM"; break;
    }
}

/*!
 *@brief Get year of now time
 *@return Year
 */
uint16_t DFRobot_DS323X::getYear(){
    uint8_t buf[2];
    readReg(DS323X_REG_RTC_MONTH, buf, 2);
    _y = bcd2bin(buf[1]) + 1900;
    if(buf[0] > 80)
        _y += 100;
    return _y;
}

/*!
 *@brief Get month of now time
 *@return Month
 */
uint8_t DFRobot_DS323X::getMonth(){
    readReg(DS323X_REG_RTC_MONTH, &_m, 1);
    _m = bcd2bin(_m);
    if(_m > 80)
        _m -= 80;
    return _m;
}

/*!
 *@brief Get date of now time
 *@return Date
 */
uint8_t DFRobot_DS323X::getDate(){
    readReg(DS323X_REG_RTC_DATE, &_d, 1);
    _d = bcd2bin(_d);
    return _d;
}

/*!
 *@brief Get hour of now time
 *@return Hour
 */
uint8_t DFRobot_DS323X::getHour(){
    readReg(DS323X_REG_RTC_HOUR, &_hh, 1);
    if(_hh&0x40){
        _hh = _hh << 3;
        _hh = bcd2bin(_hh >> 3);
    }
    else{
        _hh = _hh << 2;
        _hh = bcd2bin(_hh >> 2);
    }
    return _hh;
}

/*!
 *@brief Get minute of now time
 *@return Minute
 */
uint8_t DFRobot_DS323X::getMinute(){
    readReg(DS323X_REG_RTC_MIN, &_mm, 1);
    _mm = bcd2bin(_mm);
    return _mm;
}

/*!
 *@brief Get second of now time
 *@return Second
 */
uint8_t DFRobot_DS323X::getSecond(){
    readReg(DS323X_REG_RTC_SEC, &_ss, 1);
    _ss = bcd2bin(_ss);
    return _ss;
}

/*!
 *@brief Get current temperature, 
 *@return temperature, unit:℃
 */
float DFRobot_DS323X::getTemperatureC(){
    uint8_t buf[2];
    readReg(DS323X_REG_TEMPERATURE, buf, 2);
    return (buf[0] + (buf[1]>>6)*0.25f);
}

/*!
 *@brief Judge if it is power-down 
 *@return If return true, power down, time needs to reset; false, work well. 
 */
bool DFRobot_DS323X::isLostPower(void) {
    uint8_t status;
    readReg(DS323X_REG_STATUS, &status, 1);
    return status >> 7;
}

/*!
 *@brief Set alarm clock
 *@param alarmType Alarm clock working mode typedef enum{
 *@n                                  eEverySecond,
 *@n                                  eSecondsMatch,
 *@n                                  eSecondsMinutesMatch,
 *@n                                  eSecondsMinutesHoursMatch,
 *@n                                  eSecondsMinutesHoursDateMatch,
 *@n                                  eSecondsMinutesHoursDayMatch, //Alarm1
 *@n                                  eUnknownAlarm1
 *@n                                  }eAlarm1Types_t;
 *@param days    1-31
 *@param hours   0-23
 *@param minutes 0-59
 *@param seconds 0-59
 */
void DFRobot_DS323X::setAlarm1(eAlarm1Types_t alarmType, int16_t date,int8_t hour,
                               int8_t minute,int8_t second){
    int16_t dates = bin2bcd(date);
    int8_t hours;
    if (_mode == 0 ){
        hours = (_mode << 5|bin2bcd(hour));
    }else{
        if (hour == 0){
            hours = 0x52;
        }else if (hour >0 && hour < 12){
            hours = (0x40|bin2bcd(hour));
        }else if (hour == 12){
            hours = 0x72;
        }else if (hour >12 && hour < 24){
            hours = (0x60|bin2bcd(hour - 12));
        }
    }
    int8_t minutes = bin2bcd(minute);
    int8_t seconds = bin2bcd(second);
    uint8_t days = bin2bcd(dayOfWeek());
    uint8_t buffer;
    if (alarmType >= eUnknownAlarm1)
        return;
    writeReg(DS323X_REG_ALM1_SEC, &seconds, 1);
    writeReg(DS323X_REG_ALM1_MIN, &minutes, 1);
    writeReg(DS323X_REG_ALM1_HOUR, &hours, 1);
    if (alarmType == eSecondsMinutesHoursDateMatch)
        writeReg(DS323X_REG_ALM1_DAY, &dates, 1);
    else
        writeReg(DS323X_REG_ALM1_DAY, &days, 1);
    if(alarmType<eSecondsMinutesHoursDateMatch){
        readReg(DS323X_REG_ALM1_DAY, &buffer, 1);
        buffer |= 0x80;
        writeReg(DS323X_REG_ALM1_DAY, &buffer, 1);
    }
    if(alarmType<eSecondsMinutesHoursMatch){
        readReg(DS323X_REG_ALM1_HOUR, &buffer, 1);
        buffer |= 0x80;
        writeReg(DS323X_REG_ALM1_HOUR, &buffer, 1);
    }
    if(alarmType<eSecondsMinutesMatch){
        readReg(DS323X_REG_ALM1_MIN, &buffer, 1);
        buffer |= 0x80;
        writeReg(DS323X_REG_ALM1_MIN, &buffer, 1);
    }
    if(alarmType==eEverySecond){
        readReg(DS323X_REG_ALM1_SEC, &buffer, 1);
        buffer |= 0x80;
        writeReg(DS323X_REG_ALM1_SEC, &buffer, 1);
    }
    if(alarmType==eSecondsMinutesHoursDayMatch){
        readReg(DS323X_REG_ALM1_DAY, &buffer, 1);
        buffer |= 0x40;
        writeReg(DS323X_REG_ALM1_DAY, &buffer, 1);
    }
    clearAlarm(); // Clear the alarm state
    return;
}

/*!
 *@brief Set alarm clock
 *@param alarmType Alarm clock working mode typedef enum{
 *@n                                  eEveryMinute,
 *@n                                  eMinutesMatch,
 *@n                                  eMinutesHoursMatch,
 *@n                                  eMinutesHoursDateMatch,
 *@n                                  eMinutesHoursDayMatch,        //Alarm2
 *@n                                  eUnknownAlarm2
 *@n                                  }eAlarm2Types_t;
 *@param days    1-31
 *@param hours   0-23
 *@param minutes 0-59
 */
void DFRobot_DS323X::setAlarm2(eAlarm2Types_t alarmType, int16_t date,int8_t hour,
                               int8_t minute){
    int16_t dates = bin2bcd(date);
    int8_t hours;
    if (_mode == 0 ){
        hours = (_mode << 5|bin2bcd(hour));
    }else{
        if (hour == 0){
            hours = 0x52;
        }else if (hour >0 && hour < 12){
            hours = (0x40|bin2bcd(hour));
        }else if (hour == 12){
            hours = 0x72;
        }else if (hour >12 && hour < 24){
            hours = (0x60|bin2bcd(hour - 12));
        }
    }
    int8_t minutes = bin2bcd(minute);
    uint8_t days = bin2bcd(dayOfWeek());
    uint8_t buffer;
    if (alarmType >= eUnknownAlarm2)
        return;
    writeReg(DS323X_REG_ALM2_MIN, &minutes, 1);
    writeReg(DS323X_REG_ALM2_HOUR, &hours, 1);
    if(alarmType == eMinutesHoursDateMatch)
        writeReg(DS323X_REG_ALM2_DAY, &dates, 1);
    else if (alarmType == eMinutesHoursDayMatch){
        days |= 0x80;
        writeReg(DS323X_REG_ALM2_DAY, &days, 1);
    }
    if(alarmType < eMinutesHoursDateMatch){
        readReg(DS323X_REG_ALM2_DAY, &buffer, 1);
        buffer |= 0x80;
        writeReg(DS323X_REG_ALM2_DAY, &buffer, 1);
    }
    if(alarmType < eMinutesHoursMatch){
        readReg(DS323X_REG_ALM2_HOUR, &buffer, 1);
        buffer |= 0x80;
        writeReg(DS323X_REG_ALM2_HOUR, &buffer, 1);
    }
    if(alarmType == eEveryMinute){
        readReg(DS323X_REG_ALM2_MIN, &buffer, 1);
        buffer |= 0x80;
        writeReg(DS323X_REG_ALM2_MIN, &buffer, 1);
    }
    clearAlarm(); // Clear the alarm state
    return;
}

/*!
 *@brief enable the interrupt of alarm1 
 */
void DFRobot_DS323X::enableAlarm1Int(){
    uint8_t crtl;
    readReg(DS323X_REG_CONTROL, &crtl, 1);
    crtl |= 0x01;
    writeReg(DS323X_REG_CONTROL, &crtl, 1);
}

/*!
 *@brief disable the interrupt of alarm1 
 */
void DFRobot_DS323X::disableAlarm1Int(){
    uint8_t crtl;
    readReg(DS323X_REG_CONTROL, &crtl, 1);
    crtl &= 0xFE;
    writeReg(DS323X_REG_CONTROL, &crtl, 1);
}

/*!
 *@brief enable the interrupt of alarm2 
 */
void DFRobot_DS323X::enableAlarm2Int(){
    uint8_t crtl;
    readReg(DS323X_REG_CONTROL, &crtl, 1);
    crtl |= 0x02;
    writeReg(DS323X_REG_CONTROL, &crtl, 1);
}

/*!
 *@brief disable the interrupt of alarm2 
 */
void DFRobot_DS323X::disableAlarm2Int(){
    uint8_t crtl;
    readReg(DS323X_REG_CONTROL, &crtl, 1);
    crtl &= 0xFD;
    writeReg(DS323X_REG_CONTROL, &crtl, 1);
}

/*!
 *@brief Judge if the alarm clock is triggered
 *@return eNoTrigger          // No alarm is triggered
 *@n      eAlarm1Trigger      // Alarm1 is triggered
 *@n      eAlarm2Trigger      // Alarm2 is triggered
 *@n      eAllTrigger         // All alarms are triggered
 */
DFRobot_DS323X::eTrigger_t DFRobot_DS323X::isAlarmTrig() {
    uint8_t status;
    readReg(DS323X_REG_STATUS, &status, 1);
    return static_cast<eTrigger_t>(status&3);
}

/*!
 *@brief Clear alarm flag 
 */
void DFRobot_DS323X::clearAlarm(){
    uint8_t status;
    readReg(DS323X_REG_STATUS, &status, 1);
    status &= 0xFC;
    writeReg(DS323X_REG_STATUS, &status, 1);
} 

/*!
 *@brief enable the 32k output 
 */
void DFRobot_DS323X::enable32k(){
    uint8_t status;
    readReg(DS323X_REG_STATUS, &status, 1);
    status |= 0x08;
    writeReg(DS323X_REG_STATUS, &status, 1);
}

/*!
 *@brief disable the 32k output 
 */
void DFRobot_DS323X::disable32k(){
    uint8_t status;
    readReg(DS323X_REG_STATUS, &status, 1);
    status &= 0xF7;
    writeReg(DS323X_REG_STATUS, &status, 1);
}

/*!
 *@brief write data into the SRAM
 *@param addr 0x14~0xFF
 *@param data uint8_t HEX
 *@return true means write is successful, false means write is failed
 */
bool DFRobot_DS323X::writeSRAM(uint8_t reg, uint8_t data){
    return writeReg(reg, &data, 1);
}

/*!
 *@brief read  the SRAM
 *@param addr 0x14~0xFF
 *@return data store in the SRAM
 */
uint8_t DFRobot_DS323X::readSRAM(uint8_t reg){
    uint8_t buf;
    readReg(reg, &buf, 1);
    return buf;
}

/*!
 *@brief clear the SRAM
 *@param addr 0x14~0xFF
 *@return true means write is successful, false means write is failed
 */
bool DFRobot_DS323X::clearSRAM(uint8_t reg){
    uint8_t buf = 0x00;
    return writeReg(reg, &buf, 1);
}

bool DFRobot_DS323X::writeReg(uint8_t reg, const void* pBuf, size_t size)
{
    if(pBuf == NULL){
        DBG("pBuf ERROR!! : null pointer");
    }
    uint8_t * _pBuf = (uint8_t *)pBuf;
    _pWire->beginTransmission(_deviceAddr);
    _pWire->write(&reg, 1);
    
    for(uint16_t i = 0; i < size; i++){
        _pWire->write(_pBuf[i]);
    }
    if( _pWire->endTransmission() != 0){
        return 0;
    }else{
        return 1;
    }
}

uint8_t DFRobot_DS323X::readReg(uint8_t reg, const void* pBuf, size_t size)
{
    if(pBuf == NULL){
        DBG("pBuf ERROR!! : null pointer");
    }
    uint8_t * _pBuf = (uint8_t *)pBuf;
    _pWire->beginTransmission(_deviceAddr);
    _pWire->write(&reg, 1);
    
    if( _pWire->endTransmission() != 0){
        return 0;
    }

    _pWire->requestFrom(_deviceAddr, (uint8_t) size);
    for(uint16_t i = 0; i < size; i++){
        _pBuf[i] = _pWire->read();
    }
    return size;
}
