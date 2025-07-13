#include <wup/wup.h>

#define HWID_SOC_TYPE (1 << 16)

void PLL_Init()
{
    // https://kuribo64.net/wup/doku.php?id=general_registers&s[]=pll
    // https://github.com/Arisotura/melonboot/blob/main/source/wup/wup.c#L32

    REG_UNK814 = 0xE;
    REG_UNK800 = 0x1011;
    REG_UNK800 = 0x1001;
    REG_UNK08 &= ~0x1;
    REG_UNK30 = 0;

    u32 reg0C;
    u32 clktype = !(REG_HARDWARE_ID & HWID_SOC_TYPE);

    if (clktype) {
        REG_TIMER_PRESCALER = 23;
        reg0C = 0x14;
    } else {
        REG_TIMER_PRESCALER = 15;
        reg0C = 0xD;
    }

    REG_TIMER_TARGET(0) = 0xFFFFFFFF;
    REG_UNK08 = ((REG_UNK08) & ~6) + 2;

    if (!(REG_HARDWARE_ID & (1<<19)))
    {
        REG_TIMER_CNT(0) = TIMER_ENABLE;
        while (REG_TIMER_VALUE(0) < 5013);
        REG_TIMER_CNT(0) = 0;
    }

    REG_PLL_INPUTDIV = reg0C;
    REG_PLL_MULTIPLIER = 0x5E8000;
    REG_PLL_PRIM_DIV = 5;
    REG_PLL_SEC1_DIV = 0x1FE;
    REG_PLL_SEC2_DIV = 0x6C;
    REG_PLL_SEC3_DIV = 0x1FE;
    REG_PLL_UNK24 = 0xBC;
    REG_PLL_UNK28 = 0x24;
    REG_PLL_UNK2C = 0x3A1;

    if (clktype) {
        REG_UNK08 |= 0x8;
    } else {
        REG_UNK08 &= ~0x8;
    }

    REG_UNK08 &= ~0x2;

    REG_TIMER_CNT(0) = TIMER_ENABLE;
    while (REG_TIMER_VALUE(0) < 5000);
    REG_TIMER_CNT(0) = 0;

    REG_UNK30 = 3;
    REG_UNK08 |= 0x1;
    REG_UNK814 = 0x5C;
    REG_UNK800 = 0x1011;
    REG_UNK800 = 0x1001;
}