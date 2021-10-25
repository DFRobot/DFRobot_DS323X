# DFRobot_DS323X
* [English Version](./README.md)

DS323X是一款低成本、极其精确的I2C实时时钟(RTC)模块。在整个温度范围和寿命范围内，可提供±5%ppm的精度(误差不超过0.432s)，并兼容2.5-5.5V宽电压范围。电池供电时，该模块的电流消耗低至2uA。该模块可用于测量环境温度，精度为±3℃。抗震性能是后者的25倍，温度稳定性更好。

![产品实物图](../../resources/images/DS3232.png)


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

本库使用到了modbus_tk, 使用本库前先检测树莓派是否成功导入modbus_tk, 若导入失败, 请通过以下命令安装modbus_tk库
python2: pip install modbus_tk
python3: pip3 install modbus_tk

使用库, 首先下载库文件, 将其粘贴到指定的目录中, 然后打开Examples文件夹并在该文件夹中运行演示。


## 方法

```python

  '''!
    @brief 初始化芯片
    @return True表示IIC沟通成功，false表示失败。
  '''
  def begin():

  '''!
    @brief 获取当前时间的年份
    @return 年
  '''
  def get_year():

  '''!
    @brief 获取当前时间的月份
    @return 月
  '''
  def get_month():

  '''!
    @brief 获取当前时间的日期
    @return 日期
  '''
  def get_date():

  '''!
    @brief 获取当前时间的时
    @return 小时
  '''
  def get_hour():

  '''!
    @brief 获取当前时间的分
    @return 分
  '''
  def get_minute():

  '''!
    @brief 获取当前时间的秒
    @return 秒
  '''
  def get_second():

  '''!
    @brief 获取当前时间是星期几
    @return 星期几
  '''
  def get_day_of_week():

  '''!
    @brief 设置小时时间的输出模式
    @param mode 24小时制, 12小时制; 默认24小时制
  '''
  def set_hour_system(self, mode = H24hours):

  '''!
    @brief 设置rtc时间并立即生效
    @param year, 1900~2100
    @param month, 1~12
    @param date, 1~31
    @param hour, 0-23
    @param minute, 0~59
    @param second, 0~59
  '''
  def set_time(year, month, date, hour, minute, second):

  '''!
    @brief 输出上午或下午的时间
    @return 上午或下午的时间，24小时模式返回空字符串
  '''
  def get_AM_or_PM():

  '''!
    @brief 获取传感器测量温度, 单位:℃
    @param mode 择传感器温度测量方式
    @n true 立即强制开始温度转换，测量和获得当前的温度立即
    @n false 关闭强制温度转换，获取64秒内的温度测量数据(传感器默认每64秒转换/测量一次温度)
    @return temperature, 单位:℃
  '''
  def get_temperature_C(mode=False):

  '''!
    @brief 获取传感器测量温度, 单位:℉
    @param mode 择传感器温度测量方式
    @n true 立即强制开始温度转换，测量和获得当前的温度立即
    @n false 关闭强制温度转换，获取64秒内的温度测量数据(传感器默认每64秒转换/测量一次温度)
    @return 温度测量值, 单位:℉
  '''
  def get_temperature_F(mode=False):

  '''!
    @brief 判断它是否断电
    @return True 表示rtc之前已经断电，需要复位时间; False 表示rtc运行良好
  '''
  def is_lost_power():

  '''!
    @brief 读取引脚sqw的值
    @return 枚举变量eSqwPinMode_t中witten值的解释
  '''
  def read_sqw_pin_mode():

  '''!
    @brief 设置sqw引脚的模式
    @param 枚举变量eSqwPinMode_t中witten值的解释
  '''
  def write_sqw_pin_mode(mode):

  '''!
    @brief 设置闹钟1
    @param alarmType 闹钟工作模式
    @param days    1-31
    @param hours   0-23
    @param minutes 0-59
    @param seconds 0-59
  '''
  def set_alarm1(alarmType, date, hour, minute, second):

  '''!
    @brief 设置闹钟2
    @param alarmType 闹钟工作模式
    @param days    1-31
    @param hours   0-23
    @param minutes 0-59
  '''
  def set_alarm2(alarmType, date, hour, minute):

  '''!
    @brief 检查闹钟标志是否被触发
  '''
  def is_alarm_trig():

  '''!
    @brief 清除触发的闹钟标志
  '''
  def clear_alarm():

  '''!
    @brief 启用闹钟1的中断
  '''
  def enable_alarm1_int():

  '''!
    @brief 禁用闹钟1的中断
  '''
  def disable_alarm1_int():

  '''!
    @brief 启用闹钟2的中断
  '''
  def enable_alarm2_int():

  '''!
    @brief 禁用闹钟2的中断
  '''
  def disable_alarm2_int():

  '''!
    @brief 开启32k频率输出
  '''
  def enable_32k():

  '''!
    @brief 关闭32k输出
  '''
  def disable_32k():

  '''!
    @brief 清除DS3232 SRAM上的数据
    @param addr 存储器地址
  '''
  def clear_SRAM(addr):

  '''!
    @brief 将数据写入DS3232的SRAM中
    @param addr 存储器地址
    @param data 写数据
  '''
  def write_SRAM(addr, data):

  '''!
    @brief 从DS3232的SRAM中读取数据
    @param addr 存储器地址
    @return 存储在SRAM中的数据
  '''
  def read_SRAM(addr):

```


## 兼容性

* RaspberryPi版本

| Board        | 正常运行  | 运行失败   | 未测试    | 备注
| ------------ | :-------: | :--------: | :------: | :-----: |
| RaspberryPi2 |           |            |    √     |         |
| RaspberryPi3 |           |            |    √     |         |
| RaspberryPi4 |     √     |            |          |         |

* Python版本

| Python  | 正常运行  | 运行失败   | 未测试    | 备注
| ------- | :-------: | :--------: | :------: | :-----: |
| Python2 |           |            |    √     |         |
| Python3 |     √     |            |          |         |


## 历史

- 2021/10/25 - 1.0.0 版本


## 创作者

Written by(yufeng.luo@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))

