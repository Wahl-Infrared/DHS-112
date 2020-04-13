void BT_test()
{
	unsigned long sec,sec_new;
	while(1){
		sec_new=get_system_time_sec();
		if(sec != sec_new){
			sec = sec_new;
			display_cur_temp(sec);
		}
	}
}

