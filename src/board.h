#ifndef BOARD_H
#define BOARD_H

#include "fleet.h"
#include <stdbool.h>

// Limites de acordo com a faixa de tamanho
#define MIN_BOARD_SIZE_SMALL 6
#define MAX_BOARD_SIZE_SMALL 10
#define MIN_BOARD_SIZE_MEDIUM 11
#define MAX_BOARD_SIZE_MEDIUM 15
#define MIN_BOARD_SIZE_BIG 16
#define MAX_BOARD_SIZE_BIG 20
#define MIN_BOARD_SIZE_HUGE 21
#define MAX_BOARD_SIZE_HUGE 26

#define NO_SHIP_ID -1

typedef enum {
    WATER_CELL,
    SHIP_INTACT_CELL,
    SHIP_DAMAGED_CELL,
    SHOT_MISS_CELL
} CellState;

typedef struct {
    CellState state;
    int ship_id;
} Cell;

typedef struct {
    int rows;
    int cols;
    Cell *cells;
} Board;

// Orientação para posicionamento dos navios no tabuleiro
typedef enum {
    ORIENT_H,
    ORIENT_V
} Orientation;

// Funções relacionadas à criação e manipulação do tabuleiro
Board *create_board(int size);
void destroy_board(Board *b);
Cell *get_cell(Board *b, int row, int col);
void print_board(Board *b, bool showShips);

bool in_bounds(Board *b, int row, int col);

// Funções para posicionamento de navios
bool can_place_ship(Board *b, int row, int col, int length, Orientation orient);
bool place_ship(Board *b, Fleet *fleet, int ship_index, int row, int col, Orientation orient);
bool place_fleet_random(Board *b, Fleet *fleet);

#endif
