void SD16_test()
{
	unsigned int val[2];
	while(1){
		delay_ms(1000);
		get_sd16_val(val);
		display_cur_temp(val[0]/1000);
		display_max_temp(val[0]%1000);
	}
}
