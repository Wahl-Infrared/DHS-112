/******************************************************************************
* UART0 ͨѶ�����ԭ��ͼ����ͼ��ʾ                 
*
*                MSP430FE425
*           +-----------------+
*        /|\|              XIN|-
*         | |                 | 32kHz
*         --|RST          XOUT|-
*           |                 |
*           |        (TXD)P2.4|----------->  //  ----> RXD(2)
*           |                 | 300~115200 bps            
*           |        (RXD)P2.5|<-----------  //  <---- TXD(3)  PC(DB9)
*           |                 |
*           |             GND |------------------------GND(5)
*
******************************************************************************/
#include "io430.h"
#include "lcd.h"
#include "string.h"

#define MAX_STR_LEN 64
#define F_ACLK  (32768)  /*ACLK��Ӧ����Ƶ�ʣ����޸�*/

unsigned char *send_buff;

/****************************************************************************
* ��    �ƣ�UART_Init()
* ��    �ܣ���ʼ�����ڡ������乤��ģʽ�������ʡ�
* ��ڲ�����
*           Baud         ������    (300~115200)
            Parity       ��żУ��λ('n'=��У��  'p'=żУ��  'o'=��У��)
            DatsBits     ����λλ��    (7��8)
            StopBits     ֹͣλλ��    (1��2)
* ���ڲ���������ֵΪ1ʱ��ʾ�����ɹ���Ϊ0��ʾ��������
* ��    ��: UART_Init(9600,'n',8,1) //���9600bps����У�飬8λ���ݣ�1λֹͣλ
            UART_Init(2400,'p',7,2) //���2400bps��żУ�飬7λ���ݣ�2λֹͣλ
****************************************************************************/
char UART_Init(long int Baud,char Parity,char DataBits,char StopBits)
{
  unsigned long int BRCLK;       //�����ʷ�����ʱ��Ƶ��
  int FreqMul,FLLDx,BRDIV,BRMOD; //��Ƶϵ����DCO��Ƶ�������ʷ�Ƶϵ������Ƶβ��
 // int i;
  unsigned char const ModTable[8]={0x00,0x08,0x88,0x2A,0x55,0x6B,0xdd,0xef};
           //��Ƶβ������Ӧ�ĵ���ϵ��(��0~7��"1"���ȷֲ���һ���ֽڵ�8bit��)

  //-------------���ò����ʷ�����ʱ��Դ�������㲨����ʱ��Ƶ��--------------
  UTCTL0 &=~(SSEL0+SSEL1); //���֮ǰ��ʱ������
  if(Baud<=4800)      
  {
    UTCTL0 |= SSEL0;       //����4800�Ĳ����ʣ���ACLK�����͹���
    BRCLK=F_ACLK;          //�����ʷ�����ʱ��Ƶ��=ACLK
  }
  else
  {
    UTCTL0 |= SSEL1;            //����4800�Ĳ����ʣ���SMCLK����֤�ٶ�
    FreqMul=(SCFQCTL&0x7F)+1;   //��ñ�Ƶϵ��
    FLLDx=((SCFI0&0xC0)>>6)+1;  //���DCO��Ƶϵ��(DCOPLUS�������Ķ��ⱶƵ)
    BRCLK=F_ACLK*FreqMul;       //���㲨���ʷ�����ʱ��Ƶ��=ACLK*��Ƶϵ��
    if(FLL_CTL0&DCOPLUS) BRCLK*=FLLDx; //��������DCOPLUS,��Ҫ������ⱶƵ
  }
  //------------------------���ò�����-------------------------   
  if((Baud<300)||(Baud>115200))  return(0);  //�����ʷ�Χ300-115200bps
  BRDIV=BRCLK/Baud;                          //���㲨���ʷ�Ƶϵ��(��������)
  BRMOD=((BRCLK*8)/Baud)%8;                  //���㲨���ʷ�Ƶβ��(������������)
  UBR00 = BRDIV%256;           
  UBR10 = BRDIV/256;                         //��������ϵ��
  UMCTL0 = ModTable[BRMOD];                  //��������ϵ��
  //------------------------����У��λ-------------------------  
  switch(Parity){
    case 'n':case'N': U0CTL&=~PENA;     break;        //��У��
    case 'p':case'P': U0CTL|= PENA+PEV ;break;        //żУ��
    case 'o':case'O': U0CTL|= PENA; U0CTL&=~PEV;break;//��У��  
    default : return(0);               //��������
  }
  //------------------------��������λ-------------------------   
  switch(DataBits)
  {
    case 7:case'7': U0CTL&=~CHAR; break;   //7λ����
    case 8:case'8': U0CTL|= CHAR; break;   //8λ����
    default : return(0);      //��������
  }
  //------------------------����ֹͣλ-------------------------   
  switch(StopBits)
  {
    case 1:case'1': U0CTL&=~SPB; break;  //1λֹͣλ
    case 2:case'2': U0CTL|= SPB; break;  //2λֹͣλ
    default : return(0);     //��������
  }
  P2SEL |= 0x30;              // P2.4,5 = USART0 TXD/RXD
  ME1 |= UTXE0 + URXE0;       // Enable USART0 TXD/RXD
  UCTL0 &= ~SWRST;            // Initialize USART state machine
  IE1 |= URXIE0+UTXIE0;       // Enable USART0 RX & TX interrupt
  return(1); //���óɹ�
}

/****************************************************************************
* ��    �ƣ�UART_RX()
* ��    �ܣ����ڽ����жϣ�ÿ���յ�1�ֽڻᷢ��һ���ж�
****************************************************************************/
#pragma vector=UART0RX_VECTOR
__interrupt void UART_RX (void)
{
//  RxFlag=1;
  /*����������û��жϷ��������룬�罫����ѹ����ջ����*/
  
  int a;
  a = U0RXBUF;
//  LCD_main_display(a,0);
  //__low_power_mode_off_on_exit();
}
/****************************************************************************
* ��    �ƣ�UART_TX()
* ��    �ܣ����ڷ����жϣ�ÿ����1�ֽڻᷢ��һ���ж�
****************************************************************************/
#pragma vector=UART0TX_VECTOR
__interrupt void UART_TX (void)
{
//  TxFlag=1;
//  U0TXBUF = 0x55;
//  return;
  /*����������û��жϷ��������룬�罫���ݴӻ���ȡ����*/
  if(*send_buff){
    U0TXBUF = (unsigned char)*send_buff++;
  }
}

int uart_send_str(unsigned char *src)
{
  unsigned char buff[MAX_STR_LEN] = {0};

  if(strlen(src) > (MAX_STR_LEN - 1)){
     uart_send_str("string is too long:");
     strncpy(buff, src, MAX_STR_LEN-1);
     uart_send_str(buff);
  }

  strncpy(buff, src, MAX_STR_LEN-1);
  send_buff = buff;
  
  U0TXBUF = '\t';

  return 0;
}