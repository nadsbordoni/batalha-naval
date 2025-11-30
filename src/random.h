#ifndef RND_H
#define RND_H

#include <stdbool.h>

// gerador de numeros aleatorios
void num_rand(void);

// Incia em um local especifico (bom para testes)
void seed_esp(unsigned int seed);

// Numero aleatorio entre o minino e o maximo
int numb_rando(int min, int max);

// Numero aleatório entre 1 e 0
int range_random(int max);

// Gera um true ou false
bool bool_random(void);

// Gera 0 ou 1 aleatoriamente
int binary_random(void);

// Gera um número entre 0.0 e 1.0
float floa_random(void);

// Gera uma coordenada aleatoria do tabuleiro
void return_tabuleiro(int board_size, int *row, int *col);

// Gera na vertical ou horizontal aleatoriamente
int return_number(void);  // Retorna 0 (H) ou 1 (V)

#endif