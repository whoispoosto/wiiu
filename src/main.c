#include <wup/wup.h>

#define FIRMWARE_START 0x100000
#define FIRMWARE_SIZE 0x400000

#define CHUNK_SIZE 2048

/*
* TODO:
* see if blinky program works!
* write a basic UART controller and try to send a command to my computer
* from there we can 
*/

int main() {
    u8 buf[CHUNK_SIZE];
    u8 flag = 0;

    for (u32 i = 0; i < FIRMWARE_SIZE; i += CHUNK_SIZE) {
        Flash_Read(FIRMWARE_START + i, buf, CHUNK_SIZE);
        UART_Send(buf, CHUNK_SIZE);

        flag = 1 - flag;
        UIC_SetLED(1, flag * 255);
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