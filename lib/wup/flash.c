#include <wup/wup.h>

static u8 AddrMode;

int Flash_Init()
{
    Flash_Set4ByteAddr(1);
    return 1;
}


void Flash_ReadID(u8* id, int len)
{
    u8 cmd = 0x9F;
    SPI_Start(SPI_DEVICE_FLASH, SPI_CLK_248KHZ);
    SPI_Write(&cmd, 1);
    SPI_Read(id, len);
    SPI_Finish();
}

void Flash_WaitForStatus(u8 mask, u8 val)
{
    UART_Debug("Starting SPI...");
    // TODO: figure out how to use faster clock
    SPI_Start(SPI_DEVICE_FLASH, SPI_CLK_248KHZ);

    UART_Debug("SPI started, sending cmd...");
    u8 cmd = 0x05;
    SPI_Write(&cmd, 1);

    UART_Debug("Finished SPI write");

    for (;;)
    {
        u8 status;
        SPI_Read(&status, 1);
        if ((status & mask) == val)
            break;
    }

    SPI_Finish();
}

void Flash_WriteEnable()
{
    u8 cmd = 0x06;
    SPI_Start(SPI_DEVICE_FLASH, SPI_CLK_248KHZ);
    SPI_Write(&cmd, 1);
    SPI_Finish();

    Flash_WaitForStatus(0x03, 0x02);
}

void Flash_WriteDisable()
{
    u8 cmd = 0x04;
    SPI_Start(SPI_DEVICE_FLASH, SPI_CLK_248KHZ);
    SPI_Write(&cmd, 1);
    SPI_Finish();

    Flash_WaitForStatus(0x03, 0x00);
}

void Flash_Set4ByteAddr(int val)
{
    UART_Debug("Waiting for status...");
    Flash_WaitForStatus(0x03, 0x00);

    UART_Debug("Got status, enabling write...");
    Flash_WriteEnable();

    UART_Debug("Enabled write, starting SPI...");

    u8 cmd = val ? 0xB7 : 0xE9;

    SPI_Start(SPI_DEVICE_FLASH, SPI_CLK_248KHZ);
    SPI_Write(&cmd, 1);
    SPI_Finish();

    Flash_WriteDisable();

    UART_Debug("Done!");

    AddrMode = val ? 1 : 0;
}

void Flash_Read(u32 addr, void* data, int len)
{
    SPI_Start(SPI_DEVICE_FLASH, SPI_CLK_248KHZ);

    u8 cmd[5];
    cmd[0] = 0x03;
    if (AddrMode)
    {
        cmd[1] = (addr >> 24) & 0xFF;
        cmd[2] = (addr >> 16) & 0xFF;
        cmd[3] = (addr >> 8) & 0xFF;
        cmd[4] = addr & 0xFF;
        SPI_Write(cmd, 5);
    }
    else
    {
        cmd[1] = (addr >> 16) & 0xFF;
        cmd[2] = (addr >> 8) & 0xFF;
        cmd[3] = addr & 0xFF;
        SPI_Write(cmd, 4);
    }

    u8* dst = (u8*)data;
    int kChunkLen = 0x100000; // DMA chunk
    for (int i = 0; i < len; i += kChunkLen)
    {
        int chunk = kChunkLen;
        if ((i + chunk) > len)
            chunk = len - i;

        SPI_Read(dst, chunk);
        dst += chunk;
    }

    SPI_Finish();
}

void Flash_EraseSector(u32 addr)
{
    Flash_WaitForStatus(0x03, 0x00);
    Flash_WriteEnable();

    SPI_Start(SPI_DEVICE_FLASH, SPI_CLK_248KHZ);

    u8 cmd[5];
    cmd[0] = 0xD8;
    if (AddrMode)
    {
        cmd[1] = (addr >> 24) & 0xFF;
        cmd[2] = (addr >> 16) & 0xFF;
        cmd[3] = (addr >> 8) & 0xFF;
        cmd[4] = addr & 0xFF;
        SPI_Write(cmd, 5);
    }
    else
    {
        cmd[1] = (addr >> 16) & 0xFF;
        cmd[2] = (addr >> 8) & 0xFF;
        cmd[3] = addr & 0xFF;
        SPI_Write(cmd, 4);
    }

    SPI_Finish();

    Flash_WaitForStatus(0x03, 0x00);
}

void Flash_EraseSubsector(u32 addr)
{
    Flash_WaitForStatus(0x03, 0x00);
    Flash_WriteEnable();

    SPI_Start(SPI_DEVICE_FLASH, SPI_CLK_248KHZ);

    u8 cmd[5];
    cmd[0] = 0x20;
    if (AddrMode)
    {
        cmd[1] = (addr >> 24) & 0xFF;
        cmd[2] = (addr >> 16) & 0xFF;
        cmd[3] = (addr >> 8) & 0xFF;
        cmd[4] = addr & 0xFF;
        SPI_Write(cmd, 5);
    }
    else
    {
        cmd[1] = (addr >> 16) & 0xFF;
        cmd[2] = (addr >> 8) & 0xFF;
        cmd[3] = addr & 0xFF;
        SPI_Write(cmd, 4);
    }

    SPI_Finish();

    Flash_WaitForStatus(0x03, 0x00);
}

void Flash_PageProgram(u32 addr, void* data, int len)
{
    Flash_WaitForStatus(0x03, 0x00);
    Flash_WriteEnable();

    SPI_Start(SPI_DEVICE_FLASH, SPI_CLK_248KHZ);

    u8 cmd[5];
    cmd[0] = 0x02;
    if (AddrMode)
    {
        cmd[1] = (addr >> 24) & 0xFF;
        cmd[2] = (addr >> 16) & 0xFF;
        cmd[3] = (addr >> 8) & 0xFF;
        cmd[4] = addr & 0xFF;
        SPI_Write(cmd, 5);
    }
    else
    {
        cmd[1] = (addr >> 16) & 0xFF;
        cmd[2] = (addr >> 8) & 0xFF;
        cmd[3] = addr & 0xFF;
        SPI_Write(cmd, 4);
    }

    SPI_Write(data, len);

    SPI_Finish();
}

void Flash_Write(u32 addr, void* data, int len)
{
    u8* data8 = (u8*)data;
    for (int pos = 0; pos < len; )
    {
        int chunk = 0x100 - (addr & 0xFF);
        if ((pos + chunk) > len)
            chunk = len - pos;

        Flash_PageProgram(addr, data8, chunk);
        addr += chunk;
        data8 += chunk;
        pos += chunk;
    }
}
