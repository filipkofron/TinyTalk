#include "Common.h"
#include <iostream>
#include <chrono>
#include <stdint.h>
#include <unistd.h>
#include <execinfo.h>

void prlvl(std::ostream &os, uint32_t level)
{
    for(uint32_t i = 0; i < level * 4; i++)
    {
        os << " ";
    }
}

/** FROM stackoverflow.com BEGIN **/
static void full_write(int fd, const char *buf, size_t len)
{
    while (len > 0) {
        ssize_t ret = write(fd, buf, len);

        if ((ret == -1) && (errno != EINTR))
            break;

        buf += (size_t) ret;
        len -= (size_t) ret;
    }
}

void print_backtrace(void)
{
    static const char start[] = "BACKTRACE ------------\n";
    static const char end[] = "----------------------\n";

    void *bt[4096];
    int bt_size;
    char **bt_syms;
    int i;

    bt_size = backtrace(bt, 4096);
    bt_syms = backtrace_symbols(bt, bt_size);
    full_write(STDERR_FILENO, start, strlen(start));
    for (i = 1; i < bt_size; i++) {
        size_t len = strlen(bt_syms[i]);
        full_write(STDERR_FILENO, bt_syms[i], len);
        full_write(STDERR_FILENO, "\n", 1);
    }
    full_write(STDERR_FILENO, end, strlen(end));
    free(bt_syms);
}

/** FROM stackoverflow.com  END **/

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
