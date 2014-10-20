#include "common.h"

void prlvl(std::ostream &os, uint32_t level)
{
    for(uint32_t i = 0; i < level * 4; i++)
    {
        os << " ";
    }
}
