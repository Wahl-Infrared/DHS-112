#include "common.h"

/*摄氏转华 */
void C_2_F(int *val)
{
  int temp = *val;

  temp = 32 + (int)(temp * 1.8);

  *val = temp;
}

/*华氏转摄*/
void F_2_C(int *val)
{
  int temp = *val;

  temp = (int)((temp - 32)/1.8);

  *val = temp;
}

void set_bit(unsigned int val, int bit)
{
	val |= (1 << bit);
}

void clear_bit(unsigned int val, int bit)
{
  val &= (~(1 << bit));
}

int test_bit(unsigned int val, int bit)
{
	if(val & (1 << bit)){
		return 1;
	}
	else{
		return 0;
	}	
}

u8 calc_crc8(u8 *message, int len)
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


