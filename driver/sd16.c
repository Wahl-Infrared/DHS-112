#include "io430.h"

#define FILTER_ARRAY_SIZE 16

typedef struct _ST_SD16_FILTER_{
  unsigned long sum_ch0_val;
  unsigned long sum_ch1_val;
  unsigned char ch0_count;
  unsigned char ch1_count;
}ST_SD16_FILTER;

ST_SD16_FILTER sd16;


void SD16_init(void)
{
  SD16CTL = SD16REFON + SD16DIV_3; //internal reference
  SD16CCTL0 = SD16IE + SD16DF; //
  SD16CCTL1 = SD16IE + SD16DF;
}

void get_sd16_val(unsigned int *reg)
{
  sd16.ch0_count = 0;
  sd16.ch1_count = 0;
  sd16.sum_ch0_val = 0;
  sd16.sum_ch1_val = 0;

  SD16CCTL0_bit.SD16SC = 1;
  SD16CCTL1_bit.SD16SC = 1;
  while(sd16.ch0_count < FILTER_ARRAY_SIZE){;}
  while(sd16.ch1_count < FILTER_ARRAY_SIZE){;}

  *reg = (unsigned int)((float)sd16.sum_ch0_val/FILTER_ARRAY_SIZE);
  *(reg+1) = (unsigned int)((float)sd16.sum_ch1_val/FILTER_ARRAY_SIZE);
}

/****************************************************************************
* 名    称：__SD16_conversion_read()
* 功    能：SD16转换结束中断函数，读取转换结果
* 入口参数：无
* 出口参数：无
* 说    明: 
****************************************************************************/
#pragma vector = SD16_VECTOR
__interrupt void __SD16_vector__(void)
{
  switch(SD16IV){
    case SD16IV_SD16OVIFG:
      SD16CCTL1_bit.SD16OVIFG = 0;
      break;
    case SD16IV_SD16MEM0:
      sd16.sum_ch0_val += SD16MEM0;
      if(++sd16.ch0_count >= FILTER_ARRAY_SIZE){
        SD16CCTL0_bit.SD16SC = 0;
      }
      break;
    case SD16IV_SD16MEM1:
     	sd16.sum_ch1_val += SD16MEM1;
      if(++sd16.ch1_count >= FILTER_ARRAY_SIZE){
        SD16CCTL1_bit.SD16SC = 0;
      }
      break;
    default:
      break;
  }
}

