#include <wup/wup.h>

#define FIRMWARE_START 0x100000 // 0x500000 for other partition
#define FIRMWARE_SIZE 0x400000

#define CHUNK_SIZE 2048

int main() {
    u8 buf[CHUNK_SIZE];
    u8 flag = 0;

    for (u32 i = 0; i < FIRMWARE_SIZE; i += CHUNK_SIZE) {
        Flash_Read(FIRMWARE_START + i, buf, CHUNK_SIZE);
        UART_Send(buf, CHUNK_SIZE);

        flag = 1 - flag;
        UIC_SetLED(1, (flag << 8) - flag);
    }

    // Blinky at end
    while (1) {
        UIC_SetLED(0, 255);
        WUP_DelayMS(500);
        UIC_SetLED(0, 0);
        WUP_DelayMS(500);
    }

    return 0;
}