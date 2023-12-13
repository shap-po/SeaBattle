#include "random.hpp"

#include <random>
#include <stdlib.h>
#include <time.h>

void setSeed() {
    srand(time(NULL));
}
void setSeed(int seed) {
    srand(seed);
}

int randInt(int min, int max) {
    if (min > max) return randInt(max, min);
    return rand() % (max - min + 1) + min;
}
int randInt(int max) {
    return randInt(0, max);
}
