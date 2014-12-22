// #define DEBUG
#define VERBOSE

#define MEMORY_ALLCOATOR_SIZE_DEFAULT (1024 * 256)

#define TO_TT_STR(a) ((const uint8_t *) (a))

#define COMPARE_NAME(a, b) strcmp((const char *) a, (const char *) b)

#define NOT_IMPLEMENTED \
do {   \
std::cerr << "NOT IMPLEMENTED " << __PRETTY_FUNCTION__ << " at " << __FILE__ << ":" << __LINE__ << std::endl;\
throw std::exception(); \
} while(false)

#define KILL \
do { \
    *(int *) NULL = 0; \
} while(false)

#include <cstdint>
#include <ostream>
#include <cstring>

/**
* Print exact amount of levels spaces.
*/
void prlvl(std::ostream &os, uint32_t level);

/*

Print bytes

 */
void print_bytes(size_t bytes, void *ptr);

/**
* compute short hash fron string
*/
uint32_t strHash32(const uint8_t *str);