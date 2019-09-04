/*
flash            

*/
//
//
//                  MSP430F4XX
//               +---------------+         
//               |               |
//               |           XOUT|-----
//               |               |      32.768KHz Watch Crystal
//               |            XIN|-----
//               |               |
//               +---------------+

#include "io430.h"
#include "system.h"
#include "string.h"
#include "therm_sensor.h"

#define   FLASH_SYSTEM_ADDR  (0x1000)     /*Flash 设置参数存储区首地址*/
#define   FLASH_SYSTEM_COPY_ADDR  (0x1080) /*Flash 设置参数备份区首地址*/
#define   FLASH_CAL_ADDR  (0x1040)     /*Flash 校准参数存储区首地址*/
#define   FLASH_CAL_COPY_ADDR  (0x10C0) /*Flash 校准参数备份区首地址*/
typedef unsigned char u8;
typedef unsigned int u16;

/****************************************************************************
* 名    称：Flash_Init()
* 功    能：对Flash时钟进行初始化设置
* 入口参数：无
* 出口参数：无
* 说    明: 如需使用后面的读写函数，在程序开始必须先调用该初始化函数
            配置Flash时钟必须在257~476KHz之间!!
****************************************************************************/
void flash_init(void)
{
  FCTL2 = FWKEY + FSSEL_2 + FN1; // 默认 SMCLK/3 =349KHz
}
  /* Flash时钟必须在257~476KHz之间, */
  /* 分频系数= 32*FN5 + 16*FN4 + 8*FN3 + 4*FN2 + 2*FN1 + FN0 + 1 */

  // Flash时钟选择:   
  /* FSSEL_0 Flash clock select: 0 - ACLK */
  /* FSSEL_1 Flash clock select: 1 - MCLK */
  /* FSSEL_2 Flash clock select: 2 - SMCLK */


/****************************************************************************
* 名    称：Flash_Busy()
* 功    能：Flash忙等待
* 入口参数：无
* 出口参数：无
* 说    明: 若Flash忙则等待，直到Flash操作完成(空闲)才返回。
****************************************************************************/
void flash_busy()
{
  while((FCTL3 & BUSY) == BUSY); //Busy
}

u8 get_crc8(u8 *message, int len)
{
    u8 crc = 0;
    int i;

    while(len--)
    {
        crc ^= *message++;
        for(i = 0;i < 8;i++)
        {
            if(crc & 0x01)
            {
                crc = (crc >> 1) ^ 0x8c;
            }
                else crc >>= 1;
        }
    }
    return crc;
}


int flash_read_system_setting()
{
  u8 crc_read, crc_calc;
  u8 *sys_ptr;
  u8 *flash_ptr;
  int i;

  sys_ptr = (u8*)p_system_setting;
  flash_ptr = (u8*)FLASH_SYSTEM_ADDR;
  for(i=0;i<sizeof(SYSTEM_SETTING);i++){
    *sys_ptr++ = *flash_ptr++;
  }
  crc_read = *flash_ptr;
  crc_calc = get_crc8((u8*)p_system_setting, sizeof(SYSTEM_SETTING));  
  if(crc_read == crc_calc){
    return 0;
  }
  
  sys_ptr = (u8*)p_system_setting;
  flash_ptr = (u8*)FLASH_SYSTEM_COPY_ADDR;
  for(i=0;i<sizeof(SYSTEM_SETTING);i++){
    *sys_ptr++ = *flash_ptr++;
  }
  crc_read = *flash_ptr;
  crc_calc = get_crc8((u8*)p_system_setting, sizeof(SYSTEM_SETTING));  
  if(crc_read == crc_calc){
    return 0;
  }

  return -1;
}

void flash_write_system_setting()
{
  u8 *flash_ptr, *flash_copy_ptr, *sys_ptr;
  u8 crc;
  int i;

  sys_ptr = (u8*)p_system_setting;
  flash_ptr = (u8*)FLASH_SYSTEM_ADDR;
  flash_copy_ptr = (u8*)FLASH_SYSTEM_COPY_ADDR;
  FCTL1 = FWKEY + ERASE;                    // Set Erase bit
  FCTL3 = FWKEY;                            // Clear Lock bit
  *flash_ptr = 0;
  flash_busy();
  FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation
  FCTL3 = FWKEY;   
  for(i=0; i<sizeof(SYSTEM_SETTING); i++){
    flash_busy();
    *flash_ptr++ = *sys_ptr;
    flash_busy();
    *flash_copy_ptr++ = *sys_ptr++;
  }

  crc = get_crc8((u8*)p_system_setting, sizeof(SYSTEM_SETTING)); 
  flash_busy();
  *flash_ptr = crc;
  flash_busy();
  *flash_copy_ptr = crc;
  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}


int flash_read_cal_param()
{
  u8 crc_read, crc_calc;
  u8 *cal_para_ptr;
  u8 *flash_ptr;
  int i;

  cal_para_ptr = (u8*)p_cal_param;
  flash_ptr = (u8*)FLASH_CAL_ADDR;
  for(i=0;i<sizeof(CAL_PARAM);i++){
    *cal_para_ptr++ = *flash_ptr++;
  }
  crc_read = *flash_ptr;
  crc_calc = get_crc8((u8*)p_cal_param, sizeof(CAL_PARAM));  
  if(crc_read == crc_calc){
    return 0;
  }

  cal_para_ptr = (u8*)p_cal_param;
  flash_ptr = (u8*)FLASH_CAL_COPY_ADDR;
  for(i=0;i<sizeof(CAL_PARAM);i++){
    *cal_para_ptr++ = *flash_ptr++;
  }
  crc_read = *flash_ptr;
  crc_calc = get_crc8((u8*)p_cal_param, sizeof(CAL_PARAM));  
  if(crc_read == crc_calc){
    return 0;
  }

  return -1;


}

void flash_write_cal_param()
{
  u8 *flash_ptr, *flash_copy_ptr, *cal_param_ptr;
  u8 crc;
  int i;

  cal_param_ptr = (u8*)p_cal_param;
  flash_ptr = (u8*)FLASH_CAL_ADDR;
  flash_copy_ptr = (u8*)FLASH_CAL_COPY_ADDR;
  FCTL1 = FWKEY + ERASE;                    // Set Erase bit
  FCTL3 = FWKEY;                            // Clear Lock bit
  *flash_ptr = 0;
  flash_busy();
  FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation
  FCTL3 = FWKEY;   
  for(i=0; i<sizeof(CAL_PARAM); i++){
    flash_busy();
    *flash_ptr++ = *cal_param_ptr;
    flash_busy();
    *flash_copy_ptr++ = *cal_param_ptr++;
  }

  crc = get_crc8((u8*)p_cal_param, sizeof(CAL_PARAM)); 
  flash_busy();
  *flash_ptr = crc;
  flash_busy();
  *flash_copy_ptr = crc;
  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}

