//#define DEBUG

#define MEMORY_ALLCOATOR_SIZE_DEFAULT (1024 * 1024 * 32)

#define TO_TT_STR(a) ((const uint8_t *) (a))

#include <cstdint>
#include <ostream>

/**
* Print exact amount of levels spaces.
*/
void prlvl(std::ostream &os, uint32_t level);