#include "gpio.h"
#if 0
void button_test()
{
	static int disp=100;
	static int slave = 100;
	while(1){
		if(button.button_mask == 0){
			continue;
		}

	//	delay_ms(100);
		
		if(button.button_mask_bit.menu == 1){
			switch(button.menu_state){
				case BUTTON_STATE_CLICK:
					button.menu_state = BUTTON_STATE_CLICK_ACT;
					display_max_temp(++slave);
					break;
				case BUTTON_STATE_PRESSED_1S:
					button.menu_state = BUTTON_STATE_PRESSED_1S_ACT;
					display_max_temp(++slave);
					break;
				case BUTTON_STATE_PRESSED_3S:
					button.menu_state = BUTTON_STATE_PRESSED_3S_ACT;
					slave += 5;
					display_max_temp(slave);
					break;
				case BUTTON_STATE_PRESSED_5S:
					button.menu_state = BUTTON_STATE_PRESSED_5S_ACT;
					slave += 10;
					display_max_temp(slave);
					break;
				default:
	//				display_max_temp(000);
			}			
		}else if(button.button_mask_bit.up == 1){
			switch(button.up_state){
				case BUTTON_STATE_CLICK:
					button.up_state = BUTTON_STATE_CLICK_ACT;
					display_cur_temp(++disp);
					break;
				case BUTTON_STATE_PRESSED_1S:
					button.up_state = BUTTON_STATE_PRESSED_1S_ACT;
					display_cur_temp(++disp);
					break;
				case BUTTON_STATE_PRESSED_3S:
					button.up_state = BUTTON_STATE_PRESSED_3S_ACT;
					disp += 10;
					display_cur_temp(disp);
					break;
				case BUTTON_STATE_PRESSED_5S:
					button.up_state = BUTTON_STATE_PRESSED_5S_ACT;
					disp += 50;
					display_cur_temp(disp);
					break;
				default:
	//				display_cur_temp(000);
			}		
		}else if(button.button_mask_bit.down == 1){
			switch(button.down_state){
				case BUTTON_STATE_CLICK:
					button.down_state = BUTTON_STATE_CLICK_ACT;
					display_cur_temp(--disp);
					break;
				case BUTTON_STATE_PRESSED_1S:
					button.down_state = BUTTON_STATE_PRESSED_1S_ACT;
					display_cur_temp(--disp);
					break;
				case BUTTON_STATE_PRESSED_3S:
					button.down_state = BUTTON_STATE_PRESSED_3S_ACT;
					disp -= 5;
					display_cur_temp(disp);
					break;
				case BUTTON_STATE_PRESSED_5S:
					button.down_state = BUTTON_STATE_PRESSED_5S_ACT;
					disp -= 10;
					display_cur_temp(disp);
					break;
				default:
//					display_cur_temp(0);
			}		
		}

	}
}

#endif
