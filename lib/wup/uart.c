#include <wup/wup.h>

#define MAX_STR_LEN 256

#define SUFFIX "\r\n"
#define SUFFIX_LEN 2

#define DEBUG

void UART_Init()
{
    const int num = 1;

    u32 resetbit = RESET_UART0 << num;
    REG_HARDWARE_RESET |= resetbit;
    REG_CLK_UART1 = CLK_SOURCE(CLKSRC_32MHZ) | CLK_DIVIDER(8);
    REG_HARDWARE_RESET &= ~resetbit;

    //REG_UART_CNT1(num) = 0;
    //REG_UART_IRQ_ENABLE(num) = 0;

    REG_UART_CNT2(num) = UART_ENABLE;

    // clock parameters for 115200 baud
    REG_UART_CLK_DIV1(num) = 1;
    REG_UART_CLK_MULT(num) = 288;
    REG_UART_CLK_DIV2(num) = 625;

    REG_UART_UNK10(num) = 0;
    REG_UART_CNT1(num) = UART_DATA_8BIT | UART_STOP_1BIT | UART_PARITY_NONE;
    REG_UART_UNK18(num) = 0;
    REG_UART_IRQ_ENABLE(num) = UART_IRQEN_TX;

    while (REG_UART_IRQ_STATUS(num) != UART_IRQST_TX_READY);
}

void UART_Send(u8* buf, int len)
{
    const int num = 1;

    SPDMA_Transfer(1, buf, SPDMA_PERI_UART1, SPDMA_DIR_WRITE, len);
    SPDMA_Wait(1);

    // wait for UART IRQ
    while (REG_UART_IRQ_STATUS(num) != UART_IRQST_TX_READY);
}

void UART_Debug(const char* str)
{
#ifdef DEBUG
    u32 str_len = strnlen(str, MAX_STR_LEN);

    // Create a buffer to store the final message
    // Total length: max possible message length + suffix length
    char buf[MAX_STR_LEN + SUFFIX_LEN];

    // Copy message + suffix into buffer
    memcpy(buf, str, str_len);
    memcpy(buf + str_len, SUFFIX, SUFFIX_LEN);

    UART_Send((u8*)buf, str_len + SUFFIX_LEN);
#endif
}