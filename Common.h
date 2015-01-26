// #define DEBUG
//#define VERBOSE

#ifdef DEBUG
    #define VERBOSE
#endif

//#define MEMORY_ALLCOATOR_SIZE_DEFAULT (1024 * 256)
#define MEMORY_ALLCOATOR_SIZE_DEFAULT (1024 * 1024)

#define TO_TT_STR(a) ((const uint8_t *) (a))

#define COMPARE_NAME(a, b) strcmp((const char *) a, (const char *) b)

#define NOT_IMPLEMENTED \
do {   \
std::cerr << "NOT IMPLEMENTED " << __PRETTY_FUNCTION__ << " at " << __FILE__ << ":" << __LINE__ << std::endl;\
KILL; \
} while(false)

#include <cstdint>
#include <ostream>
#include <cstring>

/**
* Print exact amount of levels spaces.
*/
void prlvl(std::ostream &os, uint32_t level);


/**
* Print backtrace.
*/
void print_backtrace(void);

/*

Print bytes

 */
void print_bytes(size_t bytes, void *ptr);

/**
* compute short hash fron string
*/
uint32_t strHash32(const uint8_t *str);

int64_t currentTimeMilis();

#define KILL \
do { \
    std::cerr << "** THIS IS FORCED KILL **" << std::endl;\
    print_backtrace();\
    std::cout.flush();\
    std::cerr.flush();\
    exit(1); \
} while(false)
