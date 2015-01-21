#include "Common.h"
#include <iostream>
#include <chrono>
#include <stdint.h>

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

uint32_t strHash32(const uint8_t *str)
{
    /*uint32_t res = 1;

    uint32_t size = (uint32_t) strlen((const char *) str);
    uint32_t sizeRest = size % 4;
    size = size / 4;
    for(uint32_t i = 0; i < size; i++)
    {
        res = ((((uint32_t *) str)[i] ^ res) << 2) + 1;
    }
    sizeRest += size * 4;
    for(uint32_t i = size * 4; i < sizeRest; i++)
    {
        res = (res << 2) + str[i] * 7;
    }
    return res;*/
    register uint32_t res = 2166136261u;
    while(*str)
    {
        res *= 16777619u;
        res ^= *str++;
    }
    return res;
}

int64_t currentTimeMilis()
{
    namespace sc = std::chrono;

    auto time = sc::system_clock::now();
    auto since_epoch = time.time_since_epoch();
    auto millis = sc::duration_cast<sc::milliseconds>(since_epoch);

    return (int64_t) millis.count();
}
