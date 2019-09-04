#ifndef __UART__
#define __UART__

char UART_Init(long int Baud,char Parity,char DataBits,char StopBits);
int uart_send_str(unsigned char *src);

#endif