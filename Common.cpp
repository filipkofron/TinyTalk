#include "common.h"
#include <iostream>

void prlvl(std::ostream &os, uint32_t level)
{
    for(uint32_t i = 0; i < level * 4; i++)
    {
        os << " ";
    }
}

void print_bytes(size_t bytes, void *ptr)
{
    const unsigned char *sptr = (const unsigned char *) ptr;
    std::cout << "[------------------------]" << std::endl;
    for(size_t i = 0; i < bytes; i+=4)
    {
        for(size_t j = i; j < i + 4 && j < bytes; j++)
        {
            int lo = sptr[j] & 0x0F;
            int hi = (sptr[j] >> 4) & 0x0F;

            if (lo > 9)
            {
                lo = 'A' + lo - 10;
            }
            else
            {
                lo = '0' + lo;
            }

            if (hi > 9)
            {
                hi = 'A' + hi - 10;
            }
            else
            {
                hi = '0' + hi;
            }

            printf("%c%c ", hi, lo);
        }
        printf("\n");
    }
    std::cout << "[------------------------]" << std::endl;
}
