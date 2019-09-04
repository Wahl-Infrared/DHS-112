/******************************************************************************
* UART0 通讯组件，原理图如下图所示                 
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
#define F_ACLK  (32768)  /*ACLK对应晶体频率，勿修改*/

unsigned char *send_buff;

/****************************************************************************
* 名    称：UART_Init()
* 功    能：初始化串口。设置其工作模式及波特率。
* 入口参数：
*           Baud         波特率    (300~115200)
            Parity       奇偶校验位('n'=无校验  'p'=偶校验  'o'=奇校验)
            DatsBits     数据位位数    (7或8)
            StopBits     停止位位数    (1或2)
* 出口参数：返回值为1时表示初化成功，为0表示参数出错
* 范    例: UART_Init(9600,'n',8,1) //设成9600bps，无校验，8位数据，1位停止位
            UART_Init(2400,'p',7,2) //设成2400bps，偶校验，7位数据，2位停止位
****************************************************************************/
char UART_Init(long int Baud,char Parity,char DataBits,char StopBits)
{
  unsigned long int BRCLK;       //波特率发生器时钟频率
  int FreqMul,FLLDx,BRDIV,BRMOD; //倍频系数、DCO倍频、波特率分频系数、分频尾数
 // int i;
  unsigned char const ModTable[8]={0x00,0x08,0x88,0x2A,0x55,0x6B,0xdd,0xef};
           //分频尾数所对应的调制系数(将0~7个"1"均匀分布在一个字节的8bit中)

  //-------------设置波特率发生器时钟源，并计算波特率时钟频率--------------
  UTCTL0 &=~(SSEL0+SSEL1); //清除之前的时钟设置
  if(Baud<=4800)      
  {
    UTCTL0 |= SSEL0;       //低于4800的波特率，用ACLK，降低功耗
    BRCLK=F_ACLK;          //波特率发生器时钟频率=ACLK
  }
  else
  {
    UTCTL0 |= SSEL1;            //高于4800的波特率，用SMCLK，保证速度
    FreqMul=(SCFQCTL&0x7F)+1;   //获得倍频系数
    FLLDx=((SCFI0&0xC0)>>6)+1;  //获得DCO倍频系数(DCOPLUS所带来的额外倍频)
    BRCLK=F_ACLK*FreqMul;       //计算波特率发生器时钟频率=ACLK*倍频系数
    if(FLL_CTL0&DCOPLUS) BRCLK*=FLLDx; //若开启了DCOPLUS,还要计算额外倍频
  }
  //------------------------设置波特率-------------------------   
  if((Baud<300)||(Baud>115200))  return(0);  //波特率范围300-115200bps
  BRDIV=BRCLK/Baud;                          //计算波特率分频系数(整数部分)
  BRMOD=((BRCLK*8)/Baud)%8;                  //计算波特率分频尾数(除不尽的余数)
  UBR00 = BRDIV%256;           
  UBR10 = BRDIV/256;                         //整数部分系数
  UMCTL0 = ModTable[BRMOD];                  //余数部分系数
  //------------------------设置校验位-------------------------  
  switch(Parity){
    case 'n':case'N': U0CTL&=~PENA;     break;        //无校验
    case 'p':case'P': U0CTL|= PENA+PEV ;break;        //偶校验
    case 'o':case'O': U0CTL|= PENA; U0CTL&=~PEV;break;//奇校验  
    default : return(0);               //参数错误
  }
  //------------------------设置数据位-------------------------   
  switch(DataBits)
  {
    case 7:case'7': U0CTL&=~CHAR; break;   //7位数据
    case 8:case'8': U0CTL|= CHAR; break;   //8位数据
    default : return(0);      //参数错误
  }
  //------------------------设置停止位-------------------------   
  switch(StopBits)
  {
    case 1:case'1': U0CTL&=~SPB; break;  //1位停止位
    case 2:case'2': U0CTL|= SPB; break;  //2位停止位
    default : return(0);     //参数错误
  }
  P2SEL |= 0x30;              // P2.4,5 = USART0 TXD/RXD
  ME1 |= UTXE0 + URXE0;       // Enable USART0 TXD/RXD
  UCTL0 &= ~SWRST;            // Initialize USART state machine
  IE1 |= URXIE0+UTXIE0;       // Enable USART0 RX & TX interrupt
  return(1); //设置成功
}

/****************************************************************************
* 名    称：UART_RX()
* 功    能：串口接收中断，每接收到1字节会发生一次中断
****************************************************************************/
#pragma vector=UART0RX_VECTOR
__interrupt void UART_RX (void)
{
//  RxFlag=1;
  /*在这里添加用户中断服务程序代码，如将数据压入接收缓冲等*/
  
  int a;
  a = U0RXBUF;
//  LCD_main_display(a,0);
  //__low_power_mode_off_on_exit();
}
/****************************************************************************
* 名    称：UART_TX()
* 功    能：串口发送中断，每发完1字节会发生一次中断
****************************************************************************/
#pragma vector=UART0TX_VECTOR
__interrupt void UART_TX (void)
{
//  TxFlag=1;
//  U0TXBUF = 0x55;
//  return;
  /*在这里添加用户中断服务程序代码，如将数据从缓冲取出等*/
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