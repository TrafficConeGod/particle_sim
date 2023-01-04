#include "rand_range.h"
#include <stdlib.h>

int rand_range(int min, int max) {
    return (rand() % (max - min + 1));
}