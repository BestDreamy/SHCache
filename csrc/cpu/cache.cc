#include "../utils.h"

struct Cache {
    uint32_t tag_array[128];
    uint32_t data_array[128][4];
    uint32_t val_array[128];
};