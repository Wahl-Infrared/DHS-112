#ifndef _COMMON_H_
#define _COMMON_H_

#define true 1
#define false 0
#define NULL 0

#define CPU_F ((double)1000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

typedef unsigned char u8;
typedef unsigned int  u16;
typedef unsigned long u32;

void C_2_F(int *val);
void F_2_C(int *val);
void set_bit(unsigned int val, int bit);
void clear_bit(unsigned int val, int bit);
int test_bit(unsigned int val, int bit);
u8 calc_crc8(u8 *message, int len);

#endif