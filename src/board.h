#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

#define NO_SHIP_ID -1

//Foram os cases mais auto explicativos que consegui pensar
typedef enum {
    WATER_CELL,
    SHIP_INTACT_CELL,
    SHIP_DAMAGED_CELL,
    SHOT_MISS_CELL
} CellState;

//Estrutura de uma célula do tabuleiro, com os casos criados acima como states
typedef struct {
    CellState state;
    int ship_id;
} Cell;

// Estrutura do tabuleiro, com número de linhas, colunas e as células
typedef struct {
    int rows;
    int cols;
    Cell *cells;
} Board;


Board *create_board(int rows, int cols);
void destroy_board(Board *b);
Cell *get_cell(Board *b, int row, int col);
void print_board(Board *b, bool showShips);

#endif
