#ifndef __LCD__
#define __LCD__
#include "stdbool.h"

/* LCD 段码中的标志位 */
enum{
  LCD_FLAG_SUN,
  LCD_FLAG_LO,
  LCD_FLAG_HI,
  LCD_FLAG_BAT,
  LCD_FLAG_HOLD,
  LCD_FLAG_MAIN_1,
  LCD_FLAG_MAIN_2,
  LCD_FLAG_MAIN_3,
  LCD_FLAG_MAIN_4,
  LCD_FLAG_MAIN_DOT,
  LCD_FLAG_TEMP_C,
  LCD_FLAG_TEMP_F,
  LCD_FLAG_MAX,
  LCD_FLAG_MIN,
  LCD_FLAG_AVG,
  LCD_FLAG_SLAVE_1,
  LCD_FLAG_SLAVE_2,
  LCD_FLAG_SLAVE_3,
  LCD_FLAG_SLAVE_4,
  LCD_FLAG_SLAVE_DOT,
  LCD_FLAG_EMS_FLAG,
  LCD_FLAG_EMS_EQU,
  LCD_FLAG_EMS_1,
  LCD_FLAG_EMS_2,
  LCD_FLAG_EMS_3,
  LCD_FLAG_EMS_DOT,
  LCD_FLAG_LASER,
  LCD_FLAG_LOCK,
  LCD_FLAG_SET,
  LCD_FLAG_MEM,
};

typedef struct _ST_LCD_{
  union{
    unsigned long flag_mask;
    struct{
      unsigned char SUN         :1;
      unsigned char LO          :1;
      unsigned char HI          :1;
      unsigned char BAT         :1;
      unsigned char HOLD        :1;
      unsigned char MAIN_1      :1;
      unsigned char             :1;
      unsigned char             :1;
      unsigned char             :1;
      unsigned char MAIN_DOT    :1;
      unsigned char TEMP_C      :1;
      unsigned char TEMP_F      :1;
      unsigned char MAX         :1;
      unsigned char MIN         :1;
      unsigned char SLAVE_1     :1;
      unsigned char             :1;
      unsigned char             :1;
      unsigned char             :1;
      unsigned char SLAVE_DOT   :1;
      unsigned char EMS_FLAG    :1;
      unsigned char EMS_EQU     :1;
      unsigned char EMS_1       :1;
      unsigned char             :1;
      unsigned char             :1;
      unsigned char EMS_DOT     :1;
      unsigned char LASER       :1;
      unsigned char LOCK        :1;
      unsigned char SET         :1;
      unsigned char MEM         :1;
    }flag_mask_bit;
  };
  union{
    unsigned long blink_mask;
    struct{
      unsigned char SUN         :1;
      unsigned char LO          :1;
      unsigned char HI          :1;
      unsigned char BAT         :1;
      unsigned char HOLD        :1;
      unsigned char MAIN_1      :1;
      unsigned char MAIN_2      :1;
      unsigned char MAIN_3      :1;
      unsigned char MAIN_4      :1;
      unsigned char MAIN_DOT    :1;
      unsigned char TEMP_C      :1;
      unsigned char TEMP_F      :1;
      unsigned char MAX         :1;
      unsigned char MIN         :1;
      unsigned char SLAVE_1     :1;
      unsigned char SLAVE_2     :1;
      unsigned char SLAVE_3     :1;
      unsigned char SLAVE_4     :1;
      unsigned char SLAVE_DOT   :1;
      unsigned char EMS_FLAG    :1;
      unsigned char EMS_EQU     :1;
      unsigned char EMS_1       :1;
      unsigned char EMS_2       :1;
      unsigned char EMS_3       :1;
      unsigned char EMS_DOT     :1;
      unsigned char LASER       :1;
      unsigned char LOCK        :1;
      unsigned char SET         :1;
      unsigned char MEM         :1;
    }blink_mask_bit;
  };
  unsigned char main_2;
  unsigned char main_3;
  unsigned char main_4;
  unsigned char slave_2;
  unsigned char slave_3;
  unsigned char slave_4;
  unsigned char ems_2;
  unsigned char ems_3;
}ST_LCD, *P_ST_LCD;

extern ST_LCD g_lcd;

void LCD_Init(void);
void LCD_Clear(void);
void LCD_main_display(int number, bool DOT);
void LCD_slave_display(int number, bool DOT);
void LCD_EMS_display(unsigned int number);
void LCD_flag_display(unsigned int flag, bool on);
void LCD_blink_display(unsigned int flag, bool on);
#endif 