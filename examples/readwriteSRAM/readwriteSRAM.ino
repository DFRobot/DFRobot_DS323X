/*!
 * @file readWriteSRAM.ino
 * @brief read and write data in SRAM, only work on DS3232 
 * @n Experiment phenomenon: There are 236 bytes of SRAM available for reading and writing 
 * @n                        The address of SRAM is 0x14~0xFF
 *
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [LuoYufeng](yufeng.luo@dfrobot.com)
 * @version  V0.1
 * @date  2021-2-23
 * @url https://github.com/DFRobot/DFRobot_DS323X
 */
#include "DFRobot_DS323X.h"

DFRobot_DS323X rtc;
uint8_t buffer[] = {0x10,0x11,0x24,0x36,0x45,0x65,0x17,0x30,0x78,0xA4,0x89};
int i = 0;

void setup(void)
{
    Serial.begin(9600);
    /*Wait for the chip to be initialized completely, and then exit*/
    while(rtc.begin() != true){
        Serial.println("Failed to init chip, please check if the chip connection is fine. ");
        delay(1000);
    }
    /*!
     *@brief clear the SRAM
     *@param addr 0x14~0xFF
     *@return true means write is successful, false means write is failed
     */
    for (uint8_t addr = 0x14; addr < 0x1F; addr++){
        while (rtc.clearSRAM(addr) != true){}
    }
    /*!
     *@brief write data into the SRAM
     *@param addr 0x14~0xFF
     *@param data uint8_t HEX
     *@return true means write is successful, false means write is failed
     */
    for (uint8_t addr = 0x14; addr < 0x1F; addr++){
        while (rtc.writeSRAM(addr,buffer[i]) != true){}
        i++;
    }
    delay(1000);
}

void loop() {
    /*!
     *@brief read  the SRAM
     *@param addr 0x14~0xFF
     *@return data store in the SRAM
     */
    for (uint8_t addr = 0x14; addr < 0x1F; addr++){
        Serial.print(rtc.readSRAM(addr),HEX);
        Serial.print(" ");
        delay(200);
    }
    delay(1000);
    Serial.println();
}
