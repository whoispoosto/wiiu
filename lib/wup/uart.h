#ifndef _UART_H_
#define _UART_H_

void UART_Init();

void UART_Send(u8* buf, int len);
void UART_Debug(const char* str);

#endif //_UART_H_
