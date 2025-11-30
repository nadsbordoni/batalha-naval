#include <stdlib.h>
#include <time.h>
#include "rnd.h"

void num_rand(void) {
    srand((unsigned int)time(0));
}

void seed_esp(unsigned int seed) {
    srand(seed);
}

int numb_rando(int min, int max) {
    if (min > max) {
        int temp = min;
        min = max;
        max = temp;
    }
    
    return min + rand() % (max - min + 1);
}

int range_random(int max) {
    if (max <= 0) {
        return 0;
    }
    
    return rand() % max;
}

bool bool_random(void) {
    return (rand() % 2) == 1;
}

int binary_random(void) {
    return rand() % 2;
}

float floa_random(void) {
    return (float)rand() / (float)RAND_MAX;
}

void return_tabuleiro(int board_size, int *row, int *col) {
    *row = rnd_range(board_size);
    *col = rnd_range(board_size);
}

int return_number(void) {
    return rnd_binary();
}