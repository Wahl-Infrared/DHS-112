#ifndef _FLASH_H_
#define _FLASH_H_

#include "common.h"

void flash_init(void);
void wait_flash_busy();
int read_flash(u16 addr, u8 *ptr, u16 len);
void write_flash(u16 addr, u8 *buff, u16 len);

#endif