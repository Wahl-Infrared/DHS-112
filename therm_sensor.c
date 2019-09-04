/****************************************************************************
* SD16 is a multichannel 16-bit sigma-delta analog-to-digital converter.
* 16位转换， 速度更快
*
*****************************************************************************/

#include "io430.h"
#include "therm_sensor.h"
#include "system.h"

#define THERMISTOR_TC  -6.32
#define SD16_COUNT 65536


ST_TEMP_SENSOR therm_sensor = {0};
ST_TEMP_VAL therm_val = {0};
ST_TEMP_RESULT g_temp_result = {0};
CAL_PARAM cal_param;
PCAL_PARAM p_cal_param = &cal_param;


const TEMP_CURVE curve[CURVE_ARRAY_SIZE]={
  {0,0},
  {30,113},
  {120,350},
  {225,624},
  {323,901},
  {423,1210},
  {525,1551},
  {620,1919},
  {720,2302},
  {868,2957},
  {1018,3641},
  {1168,4380},
  {1314,5200},
  {1460,6072},
  {1609,6993},
  {1807,8266},
  {2005,9802},
  {2208,11326},
  {2408,12972},
  {2706,15737},
  {2208,11416}
};

void set_default_cal_param()
{
  p_cal_param->R_env_pullup=1000;
  p_cal_param->env_tem_1 = 270;
  p_cal_param->env_tem_2 = 0;
  p_cal_param->env_val_1 = 47290;
  p_cal_param->env_val_2 = 56741;
  p_cal_param->therm_basic= 34267;
  p_cal_param->therm_corr = 1;
}

void AD_init(void)
{
  SD16CTL = SD16REFON + SD16DIV_3; //internal reference
  SD16CCTL0 = SD16IE; //
  SD16CCTL1 = SD16IE | SD16DF;
}

void temp_detect_start(bool on)
{
  SD16CCTL0_bit.SD16SC = on;
  SD16CCTL1_bit.SD16SC = on;
}

/****************************************************************************
* 名    称：__SD16_conversion_read()
* 功    能：SD16转换结束中断函数，读取转换结果
* 入口参数：无
* 出口参数：无
* 说    明: 
****************************************************************************/
#pragma vector = SD16_VECTOR
__interrupt void __SD16_conversion_read(void)
{
  switch(SD16IV){
    case SD16IV_SD16OVIFG:
      SD16CCTL1_bit.SD16OVIFG = 0;
      break;
    case SD16IV_SD16MEM0:
      therm_sensor.therm_value[therm_sensor.therm_array_offset] = SD16MEM0;
      if(++(therm_sensor.therm_array_offset) >= TEMP_ARRAY_SIZE){
        SD16CCTL0_bit.SD16SC = 0;
        therm_sensor.therm_value_flag = 1;
        therm_sensor.therm_array_offset = 0;
      }
      break;
    case SD16IV_SD16MEM1:
      therm_sensor.env_value[therm_sensor.env_array_offset] = SD16MEM1;
      if(++(therm_sensor.env_array_offset) >= TEMP_ARRAY_SIZE){
        SD16CCTL1_bit.SD16SC = 0;
        therm_sensor.env_value_flag=1;
        therm_sensor.env_array_offset = 0;
      }
      break;
    default:
      break;
  }
}

/*Rx = 100k - 6.32 k/℃*/
int calc_env_temp()
{
  float temp_percent;
  float R_thermistor;
  int temp;
  temp_percent = (float)SD16_COUNT/therm_val.env_avg - 1;
  R_thermistor = p_cal_param->R_env_pullup / temp_percent;
  temp = (int)(((R_thermistor - 100) /THERMISTOR_TC + 25)*10);

  return temp;
}


unsigned int delta_temp_2_value(int delta_temp)
{
  int i;
  unsigned int delta_value;
  unsigned int delta_i_value;
  unsigned int delta_i_temp;
  unsigned int delta_x_value;

  for(i=0;i<CURVE_ARRAY_SIZE;i++){
     if(delta_temp < curve[i].delta_temp){
       break;
     }    
  }

  if(i == CURVE_ARRAY_SIZE){
    i--;
  }

  delta_i_value = curve[i].delta_therm_value-curve[i-1].delta_therm_value;
  delta_i_temp = curve[i].delta_temp-curve[i-1].delta_temp;
  delta_x_value = (int)(delta_i_value * 1.0/ delta_i_temp * (curve[i].delta_temp - delta_temp));
  delta_value = curve[i].delta_therm_value - delta_x_value;
 
  return delta_value;

}

int delta_value_2_temp(unsigned int delta_value)
{
  int i;
  unsigned int delta_temp;
  unsigned int delta_i_value;
  unsigned int delta_i_temp;
  unsigned int delta_x_temp;
  
  if(delta_value == 0){
    return 0;
  }

  for(i=1;i<CURVE_ARRAY_SIZE;i++){
     if(delta_value < curve[i].delta_therm_value){
       break;
     }    
  }

  if(i == CURVE_ARRAY_SIZE){
    i--;
  }

  delta_i_value = curve[i].delta_therm_value-curve[i-1].delta_therm_value;
  delta_i_temp = curve[i].delta_temp-curve[i-1].delta_temp;
  delta_x_temp = (int)(delta_i_temp * 1.0/delta_i_value * (curve[i].delta_therm_value - delta_value));
  delta_temp = curve[i].delta_temp - delta_x_temp;
 
  return delta_temp;
}

void calc_temp_process()
{
  unsigned char i;
  unsigned long sum_therm_value;
  unsigned long sum_env_value;
  
  unsigned int env_value;
  unsigned int delta_therm;
  unsigned int delta_therm1;
  unsigned int delta_therm2;
  unsigned int delta_temp;
  unsigned char neg_temp;
  int temp;

  for(i = 0, sum_therm_value = 0, sum_env_value = 0; i < TEMP_ARRAY_SIZE; i++){
    sum_therm_value += therm_sensor.therm_value[i];
    sum_env_value += therm_sensor.env_value[i];
  }
  
  therm_val.therm_value[therm_val.value_offset]= (unsigned int)(sum_therm_value/TEMP_ARRAY_SIZE);
  therm_val.env_value[therm_val.value_offset]= (unsigned int)(sum_env_value/TEMP_ARRAY_SIZE);

  if(++(therm_val.value_offset) >= TEMP_ARRAY_SIZE){
    therm_val.value_offset = 0;

    for(i = 0, sum_therm_value = 0, sum_env_value = 0; i < TEMP_ARRAY_SIZE; i++){
      sum_therm_value += therm_val.therm_value[i];
      sum_env_value += therm_val.env_value[i];
    }
    
    therm_val.therm_avg = (unsigned int)(sum_therm_value/TEMP_ARRAY_SIZE);
    therm_val.env_avg = (unsigned int)(sum_env_value/TEMP_ARRAY_SIZE);

    g_temp_result.env_temp = calc_env_temp();
    

    if(therm_val.therm_avg > therm_val.therm_max){
      therm_val.therm_max = therm_val.therm_avg;
    }

    /* TBD */
    if(therm_val.therm_avg < p_cal_param->therm_basic){
      delta_therm = p_cal_param->therm_basic - therm_val.therm_avg;
      neg_temp = 1;
    }else{
      delta_therm = therm_val.therm_avg - p_cal_param->therm_basic;
      neg_temp = 0;
    }

    delta_therm1 = delta_therm/(p_system_setting->ems/100.0);  /*辐射率补偿*/    
    delta_therm2 = (unsigned int)(delta_therm1 * p_cal_param->therm_corr); /*校准值补偿*/

    delta_temp = delta_value_2_temp(delta_therm2);

    if(neg_temp){
      temp = g_temp_result.env_temp - delta_temp;
    }else{
      temp = g_temp_result.env_temp + delta_temp;
    }

    if(p_system_setting->c_f){
      C_2_F(&temp);
    }

    g_temp_result.therm_temp = temp;

    if(temp > g_temp_result.max_temp){
      g_temp_result.max_temp = temp;
    }

    if(temp < g_temp_result.min_temp){
      g_temp_result.min_temp = temp;
    }

    
  }  
}

void read_therm_temp()
{
  therm_sensor.therm_value_flag = 0;
  therm_sensor.env_value_flag = 0;
  temp_detect_start(1);
  while (therm_sensor.therm_value_flag == 0 || therm_sensor.env_value_flag == 0);
  calc_temp_process();

/*
  if(temp > MAX_TEMP){
    temp = MAX_TEMP;
  }

  if(temp < MIN_TEMP){
    temp = MIN_TEMP;
  }
*/



//  if(g_temp_result.therm)
  
//  unsigned char buff[60];
//  sprintf(buff, "%u", g_temp_result.therm_temp);
  
//  uart_send_str(buff);
  flush_LCD_disp();
}


void cal_process()
{
  static unsigned char hold=1;

  while(1){
    if(P1IN_bit.P7 == 0){
      if(hold){
        g_temp_result.max_temp = MIN_TEMP;
        g_temp_result.min_temp = MAX_TEMP;
        therm_val.therm_max = 0;
        hold = 0;
      }
      read_therm_temp();
      flush_LCD_disp();
    }else{
      hold = 1;
    }
  }
}

