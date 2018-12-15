#include "utils.h"

void m_delay(uint16_t limit)
{
    uint16_t x,y;
    for(x = 0; x < limit; x++)
    {
        for(y = 1; y < 1000; y++)
        {
            asm("NOP");
        }
    }
}
