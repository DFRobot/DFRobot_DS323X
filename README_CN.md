# DFRobot_DS323X
* [English Version](./README.md)

DS323X是一款低成本、极其精确的I2C实时时钟(RTC)模块。在整个温度范围和寿命范围内，可提供±5%ppm的精度(误差不超过0.432s)，并兼容2.5-5.5V宽电压范围。电池供电时，该模块的电流消耗低至2uA。该模块可用于测量环境温度，精度为±3℃。抗震性能是后者的25倍，温度稳定性更好。

![产品实物图](./resources/images/DS3232.png)


## 产品链接 (https://www.dfrobot.com.cn/search.php?keywords=DS323)
    SKU: DFR0641/DFR0819/DFR0821


## 目录

* [概述](#概述)
* [库安装](#库安装)
* [方法](#方法)
* [兼容性](#兼容性)
* [历史](#历史)
* [创作者](#创作者)


## 概述

*获取实时，初始时间需要用户设定，我们可以获取编译时间，NTP时间或自己设定，最小单位:s 
*设置闹钟，用户可以设置两个闹钟，可以在中断引脚处得到下降沿脉冲触发闹钟
*测量芯片温度，误差:±0.5℃
*在DS3232芯片中有可读写236字节的SRAM


## 库安装

要使用这个库，首先下载库文件，将其粘贴到\Arduino\libraries目录中，然后打开示例文件夹并在文件夹中运行演示。


## 方法

```C++

    /**
     * @fn begin
     * @brief 初始化芯片
     * @return True表示IIC沟通成功，false表示失败。
     */
    bool begin(void);

    /**
     * @fn getYear
     * @brief 获取当前时间的年份
     * @return 年, uint16_t
     */
    uint16_t getYear();

    /**
     * @fn getMonth
     * @brief 获取当前时间的月份
     * @return 月, uint8_t
     */
    uint8_t  getMonth();

    /**
     * @fn getDate
     * @brief 获取当前时间的日期
     * @return 日期, uint8_t
     */
    uint8_t  getDate(); 

    /**
     * @fn getHour
     * @brief 获取当前时间的时
     * @return 小时, uint8_t
     */
    uint8_t  getHour();

    /**
     * @fn getMinute
     * @brief 获取当前时间的分
     * @return 分, uint8_t
     */
    uint8_t  getMinute();

    /**
     * @fn getSecond
     * @brief 获取当前时间的秒
     * @return 秒, uint8_t
     */
    uint8_t  getSecond();

    /**
     * @fn setHourSystem
     * @brief 设置小时时间的输出模式
     * @param mode: 24小时制, 12小时制; 默认24小时制
     * @return 无
     */
    void setHourSystem(eHours_t mode);

    /**
     * @fn setTime
     * @brief 设置rtc时间并立即生效
     * @param year 1900~2100
     * @param month 1~12
     * @param date 1~31
     * @param hour 0~23
     * @param minute 0~59
     * @param second 0~59
     * @return 无
     */
    void setTime(uint16_t year, uint8_t month, uint8_t date, uint8_t hour, uint8_t minute, uint8_t second);

    /**
     * @fn  getDayOfWeek
     * @brief  获取当前时间是星期几
     * @return  星期几, String
     */
    String getDayOfWeek();

    /**
     * @fn getTemperatureC
     * @brief 获取传感器测量温度
     * @param mode 择传感器温度测量方式
     * @n true 立即强制开始温度转换，测量和获得当前的温度立即
     * @n false 关闭强制温度转换，获取64秒内的温度测量数据(传感器默认每64秒转换/测量一次温度)
     * @return 温度测量值, 单位:℃
     */
    float getTemperatureC(bool mode=false);

    /**
     * @fn isLostPower
     * @brief 判断它是否断电
     * @return true True 表示rtc之前已经断电，需要复位时间; False 表示rtc运行良好
     */
    bool isLostPower(void);

    /**
     * @fn readSqwPinMode
     * @brief 读取引脚sqw的值
     * @return 枚举变量eSqwPinMode_t的读数说明
     */
    eSqwPinMode_t readSqwPinMode();

    /**
     * @fn writeSqwPinMode
     * @brief 设置sqw引脚的模式
     * @param mode 枚举变量eSqwPinMode_t中witten值的解释
     * @return 无
     */
    void writeSqwPinMode(eSqwPinMode_t mode);

    /**
     * @fn setAlarm1
     * @brief 设置闹钟1
     * @param alarmType 闹钟工作模式
     * @param days     (1-31)
     * @param hours    (0-23)
     * @param minutes  (0-59)
     * @param seconds  (0-59)
     * @return 无
     */
    void setAlarm1(eAlarm1Types_t alarmType,int16_t days,int8_t hours,int8_t minutes,int8_t seconds);

    /**
     * @fn setAlarm2
     * @brief 设置闹钟2
     * @param alarmType 闹钟工作模式
     * @param days     (1-31)
     * @param hours    (0-23)
     * @param minutes  (0-59)
     * @return 无
     */
    void setAlarm2(eAlarm2Types_t alarmType,int16_t days,int8_t hours,int8_t minutes);

    /**
     * @fn enableAlarm1Int
     * @brief 启用闹钟1的中断
     * @return 无
     */
    void enableAlarm1Int();

    /**
     * @fn disableAlarm1Int
     * @brief 禁用闹钟1的中断
     * @return 无
     */
    void disableAlarm1Int();

    /**
     * @fn disableAlarm1Int
     * @brief 启用闹钟2的中断
     * @return 无
     */
    void enableAlarm2Int();

    /**
     * @fn disableAlarm2Int
     * @brief 禁用闹钟2的中断
     * @return 无
     */
    void disableAlarm2Int();

    /**
     * @fn getAMorPM
     * @brief 输出上午或下午的时间
     * @return 上午或下午的时间，24小时模式返回空字符串
     */
    String getAMorPM();

    /**
     * @fn isAlarmTrig
     * @brief 判断闹钟是否被触发
     * @return 枚举变量eTrigger_t
     */
    eTrigger_t isAlarmTrig();

    /**
     * @fn clearAlarm
     * @brief 清除的闹钟标志
     * @return 无
     */
    void clearAlarm();

    /**
     * @fn enable32k
     * @brief 开启32k频率输出
     * @return 无
     */
    void enable32k();

    /**
     * @fn disable32k
     * @brief 关闭32k输出
     * @return 无
     */
    void disable32k();

    /**
     * @fn writeSRAM
     * @brief 写 SRAM
     * @param addr 0x14~0xFF
     * @param data 写数据
     * @return true 意味着写SRAM是成功的, false 意味着写SRAM是失败的
     */
    bool writeSRAM(uint8_t addr, uint8_t data);

    /**
     * @fn readSRAM
     * @brief 读 SRAM
     * @param addr 0x14~0xFF
     * @return 存储在SRAM中的数据
     */
    uint8_t readSRAM(uint8_t addr);

    /**
     * @fn clearSRAM
     * @brief 清除SRAM
     * @param addr 0x14~0xFF
     * @return true 意味着清除SRAM是成功的, false 意味着清除SRAM是失败的
     */
    bool clearSRAM(uint8_t addr);

```


## 兼容性

主控               |  正常运行    |   运行失败    |   未测试    | 备注
------------------ | :----------: | :----------: | :---------: | :---:
Arduino Uno        |      √       |              |             |
Arduino MEGA2560   |      √       |              |             |
Arduino Leonardo   |      √       |              |             |
FireBeetle-ESP8266 |      √       |              |             |
FireBeetle-ESP32   |      √       |              |             |
FireBeetle-M0      |      √       |              |             |
Micro:bit          |      √       |              |             |


## 历史

- 2021/10/25 - 1.0.0 版本


## 创作者

Written by(yufeng.luo@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))

