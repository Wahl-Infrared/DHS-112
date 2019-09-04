
#include "io430.h"
#include "lcd.h"

#define MAX_SEG_MEM 11                          //�궨�壬����ܸ���
#define MAX_DIG_NUM 4                           //�궨�壬����λ��

/*�궨�壬�����a-g���ζ�Ӧ�ı��أ�����Ӳ��ֻ�øĶ�����8��*/
#define d        0x01                            //  AAAA
#define e        0x02                            // F    B
#define g        0x04                            // F    B
#define f        0x08                            //  GGGG
#define DOT_SEG  0x10                            // E    C
#define c        0x20                            // E    C
#define b        0x40                            //  DDDD
#define a        0x80
#define BAT_SEG  0x10
#define HOLD_SEG 0x10
#define LOCK_SEG 0x10
#define SET_SEG  0x10
#define MEM_SEG  0x10
#define HI_SEG   0x08
#define LO_SEG   0x04
#define SUN_SEG  0x02
#define TEMP_F_SEG    0x04
#define TEMP_C_SEG    0x08
#define LASER_SEG     0x10
#define MIN_SEG       0x20
#define MAX_SEG       0x40
#define MAIN_1_SEG    0x01
#define SLAVE_1_SEG   0x80
#define EMS_DOT_SEG   0x01
#define EMS_1_SEG     0x02
#define EMS_EQU_SEG   0x04
#define EMS_FLAG_SEG  0x08

enum{
  CHAR_0 = 0,
  CHAR_1,
  CHAR_2,
  CHAR_3,
  CHAR_4,
  CHAR_5,
  CHAR_6,
  CHAR_7,
  CHAR_8,
  CHAR_9,
  CHAR_A,
  CHAR_B,
  CHAR_C,
  CHAR_D,
  CHAR_E,
  CHAR_F,
  CHAR_G,
  CHAR_H,
  CHAR_I,
  CHAR_J,
  CHAR_K,
  CHAR_L,
  CHAR_M,
  CHAR_N,
  CHAR_n,
  CHAR_o,
  CHAR_O,
  CHAR_P,
  CHAR_Q,
  CHAR_r,
  CHAR_S,
  CHAR_t,
  CHAR_T,
  CHAR_U,
  CHAR_v,
  CHAR_W,
  CHAR_Y,
  CHAR_Z,
  CHAR_DASH,
  CHAR_QUOTE,
  CHAR_NONE
};

/*�ú궨���Զ����ɶ�����ܺõ�д����ֵ��ѧϰ*/
/*����Ӳ��������д�����*/
const char LCD_Tab[] = {
  a + b + c + d + e + f,                        // 0, Displays "0"
  b + c,                                        // 1, Displays "1"
  a + b + d + e + g,                            // 2, Displays "2"
  a + b + c + d + g,                            // 3, Displays "3"
  b + c + f + g,                                // 4, Displays "4"
  a + c + d + f +g,                             // 5, Displays "5"
  a + c + d + e + f + g,                        // 6, Displays "6"
  a + b + c,                                    // 7, Displays "7"
  a + b + c + d + e + f + g,                    // 8, Displays "8"
  a + b + c + d + f + g,                        // 9, Displays "9"
  a + b + c + e + f + g,                        // 10, Displays "A"
  c + d + e + f + g,                            // 11, Displays "B"  
  a + d + e + f,                                // 12, Displays "C"
  b + c + d + e + g,                            // 13, Displays "D"
  a + d + e + f + g,                            // 14, Displays "E"
  a + e + f + g,                                // 15, Displays "F"
  a + c + d + e + f,                            // 16, Displays "G"
  b + c + e + f + g,                            // 17, Displays "H"  
  e + f,                                        // 18, Displays "I"
  b + c + d + e,                                // 19, Displays "J"
  b + d + e + f + g,                            // 20, Displays "K"
  d + e + f,                                    // 21, Displays "L"  
  a + c + e + g,                                // 22, Displays "M"
  a + b + c + e + f,                            // 23, Displays "N"   
  c + e + g,                                    // 24, Displays "n"
  c + d + e + g,                                // 25, Displays "o"
  a + b + c + d + e + f,                        // 26, Displays "O"
  a + b + e + f + g,                            // 27, Displays "P"
  a + b + c + f + g,                            // 28, Displays "Q"
  e + g,                                        // 29, Displays "r"
  a + c + d + f +g,                             // 30, Displays "S"
  d + e + f + g,                                // 31, Displays "t"
  a + e + f ,                                   // 32, Displays "T"
  b + c + d + e + f,                            // 33, Displays "U"
  c + d + e,                                    // 34, Displays "v"
  b + d + f + g,                                // 35, Displays "W"
  b + c + d + f + g,                            // 36, Displays "Y"
  a + b + d + e + g,                            // 37, Displays "Z"
  g,                                            // 38, Displays "-"
  b,                                            // 39, Displays "'"  
  0                                             // 40, Displays " "
};

#undef a
#undef b
#undef c
#undef d
#undef e
#undef f
#undef g

/*ȫ�ֱ�������*/

ST_LCD g_lcd;

#if 0

/*��˸��־*/


typedef struct{
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
  unsigned char MENU        :1;
}_st_lcd_flags;

_st_lcd_flags g_lcd_flags, g_lcd.flag_mask_bit;



/****************************************************
* �ݴ�LCDM��ֵ��������˸��ʾ
*
* Ϊ�˷���������� 8 �� 7��������еĵ�������λ
* BAT, MAIN_DOT, HOLD, LOCK, SLAVE_DOT, SET, MENU 
* �ֱ𱣴��� LCDM1_value ��3λ �� LCDM9_value ��4λ��
* ������ΰ����Զ�Ӧλ�ñ���
*****************************************************/
union{
  unsigned char LCDM1_value;
  struct{
    unsigned char MAIN_1        :1;
    unsigned char SUN           :1;
    unsigned char LO            :1;
    unsigned char HI            :1;
    unsigned char BAT           :1;
    unsigned char MAIN_DOT      :1;
    unsigned char HOLD          :1;

  }g_lcd.flag_mask_bit;
};
unsigned char LCDM2_value;
unsigned char LCDM3_value;
unsigned char LCDM4_value;
union{
  unsigned char LCDM5_value;
  struct{
    unsigned char UNUSED_1      :1;
    unsigned char UNUSED_2      :1;    
    unsigned char TEMP_F        :1;
    unsigned char TEMP_C        :1;
    unsigned char LASER         :1;
    unsigned char MIN           :1;
    unsigned char MAX           :1;
    unsigned char SLAVE_1       :1;
  }g_lcd.flag_mask_bit;
};
unsigned char LCDM6_value;
unsigned char LCDM7_value;
unsigned char LCDM8_value;
union{
  unsigned char LCDM9_value;
  struct{
    unsigned char EMS_DOT       :1;
    unsigned char EMS_1         :1;
    unsigned char EMS_EQU       :1;
    unsigned char EMS_FLAG      :1;
    unsigned char LOCK          :1;
    unsigned char SLAVE_DOT     :1;
    unsigned char SET           :1;
    unsigned char MENU          :1;
  }g_lcd.flag_mask_bit;
};
unsigned char LCDM10_value;
unsigned char LCDM11_value;
#endif

/****************************************************************************
* ��    �ƣ�LCD_Init()
* ��    �ܣ���ʼ��LCD��ʾ����
* ��ڲ�������
* ���ڲ�������
* ˵    ��: ��������LCD����֮ǰ����Ҫ���øú�������LCD������
****************************************************************************/
void LCD_Init()
{
  /*  ���� */
  LCD_Clear();
  LCDCTL = 0x00;
  LCDCTL  = LCDSG0_3 + LCD4MUX + LCDON;         // LCDģʽ:4mux LCD, segs0-15
  BTCTL = BT_fLCD_DIV64 + BT_ADLY_1000;         // ���� LCD ˢ���ʣ� LCD 1s ��˸
  /*ˢ����Խ������Խ�ͣ����ٶ�̫��LCD����˸*/

  IE2 |= BTIE;                                  //����basic timer �жϣ�������˸��ʾ
}

/****************************************************************************
* ��    �ƣ�__LCD_blink()
* ��    �ܣ�basic timer �ж���ں���������LCD��˸��
* ��ڲ�������
* ���ڲ�������
* ˵    ��: 1s��˸һ�Σ� ��������ѡ�б�ʶ
****************************************************************************/
#pragma vector = BASICTIMER_VECTOR
__interrupt void __LCD_blink(void)
{
  static bool blink = false;
  
  blink = !blink;

  if(blink){
    if(g_lcd.blink_mask_bit.SUN){
      LCDM1 |= SUN_SEG;
    }
    
    if(g_lcd.blink_mask_bit.LO){
      LCDM1 |= LO_SEG;
    }
    
    if(g_lcd.blink_mask_bit.HI){
      LCDM1 |= HI_SEG;
    }
    
    if(g_lcd.blink_mask_bit.MAIN_1){
      LCDM1 |= MAIN_1_SEG;
    }

    if(g_lcd.blink_mask_bit.BAT){
      LCDM2 |= BAT_SEG;
    }
    
    if(g_lcd.blink_mask_bit.MAIN_2){
      LCDM2 = LCDM2 & BAT_SEG | g_lcd.main_2; 
    }
    
    if(g_lcd.blink_mask_bit.MAIN_DOT){
      LCDM3 |= DOT_SEG;
    }
    
    if(g_lcd.blink_mask_bit.MAIN_3){
      LCDM3 = LCDM3 & DOT_SEG | g_lcd.main_3; 
    } 

    if(g_lcd.blink_mask_bit.HOLD){
      LCDM4 |= HOLD_SEG;
    }
    
    if(g_lcd.blink_mask_bit.MAIN_4){
      LCDM4 = LCDM4 & HOLD_SEG | g_lcd.main_4; 
    }

    if(g_lcd.blink_mask_bit.TEMP_C){
      LCDM5 |= TEMP_C_SEG;
    }
    
    if(g_lcd.blink_mask_bit.TEMP_F){
      LCDM5 |= TEMP_F_SEG;
    }
    
    if(g_lcd.blink_mask_bit.MAX){
      LCDM5 |= MAX_SEG;
    }

    if(g_lcd.blink_mask_bit.MIN){
      LCDM5 |= MIN_SEG;
    }

    if(g_lcd.blink_mask_bit.LASER){
      LCDM5 |= LASER_SEG;
    }

    if(g_lcd.blink_mask_bit.SLAVE_1){
      LCDM5 |= SLAVE_1_SEG;
    }

    if(g_lcd.blink_mask_bit.LOCK){
      LCDM6 |= LOCK_SEG;
    }
    
    if(g_lcd.blink_mask_bit.SLAVE_2){
      LCDM6 = LCDM6 & LOCK_SEG | g_lcd.slave_2; 
    }
    
    if(g_lcd.blink_mask_bit.SLAVE_DOT){
      LCDM7 |= DOT_SEG;
    }
    
    if(g_lcd.blink_mask_bit.SLAVE_3){
      LCDM7 = LCDM7 & DOT_SEG | g_lcd.slave_3; 
    } 

    if(g_lcd.blink_mask_bit.SET){
      LCDM8 |= SET_SEG;
    }
    
    if(g_lcd.blink_mask_bit.SLAVE_4){
      LCDM8 = LCDM8 & SET_SEG | g_lcd.slave_4; 
    }  
    
    if(g_lcd.blink_mask_bit.EMS_FLAG){
      LCDM9 |= EMS_FLAG_SEG;
    }
    
    if(g_lcd.blink_mask_bit.EMS_EQU){
      LCDM9 |= EMS_EQU_SEG;
    }

    if(g_lcd.blink_mask_bit.EMS_1){
      LCDM9 |= EMS_1_SEG;
    }
    
    if(g_lcd.blink_mask_bit.EMS_DOT){
      LCDM9 |= EMS_DOT_SEG;
    }

   if(g_lcd.blink_mask_bit.MEM){
      LCDM10 |= MEM_SEG;
    }
    
    if(g_lcd.blink_mask_bit.EMS_2){
        LCDM10 = LCDM10 & MEM_SEG | g_lcd.ems_2; 
    } 

    if(g_lcd.blink_mask_bit.EMS_3){
        LCDM11 = g_lcd.ems_3; 
    }
  }else{
    if(g_lcd.blink_mask_bit.SUN){
      LCDM1 &= ~SUN_SEG;
    }

    if(g_lcd.blink_mask_bit.LO){
      LCDM1 &= ~LO_SEG;
    }

    if(g_lcd.blink_mask_bit.HI){
      LCDM1 &= ~HI_SEG;
    }

    if(g_lcd.blink_mask_bit.MAIN_1){
      LCDM1 &= ~MAIN_1_SEG;
    }

    if(g_lcd.blink_mask_bit.BAT){
      LCDM2 &= ~BAT_SEG;
    }

    if(g_lcd.blink_mask_bit.MAIN_2){
      LCDM2 &= BAT_SEG; 
    }

    if(g_lcd.blink_mask_bit.MAIN_DOT){
      LCDM3 &= ~DOT_SEG;
    }

    if(g_lcd.blink_mask_bit.MAIN_3){
      LCDM3 &= DOT_SEG;
    } 

    if(g_lcd.blink_mask_bit.HOLD){
      LCDM4 &= ~HOLD_SEG;
    }

    if(g_lcd.blink_mask_bit.MAIN_4){
      LCDM4 &= HOLD_SEG;
    }

    if(g_lcd.blink_mask_bit.TEMP_C){
      LCDM5 &= ~TEMP_C_SEG;
    }

    if(g_lcd.blink_mask_bit.TEMP_F){
      LCDM5 &= ~TEMP_F_SEG;
    }

    if(g_lcd.blink_mask_bit.MAX){
      LCDM5 &= ~MAX_SEG;
    }

    if(g_lcd.blink_mask_bit.MIN){
      LCDM5 &= ~MIN_SEG;
    }

    if(g_lcd.blink_mask_bit.LASER){
      LCDM5 &= ~LASER_SEG;
    }

    if(g_lcd.blink_mask_bit.SLAVE_1){
      LCDM5 &= ~SLAVE_1_SEG;
    }

    if(g_lcd.blink_mask_bit.LOCK){
      LCDM6 &= ~LOCK_SEG;
    }

    if(g_lcd.blink_mask_bit.SLAVE_2){
      LCDM6 &= LOCK_SEG;
    }

    if(g_lcd.blink_mask_bit.SLAVE_DOT){
      LCDM7 &= ~DOT_SEG;
    }

    if(g_lcd.blink_mask_bit.SLAVE_3){
      LCDM7 &= DOT_SEG;
    } 

    if(g_lcd.blink_mask_bit.SET){
      LCDM8 &= ~SET_SEG;
    }

    if(g_lcd.blink_mask_bit.SLAVE_4){
      LCDM8 &= SET_SEG;
    }  

    if(g_lcd.blink_mask_bit.EMS_FLAG){
      LCDM9 &= ~EMS_FLAG_SEG;
    }

    if(g_lcd.blink_mask_bit.EMS_EQU){
      LCDM9 &= ~EMS_EQU_SEG;
    }

    if(g_lcd.blink_mask_bit.EMS_1){
      LCDM9 &= ~EMS_1_SEG;
    }

    if(g_lcd.blink_mask_bit.EMS_DOT){
      LCDM9 &= ~EMS_DOT_SEG;
    }

    if(g_lcd.blink_mask_bit.MEM){
      LCDM10 &= ~MEM_SEG;
    }

    if(g_lcd.blink_mask_bit.EMS_2){
      LCDM10 &= MEM_SEG; 
    } 

    if(g_lcd.blink_mask_bit.EMS_3){
      LCDM11 = CHAR_NONE; 
    }
  }
}

/****************************************************************************
* ��    �ƣ�LCD_main_display()
* ��    �ܣ���LCD����������ʾ -99 ~ 1999 ���ڵ�����
* ��ڲ�����number:��ʾ��ֵ  (-99~1999)
            DOT   : 0 ����ʾС����
                    1 ��ʾС����
* ���ڲ�������
* ��    ��: LCD_main_display(235,0); ��ʾ���: 123 (��С����)
            LCD_main_display(1235,1); ��ʾ���:123.5 (��ʾС����)
* ˵    ��: ��LCD������ƣ� ������ʾ��Χ���Ϊ-99�� �����С����
            ��Ϊ-9.9
****************************************************************************/
void LCD_main_display(int number, bool DOT)
{

  g_lcd.flag_mask_bit.MAIN_DOT = DOT;

  if(number < -99){                       //С��-99����ʾLo
    g_lcd.flag_mask_bit.MAIN_1 = 0;
    g_lcd.main_2 = LCD_Tab[CHAR_NONE];
    g_lcd.main_3 = LCD_Tab[CHAR_L];
    g_lcd.main_4 = LCD_Tab[CHAR_o];
  }else if(number < -9){                  // -99 ~ -10
    number = -number;
    g_lcd.flag_mask_bit.MAIN_1 = 0;
    g_lcd.main_2 = LCD_Tab[CHAR_DASH];
    g_lcd.main_3 = LCD_Tab[number/10];
    g_lcd.main_4 = LCD_Tab[number%10];
  }else if(number < 0){                   // -9 ~ 0
    number = -number;
    g_lcd.flag_mask_bit.MAIN_1 = 0;
    if(DOT){                              //��С���㣬��λ��0
      g_lcd.main_2 = LCD_Tab[CHAR_DASH];
      g_lcd.main_3 = LCD_Tab[CHAR_0];
    }else{                                // ����С����
      g_lcd.main_2 = LCD_Tab[CHAR_NONE];
      g_lcd.main_3 = LCD_Tab[CHAR_DASH];
    }   
    g_lcd.main_4 = LCD_Tab[number];
  }else if(number < 10){                  // 0 ~ 9
    g_lcd.flag_mask_bit.MAIN_1 = 0;
    g_lcd.main_2 = LCD_Tab[CHAR_NONE];          
    g_lcd.main_3 = DOT?LCD_Tab[0]:LCD_Tab[CHAR_NONE];  //��С���㣬��λ��0
    g_lcd.main_4 = LCD_Tab[number];
  }else if(number < 100){               // 10 ~ 99
    g_lcd.flag_mask_bit.MAIN_1 = 0;
    g_lcd.main_2 = LCD_Tab[CHAR_NONE];
    g_lcd.main_3 = LCD_Tab[number/10];
    g_lcd.main_4 = LCD_Tab[number%10];
  }else if(number < 1000){              // 100 ~ 999
    g_lcd.flag_mask_bit.MAIN_1 = 0;
    g_lcd.main_2 = LCD_Tab[number/100];
    g_lcd.main_3 = LCD_Tab[(number/10)%10];
    g_lcd.main_4 = LCD_Tab[number%10];
  }else if(number < 2000){              // 1000 ~ 1999
    g_lcd.flag_mask_bit.MAIN_1 = 1;
    g_lcd.main_2 = LCD_Tab[(number/100)%10];
    g_lcd.main_3 = LCD_Tab[(number/10)%10];
    g_lcd.main_4 = LCD_Tab[number%10];
  }else{                              //����1999����ʾ ��HI ��
    g_lcd.flag_mask_bit.MAIN_1 = 0;
    g_lcd.main_2 = LCD_Tab[CHAR_NONE];;
    g_lcd.main_3 = LCD_Tab[CHAR_H];
    g_lcd.main_4 = LCD_Tab[CHAR_I];
  }

  /*ˢ�� LCD memory*/
  if(g_lcd.flag_mask_bit.MAIN_1){
    LCDM1 |= MAIN_1_SEG;
  }else{
    LCDM1 &= ~MAIN_1_SEG;
  }

  if(g_lcd.flag_mask_bit.BAT){
    LCDM2 = g_lcd.main_2 | BAT_SEG;
  }else{
    LCDM2 = g_lcd.main_2;
  }

  if(g_lcd.flag_mask_bit.MAIN_DOT){
    LCDM3 = g_lcd.main_3 | DOT_SEG;
  }else{
    LCDM3 = g_lcd.main_3;
  }

  if(g_lcd.flag_mask_bit.HOLD){
    LCDM4 = g_lcd.main_4 | HOLD_SEG;
  }else{
    LCDM4 = g_lcd.main_4;
  }
}

/****************************************************************************
* ��    �ƣ�LCD_slave_display()
* ��    �ܣ���LCD���ܣ����/��С����������ʾ-99 ~ 1999 ���ڵ�����
* ��ڲ�����number:��ʾ��ֵ  (-99~1999)
            DOT   : 0 ����ʾС����
                    1 ��ʾС����
* ���ڲ�������
* ��    ��: LCD_slave_display(235,0); ��ʾ���: 123 (��С����)
            LCD_slave_display(1235,1); ��ʾ���:123.5 (��ʾС����)
* ˵    ��: ��LCD������ƣ� ������ʾ��Χ���Ϊ-99�� �����С����
            ��Ϊ-9.9
****************************************************************************/
void LCD_slave_display(int number, bool DOT)
{
  g_lcd.flag_mask_bit.SLAVE_DOT = DOT;

  if(number < -99){                     //С��-99����ʾLo
    g_lcd.flag_mask_bit.SLAVE_1 = 0;
    g_lcd.slave_2 = LCD_Tab[CHAR_NONE];
    g_lcd.slave_3 = LCD_Tab[CHAR_L];
    g_lcd.slave_4 = LCD_Tab[CHAR_o];
  }else if(number < -9){                // -99 ~ -10
    number = -number;
    g_lcd.flag_mask_bit.SLAVE_1 = 0;
    g_lcd.slave_2 = LCD_Tab[CHAR_DASH];
    g_lcd.slave_3 = LCD_Tab[number/10];
    g_lcd.slave_4 = LCD_Tab[number%10];
  }else if(number < 0){                 // -9 ~ 0
    number = -number;
    g_lcd.flag_mask_bit.SLAVE_1 = 0;
    if(DOT){
      g_lcd.slave_2 = LCD_Tab[CHAR_DASH]; 
      g_lcd.slave_3 = LCD_Tab[CHAR_0];          //��С���㣬��λ��0
    }else{
      g_lcd.slave_2 = LCD_Tab[CHAR_NONE];
      g_lcd.slave_3 = LCD_Tab[CHAR_DASH];       // ����С����
    }   
    g_lcd.slave_4 = LCD_Tab[number];
  }else if(number < 10){                 // 0 ~ 9
    g_lcd.flag_mask_bit.SLAVE_1 = 0;
    g_lcd.slave_2 = LCD_Tab[CHAR_NONE];          
    g_lcd.slave_3 = DOT>0? LCD_Tab[CHAR_0] : LCD_Tab[CHAR_NONE]; 
    g_lcd.slave_4 = LCD_Tab[number];
  }else if(number < 100){               // 10 ~ 99
    g_lcd.flag_mask_bit.SLAVE_1 = 0;
    g_lcd.slave_2 = LCD_Tab[CHAR_NONE];
    g_lcd.slave_3 = LCD_Tab[number/10];
    g_lcd.slave_4 = LCD_Tab[number%10];
  }else if(number < 1000){              // 100 ~ 999
    g_lcd.flag_mask_bit.SLAVE_1 = 0;
    g_lcd.slave_2 = LCD_Tab[number/100];
    g_lcd.slave_3 = LCD_Tab[(number/10)%10];
    g_lcd.slave_4 = LCD_Tab[number%10];
  }else if(number < 2000){              // 1000 ~ 1999
    g_lcd.flag_mask_bit.SLAVE_1 = 1;
    g_lcd.slave_2 = LCD_Tab[(number/100)%10];
    g_lcd.slave_3 = LCD_Tab[(number/10)%10];
    g_lcd.slave_4 = LCD_Tab[number%10];
  }else{                              //����1999����ʾ ��HI ��
    g_lcd.flag_mask_bit.SLAVE_1 = 0;
    g_lcd.slave_2 = LCD_Tab[CHAR_NONE];
    g_lcd.slave_3 = LCD_Tab[CHAR_H];
    g_lcd.slave_4 = LCD_Tab[CHAR_I];
  }

  /*ˢ�� LCD memory*/
  if(g_lcd.flag_mask_bit.SLAVE_1){
    LCDM5 |= SLAVE_1_SEG;
  }else{
    LCDM5 &= ~SLAVE_1_SEG;
  }

  if(g_lcd.flag_mask_bit.BAT){
    LCDM6 = g_lcd.slave_2 | LOCK_SEG;
  }else{
    LCDM6 = g_lcd.slave_2;
  }

  if(g_lcd.flag_mask_bit.SLAVE_DOT){
    LCDM7 = g_lcd.slave_3 | DOT_SEG;
  }else{
    LCDM7 = g_lcd.slave_3;
  }

  if(g_lcd.flag_mask_bit.SET){
    LCDM8 = g_lcd.slave_4 | SET_SEG;
  }else{
    LCDM8 = g_lcd.slave_4;
  }
}

/****************************************************************************
* ��    �ƣ�LCD_EMS_display()
* ��    �ܣ�LCD��������ʾ��
* ��ڲ�����number:��ʾ��ֵ  (0~100)
* ���ڲ�������
* ��    ��: LCD_EMS_display(56); ��ʾ���: .56
            LCD_EMS_display(100); ��ʾ���:1.00
* ˵    ��: ���������Ϊ1��С����ʼ����?****************************************************************************/
void LCD_EMS_display(unsigned int number)
{
  if(number > 100){
    g_lcd.flag_mask_bit.EMS_1 = 0;
    g_lcd.ems_2= LCD_Tab[CHAR_DASH];
    g_lcd.ems_3 = LCD_Tab[CHAR_DASH];
  }else if(number == 100){
    g_lcd.flag_mask_bit.EMS_1 = 1;
    g_lcd.ems_2 = LCD_Tab[CHAR_0];
    g_lcd.ems_3 = LCD_Tab[CHAR_0];
  }else{
    g_lcd.flag_mask_bit.EMS_1 = 0;
    g_lcd.ems_2 = LCD_Tab[number/10];
    g_lcd.ems_3 = LCD_Tab[number%10];
  }

  /*ˢ�� LCD memory*/
  if(g_lcd.flag_mask_bit.EMS_1){
    LCDM9 |= EMS_1_SEG;
  }else{
    LCDM9 &= ~EMS_1_SEG;
  }

  if(g_lcd.flag_mask_bit.MEM){
    LCDM10 = g_lcd.ems_2 | MEM_SEG;
  }else{
    LCDM10 = g_lcd.ems_2;
  }

  LCDM11 = g_lcd.ems_3;
}

/****************************************************************************
* ��    �ƣ�LCD_flag_display()
* ��    �ܣ����ֱ�־��ʾ��
* ��ڲ�����flag:��ʾ��־  (ö������ LCD_FLAG_XXX)
*           on: 0 �ر���ʾ
*               1 ����ʾ
* ���ڲ�������
* ��    ��: LCD_flag_display(LCD_FLAG_HOLD, 1); ��ʾ���: hold ����
* ˵    ��: 
****************************************************************************/
void LCD_flag_display(unsigned int flag, bool on)
{
  switch(flag){
  case LCD_FLAG_SUN:
    g_lcd.flag_mask_bit.SUN = on;
    if(on){
      LCDM1 |= SUN_SEG;
    }else{
      LCDM1 &= ~SUN_SEG;
    }
    break;
  case LCD_FLAG_LO:
    g_lcd.flag_mask_bit.LO = on;
    if(on){
      LCDM1 |= LO_SEG;
    }else{
      LCDM1 &= ~LO_SEG;
    }
    break;
  case LCD_FLAG_HI:
    g_lcd.flag_mask_bit.HI = on;
    if(on){
      LCDM1 |= HI_SEG;
    }else{
      LCDM1 &= ~HI_SEG;
    }
    break;
  case LCD_FLAG_BAT:
    g_lcd.flag_mask_bit.BAT = on;
    if(on){
      LCDM2 |= BAT_SEG;
    }else{
      LCDM2 &= ~BAT_SEG;
    }
    break;
  case LCD_FLAG_HOLD:
    g_lcd.flag_mask_bit.HOLD = on;
    if(on){
      LCDM4 |= HOLD_SEG;
    }else{
      LCDM4 &= ~HOLD_SEG;
    }
    break;
  case LCD_FLAG_TEMP_C:
    g_lcd.flag_mask_bit.TEMP_C = on;
    if(on){
      LCDM5 |= TEMP_C_SEG;
    }else{
      LCDM5 &= ~TEMP_C_SEG;
    }
    break;
  case LCD_FLAG_TEMP_F:
    g_lcd.flag_mask_bit.TEMP_F = on;
    if(on){
      LCDM5 |= TEMP_F_SEG;
    }else{
      LCDM5 &= ~TEMP_F_SEG;
    }
    break;
  case LCD_FLAG_MAX:
    g_lcd.flag_mask_bit.MAX = on;
    if(on){
      LCDM5 |= MAX_SEG;
    }else{
      LCDM5 &= ~MAX_SEG;
    }
    break;
  case LCD_FLAG_MIN:
    g_lcd.flag_mask_bit.MIN = on;
    if(on){
      LCDM5 |= MIN_SEG;
    }else{
      LCDM5 &= ~MIN_SEG;
    }
    break;
  case LCD_FLAG_LASER:
    g_lcd.flag_mask_bit.LASER = on;
    if(on){
      LCDM5 |= LASER_SEG;
    }else{
      LCDM5 &= ~LASER_SEG;
    }
    break;
  case LCD_FLAG_LOCK:
    g_lcd.flag_mask_bit.LOCK = on;
    if(on){
      LCDM6 |= LOCK_SEG;
    }else{
      LCDM6 &= ~LOCK_SEG;
    }
    break;
  case LCD_FLAG_SET:
    g_lcd.flag_mask_bit.SET = on;
    if(on){
      LCDM8 |= SET_SEG;
    }else{
      LCDM8 &= ~SET_SEG;
    }
    break;
  case LCD_FLAG_EMS_FLAG:
     g_lcd.flag_mask_bit.EMS_FLAG = on;
    if(on){
      LCDM9 |= EMS_FLAG_SEG;
    }else{
      LCDM9 &= ~EMS_FLAG_SEG;
    }
    break;
  case LCD_FLAG_MEM:                   // MEM ��־����LCDM9_value bit 7, ������ʾ������LCDM10 bit 4
    g_lcd.flag_mask_bit.MEM = on;
    if(on){
      LCDM10 |= MEM_SEG;
    }else{
      LCDM10 &= ~MEM_SEG;
    }
    break;

  default:
    break;
  }
}

/****************************************************************************
* ��    �ƣ�LCD_blink_display()
* ��    �ܣ���˸��ʾ���ֶΡ�
* ��ڲ�����flag:��ʾ�ֶ�  (ö������ LCD_FLAG_XXX)
*           on: 0 �ر���˸��ʾ
*               1 ����˸��ʾ
* ���ڲ�������
* ��    ��: LCD_blink_display(LCD_FLAG_HOLD, 1); ��ʾ���: hold ��˸
* ˵    ��: Ŀǰ��֧�� 1s ��˸
****************************************************************************/
void LCD_blink_display(unsigned int flag, bool on)
{
  if(on){
    g_lcd.blink_mask |= (unsigned long)1<<flag;
  }else{
    g_lcd.blink_mask &= ~(unsigned long)(1<<flag);
  }
}

/****************************************************************************
* ��    �ƣ�LCD_Clear()
* ��    ��: ���LCD��ʾ
* ��ڲ�������
* ���ڲ�������
* ˵    ��: ���øú�����LCD��ʾ������������ر�LCDģ�顣
****************************************************************************/
void LCD_Clear()
{ 
  char i;
  char *pLCD;

  pLCD = (char *)&g_lcd;
  for (i = 0; i < sizeof(g_lcd); i++)        // Clear g_lcd
  {
    *pLCD++ = 0;
  }

  pLCD = (char *)&LCDM1;                     // ȡLCDM1�Ĵ���(���λ)�ĵ�ַ
  for (i = 0; i < MAX_SEG_MEM; i++)          // Clear LCD memory
  {
    *pLCD++ = 0;                    //����
  }
}
