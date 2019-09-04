#include "temp.h"

ST_TEMP_SENSOR temp_sensor = {0};
ST_TEMP_RESULT g_temp_result = {0};

void calc_temp_process()
{
  unsigned char i;
  unsigned long therm_temp_value = 0;
  unsigned long env_temp_value = 0;

  
  for(i = 0; i < TEMP_ARRAY_SIZE; i++){
    therm_temp_value += temp_sensor.therm_value[i];
    env_temp_value += temp_sensor.env_temp[i];
  }
  
  g_temp_result.therm_value= (unsigned int)((therm_temp_value * 1.0)/TEMP_ARRAY_SIZE);
  g_temp_result.env_value= (unsigned int)((env_temp_value * 1.0)/TEMP_ARRAY_SIZE);
}

void cal_process(){
  while()


}


