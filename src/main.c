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
    // Blinky at end
    while (1) {
        UART_SendString("Hello world!");
        UIC_SetLED(0, 255);
        WUP_DelayMS(500);
        UIC_SetLED(0, 0);
        WUP_DelayMS(500);
    }

    return 0;
}