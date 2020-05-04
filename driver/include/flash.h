#ifndef _FLASH_H_
#define _FLASH_H_

#include "common.h"

#define   SETTING_ADDR  (0x1000)
#define   SETTING_BAK_ADDR  (0x1080)
#define   MAX_SETTING_SIZE  (64)

#define   CAL_ADDR  (0x1040)
#define   CAL_BAK_ADDR  (0x10c0)
#define   MAX_CAL_SIZE  (64)

void flash_init(void);
void wait_flash_busy();
int read_flash(u16 addr, u8 *ptr, u16 len);
void write_flash(u16 addr, u8 *buff, u16 len);

#endif