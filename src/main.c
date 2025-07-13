#include <wup/wup.h>

#define FIRMWARE_START 0x100000
#define PARTITION_SIZE 0x400000
#define PARTITION_COUNT 2

#define CHUNK_SIZE 2048

#define HOME_LED 0
#define BATTERY_LED 1

int main() {
    u8 buf[CHUNK_SIZE];
    u8 led = 0;

    for (u32 round = 0; round < PARTITION_COUNT; ++round) {
        // Delay might not be necessary, but it increases the reliability of this binary working properly
        WUP_DelayMS(500);

        for (u32 i = 0; i < PARTITION_SIZE; i += CHUNK_SIZE) {
            u32 addr = FIRMWARE_START + (PARTITION_SIZE * round) + i;

            Flash_Read(addr, buf, CHUNK_SIZE);
            UART_Send(buf, CHUNK_SIZE);

            // Flip LED
            led = 1 - led;
            UIC_SetLED(BATTERY_LED, led ? 255 : 0);
        }
    }

    // Blinky at end
    while (1) {
        UIC_SetLED(HOME_LED, 255);
        WUP_DelayMS(500);
        UIC_SetLED(HOME_LED, 0);
        WUP_DelayMS(500);
    }

    return 0;
}