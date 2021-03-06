/****************************************************************************
* SD16 is a multichannel 16-bit sigma-delta analog-to-digital converter.
* 16位转换， 速度更快
*
*****************************************************************************/

#include "io430.h"
#include "therm_sensor.h"
#include "system.h"


#define THERMOPILE_CHANNEL SD16INCH0
#define THERMISTOR_CHANNEL SD16INCH1

#define THERMISTOR_TC  -6.32
#define SD16_VREF  1.2
#define SD16_COUNT 65536
#define ENV_TEMP_MIN -50
#define ENV_TEMP_MAX 150
#define ENV_TEMP_STEP 5
#define ENV_TEMP_0_OFFSET 10
#define OBJECT_TEMP_MIN -50
#define OBJECT_TEMP_MAX 1500
#define OBJECT_TEMP_STEP 10
#define OBJECT_TEMP_0_OFFSET 5
/*********************************************
   STP9CF89    探头固定环温测量不同目标
   环温:    0 摄氏度
   测量范围:  -50   到 1500  摄氏度
   目标步进: 10
   ********************************************/
const float  Thermopile_env_0_C[]={
  -0.707,  -0.587,  -0.457,  -0.316,  -0.163,   0.000,   0.175,   0.363,
   0.563,   0.776,   1.003,   1.243,   1.498,   1.767,   2.051,   2.350,
   2.665,   2.997,   3.344,   3.709,   4.091,   4.490,   4.908,   5.343,
   5.798,   6.272,   6.765,   7.278,   7.811,   8.365,   8.940,   9.536,
   10.154,  10.794,  11.457,  12.142,  12.851,  13.583,  14.339,  15.120,
   15.925,  16.755,  17.611,  18.492,  19.400,  20.334,  21.295,  22.283,
   23.299,  24.342,  25.415,  26.515,  27.645,  28.804,  29.993,  31.212,
   32.462,  33.742,  35.053,  36.396,  37.771,  39.179,  40.618,  42.091,
   43.597,  45.137,  46.711,  48.319,  49.962,  51.640,  53.354,  55.103,
   56.889,  58.711,  60.570,  62.466,  64.399,  66.371,  68.381,  70.429,
   72.516,  74.643,  76.810,  79.016,  81.263,  83.550,  85.879,  88.248,
   90.660,  93.114,  95.610,  98.149,  100.731, 103.357, 106.026, 108.740,
   111.498, 114.301, 117.149, 120.043, 122.982, 125.968, 129.001, 132.080,
   135.207, 138.381, 141.603, 144.874, 148.193, 151.561, 154.978, 158.445,
   161.963, 165.530, 169.148, 172.817, 176.538, 180.310, 184.134, 188.011,
   191.940, 195.923, 199.959, 204.048, 208.192, 212.390, 216.643, 220.951,
   225.314, 229.734, 234.209, 238.740, 243.329, 247.975, 252.678, 257.438,
   262.257, 267.135, 272.071, 277.066, 282.121, 287.236, 292.411, 297.646,
   302.942, 308.299, 313.718, 319.198, 324.741, 330.346, 336.014, 341.745,
   347.539, 353.397, 359.320, 365.307
};

/*********************************************
   STP9CF89    探头不同环境温度下测量固定目标
   目标温度:    0 摄氏度
   环温范围:  -50   到 150  摄氏度
   测量步进:  5
 ********************************************/
const float Thermopile_obj_0_C[]={ 
   0.707,  0.648,  0.587,  0.523,  0.457,  0.387,  0.316,  0.241,
   0.163,  0.083,  0.000, -0.086, -0.175, -0.268, -0.363, -0.461,
  -0.563, -0.668, -0.776, -0.888, -1.003, -1.121, -1.243, -1.369,
  -1.498, -1.631, -1.767, -1.907, -2.051, -2.199, -2.350, -2.506,
  -2.665, -2.829, -2.997, -3.168, -3.344, -3.525, -3.709, -3.898,
  -4.091
};

ST_THERM_SENSOR therm_sensor = {0};
ST_TEMP_RESULT g_temp_result = {0};
CAL_PARAM cal_param;
PCAL_PARAM p_cal_param = &cal_param;

void set_default_cal_param()
{
  p_cal_param->R_env_pullup = 1000.00;
  p_cal_param->gain = 122.95;
  p_cal_param->basic_SD16_val = 2247;
}

void SD16_init(void)
{
  SD16CTL = SD16REFON + SD16DIV_3; //internal reference
  SD16CCTL0 = SD16IE | SD16DF; //
  SD16CCTL1 = SD16IE | SD16DF;
}

void start_SD16_convert(unsigned int channel)
{
  switch(channel){
    case SD16INCH0:
      SD16CCTL0_bit.SD16SC = 1;
      break;
    case SD16INCH1:
      SD16CCTL1_bit.SD16SC = 1;
      break;
//    case SD16INCH2:
//      SD16CCTL2_bit.SD16SC = 1;
//      break;
    default:
      SD16CCTL0_bit.SD16SC = 0;
      SD16CCTL1_bit.SD16SC = 0;
 //     SD16CCTL2_bit.SD16SC = 0;
  }  
}

void stop_SD16_convert(unsigned int channel)
{
  switch(channel){
    case SD16INCH0:
      SD16CCTL0_bit.SD16SC = 0;
      break;
    case SD16INCH1:
      SD16CCTL1_bit.SD16SC = 0;
      break;
 //   case SD16INCH2:
//      SD16CCTL2_bit.SD16SC = 0;
//      break;
    default:
      SD16CCTL0_bit.SD16SC = 0;
      SD16CCTL1_bit.SD16SC = 0;
 //     SD16CCTL2_bit.SD16SC = 0;
  }  
}


/****************************************************************************
* 名    称：__SD16_conversion_read()
* 功    能：SD16转换结束中断函数，读取转换结果
* 入口参数：无
* 出口参数：无
* 说    明: 
****************************************************************************/
#pragma vector = SD16_VECTOR
__interrupt void __read_SD16_conversion(void)
{
  switch(SD16IV){
    case SD16IV_SD16OVIFG:
      SD16CCTL1_bit.SD16OVIFG = 0;
      break;
    case SD16IV_SD16MEM0:
      therm_sensor.thermopile_sum_value += SD16MEM0;
      therm_sensor.thermopile_value_count++;
      break;
    case SD16IV_SD16MEM1:
      therm_sensor.thermistor_sum_value += SD16MEM1;
      therm_sensor.thermistor_value_count++;
      break;
    default:
      break;
  }
}

unsigned int read_thermopile_value()
{
  unsigned int avg_value;

  therm_sensor.thermopile_sum_value = 0;
  therm_sensor.thermopile_value_count = 0;
  start_SD16_convert(THERMOPILE_CHANNEL);
  while(therm_sensor.thermopile_value_count < THERMOPILE_SUM_COUNT);
  stop_SD16_convert(THERMOPILE_CHANNEL);

  avg_value = (unsigned int)((float)therm_sensor.thermopile_sum_value/therm_sensor.thermopile_value_count);
  return avg_value;
}

unsigned int read_thermistor_value()
{
  unsigned int avg_value;

  therm_sensor.thermistor_sum_value = 0;
  therm_sensor.thermistor_value_count = 0;
  start_SD16_convert(THERMISTOR_CHANNEL);
  while(therm_sensor.thermistor_value_count < THERMISTOR_SUM_COUNT);
  stop_SD16_convert(THERMISTOR_CHANNEL);

  avg_value = (unsigned int)((float)therm_sensor.thermistor_sum_value/therm_sensor.thermistor_value_count);
  return avg_value;
}

/*获取目标thermopile 值*/
float get_objcet_thermopile()
{
  unsigned int thermopile_value;
  int delta_val;
  float thermopile;

  thermopile_value = read_thermopile_value();
  
  delta_val = (int)(thermopile_value - p_cal_param->basic_SD16_val);
  thermopile = delta_val * SD16_VREF / SD16_COUNT / p_cal_param->gain;
  
  return thermopile;
}

/*****************************
  * Function: get_thermistor_temp()
  * 功能:  读取热敏电阻温度值
  * 热敏电阻系数:  -6.32
  *以25℃为基准公式:    Rx = 100k - 6.32 k/℃
******************************/
int get_thermistor_temp()
{
   unsigned int value;
   float temp_percent;
   float R_thermistor;
   int temp;
   
   value = read_thermistor_value();
   temp_percent = (float)SD16_COUNT/value - 1;
   R_thermistor = p_cal_param->R_env_pullup / temp_percent;
   temp = (int)((R_thermistor - 100) /THERMISTOR_TC + 25);
  
   return temp;
}

/*******************************************************************
 * 采用分段线性计算当前环温下0℃目标的thermopile
 * 数组: Thermopile_obj_0_C
 * 步骤:
 * 1. 在数组中找到当前温度所处区间offset
 * 2. 根据offset 和offset+1 的值计算该区间的斜率 k
 * 3.根据k值得到当前温度对应的值。
 *
******************************************/
float get_obj_0_thermopile()
{
  float k,thermopile;
  unsigned int offset,array_size;
  int env;

  env = get_thermistor_temp();

  array_size = sizeof(Thermopile_obj_0_C);
  offset = ENV_TEMP_0_OFFSET + env/ENV_TEMP_STEP;

  if (offset < 1){
    k = (Thermopile_obj_0_C[1]-Thermopile_obj_0_C[0])/ENV_TEMP_STEP;
    thermopile = Thermopile_obj_0_C[0] + k * (env - ENV_TEMP_MIN);
  }else if(offset > array_size -2){
    k = (Thermopile_obj_0_C[array_size-1]-Thermopile_obj_0_C[array_size-2])/ENV_TEMP_STEP;
    thermopile = Thermopile_obj_0_C[array_size-1] + k * (env - ENV_TEMP_MAX);
  }else{
    k = (Thermopile_obj_0_C[offset+1]-Thermopile_obj_0_C[offset])/ENV_TEMP_STEP;
    thermopile = Thermopile_obj_0_C[offset] + k * (env%ENV_TEMP_STEP);
  }

  return thermopile;
}


unsigned int find_therm_offset(float therm)
{
  unsigned int array_size = sizeof(Thermopile_env_0_C)/sizeof(float);
  unsigned int offset, max_offset, min_offset;

  min_offset = 0;
  max_offset = array_size -1;
  offset = array_size / 2;

  while(1){
    if(Thermopile_env_0_C[offset] > therm){
      max_offset = offset;
    }else if(Thermopile_env_0_C[offset] < therm){
      min_offset = offset;
    }else{
      return offset;
    }

    offset = (min_offset + max_offset) / 2;

    if(offset == min_offset){
      return offset;
    }
  }
}

int thermopile_2_temp(float therm)
{
  float k, delta_therm;
  int offset, temp, delta_temp;
  
  offset = find_therm_offset(therm);
  delta_therm = therm - Thermopile_env_0_C[offset];
  k = OBJECT_TEMP_STEP*1.0 / (Thermopile_env_0_C[offset + 1] - Thermopile_env_0_C[offset]);
  delta_temp = (int)(delta_therm * k);

  temp = OBJECT_TEMP_MIN + OBJECT_TEMP_STEP * offset + delta_temp;
  return temp;
}


int get_object_temp()
{
  float cur_thermopile,thermopile_0;
  float ems = p_system_setting->ems / 100.0;


  cur_thermopile = get_objcet_thermopile();
  cur_thermopile = cur_thermopile / ems;  /*辐射率补偿*/   
  
  thermopile_0 = get_obj_0_thermopile();  /*移动到0℃曲线上  */
  cur_thermopile -= thermopile_0;
  
  return thermopile_2_temp(cur_thermopile);
}

void cal_process()
{
  static unsigned char hold=1;

  while(1){
    if(P1IN_bit.P7 == 0){
      if(hold){
        g_temp_result.max_temp = MIN_TEMP;
        g_temp_result.min_temp = MAX_TEMP;
        g_temp_result.max_thermopile = 0;
        hold = 0;
      }
      g_temp_result.therm_temp = get_object_temp();
      flush_LCD_disp();
    }else{
      hold = 1;
    }
  }
}

