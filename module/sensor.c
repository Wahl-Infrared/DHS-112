#include "common.h"
#include "cal.h"
#include "settings.h"

#define ENV_CURVE_STEP 5
#define OBJ_CURVE_STEP 10
#define VREF 1.2
#define SD16_V_REF 1200


#if 0
/* 环温 25℃， 目标 -50 ~ 1500 各点 探头输出电压(mV), 步进10 */
const float thermopile_output[]={
 -1.168, -1.048, -0.918, -0.777, -0.625,  -0.461,
 -0.286, -0.098,  0.102,  0.315,  0.541,  0.782,  1.036,  1.306,  1.590,  1.889,
  2.204,  2.535,  2.883,  3.248,  3.629,  4.029,  4.446,  4.882,  5.337,  5.810,  
  6.303,  6.816,  7.350,  7.904,  8.478,  9.075,  9.693, 10.333, 10.995, 11.681,
 12.390, 13.122, 13.878, 14.658, 15.464, 16.294, 17.149, 18.031, 18.938, 19.873,
 20.833, 21.822, 22.837, 23.881, 24.953, 26.054, 27.184, 28.343, 29.532, 30.751,
 32.000, 33.280, 34.592, 35.935, 37.310, 38.717, 40.157, 41.630, 43.136, 44.676,
 46.250, 47.858, 49.501, 51.179, 52.893, 54.642, 56.427, 58.249, 60.108, 62.004, 
 63.938, 65.909, 67.919, 69.968, 72.055, 74.182, 76.348, 78.554, 80.801, 83.089,
 85.417, 87.787, 90.199, 92.652, 95.149, 97.688,100.270,102.895,105.565,108.278,
111.036,113.839,116.688,119.581,122.521,125.507,128.539,131.619,134.745,137.919,
141.142,144.412,147.731,151.099,154.517,157.984,161.501,165.069,168.687,172.356,
176.076,179.849,183.673,187.550,191.479,195.462,199.497,203.587,207.731,211.929,
216.182,220.490,224.853,229.272,233.747,238.279,242.868,247.513,252.216,256.977,
261.796,266.673,271.610,276.605,281.660,286.774,291.949,297.184,302.480,307.838,
313.256,318.737,324.279,329.884,335.552,341.283,347.078,352.936,358.858,364.845};
#endif

/* 环温 0℃， 目标 -50 ~ 1500 各点 探头输出电压(mV), 步进10 */
const float thermopile_obj_curve[]={
 -0.707,  -0.587,  -0.457,  -0.316,  -0.163,
  0.000,
  0.175,   0.363,   0.563,   0.776,   1.003,   1.243,   1.498,   1.767,   2.051,   2.350,
  2.665,   2.997,   3.344,   3.709,   4.091,   4.490,   4.908,   5.343,   5.798,   6.272,
  6.765,   7.278,   7.811,   8.365,   8.940,   9.536,  10.154,  10.794,  11.457,  12.142,
 12.851,  13.583,  14.339,  15.120,  15.925,  16.755,  17.611,  18.492,  19.400,  20.334,
 21.295,  22.283,  23.299,  24.342,  25.415,  26.515,  27.645,  28.804,  29.993,  31.212,
 32.462,  33.742,  35.053,  36.396,  37.771,  39.179,  40.618,  42.091,  43.597,  45.137,
 46.711,  48.319,  49.962,  51.640,  53.354,  55.103,  56.889,  58.711,  60.570,  62.466,
 64.399,  66.371,  68.381,  70.429,  72.516,  74.643,  76.810,  79.016,  81.263,  83.550,
 85.879,  88.248,  90.660,  93.114,  95.610,  98.149, 100.731, 103.357, 106.026, 108.740,
111.498, 114.301, 117.149, 120.043, 122.982, 125.968, 129.001, 132.080, 135.207, 138.381,
141.603, 144.874, 148.193, 151.561, 154.978, 158.445, 161.963, 165.530, 169.148, 172.817,
176.538, 180.310, 184.134, 188.011, 191.940, 195.923, 199.959, 204.048, 208.192, 212.390,
216.643, 220.951, 225.314, 229.734, 234.209, 238.740, 243.329, 247.975, 252.678, 257.438,
262.257, 267.135, 272.071, 277.066, 282.121, 287.236, 292.411, 297.646, 302.942, 308.299,
313.718, 319.198, 324.741, 330.346, 336.014, 341.745, 347.539, 353.397, 359.320, 365.307};

/* object 0℃， 环温 -50~150�?输出电压（mV）， 步进 5 */
const float thermopile_env_curve[]={
 0.707,	 0.648,	 0.587,	 0.523,	 0.457,	 0.387,	 0.316,	 0.241,	 0.163,  0.083,
 0.000,	-0.086,	-0.175,	-0.268,	-0.363,	-0.461,	-0.563,	-0.668,	-0.776,	-0.888,
-1.003,	-1.121,	-1.243,	-1.369,	-1.498,	-1.631,	-1.767,	-1.907,	-2.051,	-2.199,
-2.350,	-2.506,	-2.665,	-2.829,	-2.997,	-3.168,	-3.344,	-3.525,	-3.709,	-3.898,
-4.091};

/* 环温传感�?热敏电阻�?-20 ~ 80 �?*/

const float thermistor_curve[]={
975.8038, 920.5962, 868.8615, 820.3603, 774.8710, 732.1889, 692.1238, 654.4999, 619.1540, 585.9346,
554.7016, 525.3245, 497.6821, 471.6621, 447.1599, 424.0781, 402.3264, 381.8204, 362.4818, 344.2375,
327.0195, 310.7640, 295.4121, 280.9084, 267.2014, 254.2428, 241.9877, 230.3940, 219.4224, 209.0361,
199.2007, 189.8841, 181.0559, 172.6881, 164.7540, 157.2290, 150.0898, 143.3144, 136.8825, 130.7749,
124.9734, 119.4612, 114.2223, 109.2417, 104.5053,      100,  95.7132,  91.6333,  87.7492,  84.0505,
 80.5274,  77.1707,  73.9717,  70.9222,  68.0144,  65.2411,  62.5954,  60.0707,  57.6610,  55.3604,
 53.1635,  51.0651,  49.0602,  47.1443,  45.3130,  43.5621,  41.8878,  40.2862,  38.7539,  37.2876,
 35.8842,  34.5405,  33.2538,  32.0214,  30.8408,  29.7096,  28.6253,  27.5860,  26.5895,  25.6338,
 24.7171,  23.8376,  22.9937,  22.1836,  21.4061,  20.6594,  19.9424,  19.2537,  18.5920,  17.9562,
 17.3452,  16.7578,  16.1930,  15.6499,  15.1276,  14.6251,  14.1417,  13.6764,  13.2286,  12.7976,
 12.3825};
 /*,  11.9828,  11.5978,  11.2270,  10.8697,  10.5254,  10.1935,   9.8736,   9.5652,   9.2678,
  8.9809,   8.7042,   8.4373,   8.1797,   7.9312,   7.6912,   7.4596,   7.2360,   7.0201,   6.8115,
  6.6101,   6.4155,   6.2274};
*/

/* 查询目标�?�?热敏电阻曲线表中的位�?*/
u8 find_thermistor_offset(float val)
{
  u8 hi_off = sizeof(thermistor_curve);
  u8 lo_off = 0;
  u8 cur_off;

  cur_off = (hi_off + lo_off)/2;

  while(cur_off != hi_off && cur_off != lo_off){
    if(val > thermistor_curve[cur_off]){
      hi_off = cur_off;
    }else if(val < thermistor_curve[cur_off]){
      lo_off = cur_off;
    }else{
      break;
    }
    cur_off = (hi_off + lo_off)/2;
  }
  return cur_off;
}

/* 查询目标�?�?热电堆曲线表中的位置 */
u8 find_thermopile_obj_offset(float val)
{
  u8 hi_off = sizeof(thermopile_obj_curve);
  u8 lo_off = 0;
  u8 cur_off;

  cur_off = (hi_off + lo_off)/2;

  while(cur_off != hi_off && cur_off != lo_off){
    if(val < thermopile_obj_curve[cur_off]){
      hi_off = cur_off;
    }else if(val > thermopile_obj_curve[cur_off]){
      lo_off = cur_off;
    }else{
      break;
    }
    cur_off = (hi_off + lo_off)/2;
  }
  return cur_off;
}

/* 通过环温 计算温度补偿�?*/
float get_thermopile_env_val(float temp)
{
  u8 offset;
  int off_temp;
  float delta;
  float K;

  if(temp <= -50){
    offset = 0;
    off_temp = -50;
  }else if(temp < 0){
    offset = (u8)temp/5 + 9;
    off_temp = 5 * offset - 50;
  }else if(temp < 150){
    offset = (u8)temp/5 + 10;
    off_temp = 5 * offset - 50;
  }else{
    offset = 39;
    off_temp = 145;
  }

  K = (thermopile_env_curve[offset+1]-thermopile_env_curve[offset])/ENV_CURVE_STEP;
  delta = K * (temp - off_temp);

  return (thermopile_env_curve[offset] + delta);
}

/* 通过温度�?查询传感器电�?*/
float get_thermopile_val(float temp)
{
  u8 offset;
  int off_temp;
  float delta;
  float K;

  if(temp <= -50){
    offset = 0;
    off_temp = -50;
  }else if(temp < 0){
    offset = (u16)temp/10 + 4;
    off_temp = 10 * offset - 50;
  }else if(temp < 1500){
    offset = (u16)temp/10 + 5;
    off_temp = 10 * offset - 50;
  }else{
    offset = 154;
    off_temp = 1490;
  }



  K = (thermopile_obj_curve[offset+1] - thermopile_obj_curve[offset]) / 10;
  delta = K * (temp - off_temp);

  return thermopile_obj_curve[offset] + delta;

}



/* 通过 输出电压 查询 温度 */
float get_obj_temp(float voltage)
{
  float k;
  u8 offset;
  float delta;
  float off_temp;

  offset = find_thermopile_obj_offset(voltage);
  off_temp = -50 + offset * 10;

  k = (thermopile_obj_curve[offset+1] - thermopile_obj_curve[offset])/OBJ_CURVE_STEP;

  delta = k * (voltage - thermopile_obj_curve[offset]);

  return (off_temp + delta);
}

/*通过 温度 查表计算 热敏电阻阻�? */
float get_thermistor_val(int temp)
{
  u8 offset;
  int off_temp;
  float K;
  float delta;

  if(temp <= -200){
    offset = 0;
    off_temp = -20;
  }else if(temp < 0){
    offset = temp/10 + 19;
    off_temp = offset - 20;
  }else if(temp < 800){
    offset = temp/10 + 20;
    off_temp = offset - 20;
  }else{
    offset = 99;
    off_temp = -79;
  }

  K = (thermistor_curve[offset+1] - thermistor_curve[offset]);
  delta = K * (temp%10) / 10;

  return (thermistor_curve[offset] + delta);
  
}


/* 计算环境温度 */
float calc_evn_temp(u16 val)
{
  float Rx;
  u8 offset;
  float delta;
  
  Rx = (float)val * p_calibration->R_thermistor_plug_up / (65536-val);
  offset = find_thermistor_offset(Rx);
  delta = (Rx - thermistor_curve[offset])/(thermistor_curve[offset+1]-thermistor_curve[offset]);

  return (-20 + offset + delta);
}

/* 计算探头输出电压
 * 公式为：
 *                  Val-Val0              
 *       Vx  =    ———————————�?* K * Vref
 *                   65536          
 *
 *
 *
 *       Val0  =  为基本值， 探头输入�?时测得的值，通过环温标定
 *
 *                              R4                
 *       K 为放大倍数的倒数          ———————�?，通过100°C 标定
 *                           R4 + R5             
 */
float calc_thermopile_voltage(u16 val)
{
  u16 val0 = p_calibration->thermopile_val_env;
  float k = p_calibration->gain;
  
  return ((val-val0) * k * VREF);
}

/*
 * 计算设置环温值对应的热敏电阻�? *
 *   根据公式�? * 
 *               Rx            Val
 *              ———�? =   ———————————�? *               Rp        65536 - val
*/
float calc_R_p(u16 sd_val, int set_val)
{
  float Rx, Rp;

  Rx = get_thermistor_val(set_val);
  Rp = Rx *(65536 - sd_val) / sd_val;
  return Rp;
}

float calc_gain(u16 *sd_val, int set_val)
{
  float env_temp;
  float env_val;
  float v_thermopile;
  float v_sd_calc;

  v_sd_calc = (*sd_val) / 65536.0 * SD16_V_REF;

  v_thermopile = get_thermopile_val(((float)set_val)/10);
  v_thermopile = v_thermopile * p_setting->ems / 100;  //辐射率衰�?

  env_temp = calc_evn_temp(*(sd_val+1));
  env_val = get_thermopile_env_val(env_temp);
  v_thermopile += env_val;

}


float ems_compensate(float val, u8 ems)
{
  return (val * 100 / ems);
}

float env_compensate(float val, float env)
{
  float env_val = get_thermopile_env_val(env);
  
  return (val - env_val);
}

float calc_temperature(u16 *val)
{
  u16 sd16_val[2] = {0};
  float env_temp;
  float v_thermopile;

  v_thermopile = calc_thermopile_voltage(*val);
  v_thermopile = ems_compensate(v_thermopile, p_setting->ems);
  
  env_temp = calc_evn_temp(*(val+1));  
  v_thermopile = env_compensate(v_thermopile, env_temp);

  return get_obj_temp(v_thermopile);
}
