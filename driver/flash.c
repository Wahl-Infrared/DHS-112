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
#include "common.h"

/****************************************************************************
* �?   称：Flash_Init()
* �?   能：对Flash时钟进行初始化设�?* 入口参数：无
* 出口参数：无
* �?   �? 如需使用后面的读写函数，在程序开始必须先调用该初始化函数
            配置Flash时钟必须�?57~476KHz之间!!
****************************************************************************/
void flash_init(void)
{
  FCTL2 = FWKEY + FSSEL_2 + FN1; // 默认 SMCLK/3 =349KHz
}
  /* Flash时钟必须�?57~476KHz之间, */
  /* 分频系数= 32*FN5 + 16*FN4 + 8*FN3 + 4*FN2 + 2*FN1 + FN0 + 1 */

  // Flash时钟选择:   
  /* FSSEL_0 Flash clock select: 0 - ACLK */
  /* FSSEL_1 Flash clock select: 1 - MCLK */
  /* FSSEL_2 Flash clock select: 2 - SMCLK */


/****************************************************************************
* �?   称：Flash_Busy()
* �?   能：Flash忙等�?* 入口参数：无
* 出口参数：无
* �?   �? 若Flash忙则等待，直到Flash操作完成(空闲)才返回�?****************************************************************************/
void wait_flash_busy()
{
  while((FCTL3 & BUSY) == BUSY); //Busy
}

int read_flash(u16 addr, u8 *ptr, u16 len)
{
  u8 *flash_ptr;
  int i;

  flash_ptr = (u8*)addr;
  for(i=0;i<len;i++){
    *ptr++ = *flash_ptr++;
  }
  return 0;
}

void write_flash(u16 addr, u8 *buff, u16 len)
{
  u8 *flash_ptr;
  int i;

  flash_ptr = (u8*)addr;

  FCTL1 = FWKEY + ERASE;                    // Set Erase bit
  FCTL3 = FWKEY;                            // Clear Lock bit
  *flash_ptr = 0;
  wait_flash_busy();
  FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation
  FCTL3 = FWKEY;

  for(i=0; i<len; i++){
    wait_flash_busy();
    *flash_ptr++ = *buff++;
    wait_flash_busy();
  }
}
