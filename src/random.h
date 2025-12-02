#ifndef RANDOM_H
#define RANDOM_H

#include <stdbool.h>

// Inicializa o gerador de números aleatórios
void random_set_seed(unsigned int seed);

// Inteiro entre min e max especificados (inclusivo)
int random_int_range(int min, int max);

int random_index(int max);

bool random_bool(void);

int random_bit(void);

float random_float(void);

// Coordenada aleatória dentro do tabuleiro
void random_board_coord(int board_size, int *row, int *col);

int return_number(void);

#endif