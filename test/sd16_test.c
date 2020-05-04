#include "sd16.h"
#include "display.h"
#include "common.h"

void thermopile_test()
{
	unsigned int val[2];
	while(1){
		delay_ms(1000);
		get_sd16_val(val);
		display_main_int(val[0]/1000);
		display_slave_int(val[0]%1000);
	}
}

void thermistor_test()
{
	unsigned int val[2];
	while(1){
		delay_ms(1000);
		get_sd16_val(val);
		display_main_int(val[1]/1000);
		display_slave_int(val[1]%1000);
	}
}

