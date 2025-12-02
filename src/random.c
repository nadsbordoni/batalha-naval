#include <stdlib.h>
#include <time.h>
#include "random.h"

void random_set_seed(unsigned int seed) {
    srand(seed);
}

int random_int_range(int min, int max) {
    if (min > max) {
        int temp = min;
        min = max;
        max = temp;
    }
    return min + (rand() % (max - min + 1));
}

int random_index(int max) {
    if (max <= 1) return 0;
    return rand() % max;
}

bool random_bool(void) {
    return rand() % 2;
}

int random_bit(void) {
    return rand() % 2;
}

float random_float(void) {
    return (float)rand() / (float)(RAND_MAX + 1.0);
}

void random_board_coord(int board_size, int *row, int *col) {
    if (!row || !col || board_size <= 0) return;
    *row = random_index(board_size);
    *col = random_index(board_size);
}

int return_number(void) {
    return random_bit();
}