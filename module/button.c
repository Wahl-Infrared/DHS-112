#include "io430.h"
#include "timer_A.h"
#include "gpio.h"
#include "system.h"
#include "button.h"

#define MENU_MASK 0x0c
#define UP_MASK 0x04
#define DOWN_MASK 0x08
#define POWER_MASK 0x80

typedef struct _ST_BUTTON_{
	union{
		unsigned int button_mask;
		struct {
			unsigned char menu	:1;
			unsigned char up		:1;
			unsigned char down	:1;
			unsigned char power	:1;
			unsigned char cal		:1;
		}button_mask_bit;
	};
	unsigned int pressed_count;
	unsigned char menu_state;
	unsigned char up_state;
	unsigned char down_state;
	unsigned char power_state;
	unsigned char cal_state;
}ST_BUTTON, *P_ST_BUTTON;

ST_BUTTON button = {0};

void button_state_inc(unsigned char *state)
{
	switch(*state){
		case BUTTON_STATE_INIT:
			*state = BUTTON_STATE_CLICK;
			break;
		case BUTTON_STATE_CLICK_ACT:
			if(button.pressed_count > 30){
				*state = BUTTON_STATE_PRESSED_1S;
			}
			break;
		case BUTTON_STATE_PRESSED_1S_ACT:
			if(button.pressed_count > 200){
				*state = BUTTON_STATE_PRESSED_3S;
			}else if(button.pressed_count % 10 == 0){
				*state = BUTTON_STATE_PRESSED_1S;
			}
			break;
		case BUTTON_STATE_PRESSED_3S_ACT:
			if(button.pressed_count > 400){
				*state = BUTTON_STATE_PRESSED_5S;
			}else if(button.pressed_count % 10 == 0){
				*state = BUTTON_STATE_PRESSED_3S;
			}
			break;
		case BUTTON_STATE_PRESSED_5S_ACT:
			if(button.pressed_count % 10 == 0){
				*state = BUTTON_STATE_PRESSED_5S;
			}
			break;
		default:
			return;
	}
}

void button_timer_handle(u8 mask)
{
  if(button.button_mask_bit.power){
    if(!(mask & POWER_MASK)){
      button_state_inc(&(button.power_state));
    }else{
     button.power_state = BUTTON_STATE_INIT;
     button.button_mask_bit.power = 0;
    }     
  }else if(button.button_mask_bit.menu){
		if(!(mask & MENU_MASK)){
		  button_state_inc(&(button.menu_state));
		}else{
		 button.menu_state = BUTTON_STATE_INIT;
     button.button_mask_bit.menu = 0;
    }
	}else if(button.button_mask_bit.up){
		if(!(mask & UP_MASK)){
			button_state_inc(&(button.up_state));
		}else{
			button.up_state = BUTTON_STATE_INIT;
			button.button_mask_bit.up = 0;
		}
	}else if(button.button_mask_bit.down){
		if(!(mask & DOWN_MASK)){
			button_state_inc(&(button.down_state));
		}else{
			button.down_state = BUTTON_STATE_INIT;
			button.button_mask_bit.down = 0;
		}
	}else{
		button.button_mask = 0;
	}

	 if (button.button_mask == 0){
		button.pressed_count = 0;
		stop_timer_A();
	}else{
    button.pressed_count++;
	}
}

void button_pressed_handle(unsigned char mask)
{
  if(!(mask & POWER_MASK)){
    button.button_mask_bit.power = 1;
  }else if(!(mask & MENU_MASK)){
    button.button_mask_bit.menu = 1;
  }else if(!(mask & UP_MASK)){
    button.button_mask_bit.up = 1;
  }else if(!(mask & DOWN_MASK)){
    button.button_mask_bit.down = 1;
  }else{
    return;
  }
  start_timer_A(30); /* 30 ms 消抖*/
}


int is_button_pressed(){
  if(button.pressed_count == 0){
    return false;
  }else{
    return true;
  }
}

u8 get_button_state(u8 button_name)
{
  switch(button_name){
    case BUTTON_NAME_MENU:
      return button.menu_state;
    case BUTTON_NAME_UP:
      return button.up_state;
    case BUTTON_NAME_DOWN:
      return button.down_state;
    case BUTTON_NAME_POWER:
      return button.power_state;
  }
  return BUTTON_STATE_NONE;
}

void set_button_state(u8 button_name, u8 state)
{
  switch(button_name)
  {
    case BUTTON_NAME_MENU:
      button.menu_state = state;
    case BUTTON_NAME_UP:
      button.up_state = state;
    case BUTTON_NAME_DOWN:
      button.down_state = state;
    case BUTTON_NAME_POWER:
      button.power_state = state;
  }
}

void button_init()
{
  register_timer_A(button_timer_handle);
  register_P1_ISR(button_pressed_handle);
}

