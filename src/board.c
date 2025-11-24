// board.c
#include <stdio.h>
#include <stdlib.h>
#include "board.h"

Board *create_board(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return NULL;

    Board *b = malloc(sizeof(Board));
    if (!b) return NULL;

    b->rows = rows;
    b->cols = cols;

    b->cells = malloc(rows * cols * sizeof(Cell));
    if (!b->cells) {
        free(b);
        return NULL;
    }

    // Inicializa todas as células como água
    for (int i = 0; i < rows * cols; i++) {
        b->cells[i].state = WATER_CELL;
        b->cells[i].ship_id = NO_SHIP_ID;
    }

    return b;
}

void destroy_board(Board *b) {
    if (!b) return;
    free(b->cells);
    free(b);
}

Cell *get_cell(Board *b, int row, int col) {
    if (!b) return NULL;

    // Verifica se os índices estão dentro dos limites do tabuleiro 
    if (row < 0 || row >= b->rows) return NULL;
    if (col < 0 || col >= b->cols) return NULL;

    int index = row * b->cols + col;
    return &b->cells[index];
}

// A ideia é printar o tabuleiro com as letras e numeros, como era no papel (pelo menos era quando eu jogava)
void print_board(Board *b, bool showShips) {
    if (!b) return;

    printf("   ");
    for (int c = 0; c < b->cols; c++) {
        printf("%c ", 'A' + c);
    }
    printf("\n");

    for (int r = 0; r < b->rows; r++) {
        printf("%2d ", r + 1);

        for (int c = 0; c < b->cols; c++) {
            Cell *cell = get_cell(b, r, c);

            char ch = '?';

            if (!cell) {
                fprintf(stderr, "[ERRO] print_board: coordenada invalida (%d,%d)\n", r, c);
                ch = '!';
            } else {
                switch (cell->state) {
                    case WATER_CELL:
                        ch = '~';
                        break;

                    case SHIP_INTACT_CELL:
                        ch = showShips ? 'S' : '~';
                        break;

                    case SHIP_DAMAGED_CELL:
                        ch = 'X';
                        break;

                    case SHOT_MISS_CELL:
                        ch = '.';
                        break;

                    default:
                        ch = '?';
                        break;
                }
            }

            printf("%c ", ch);
        }

        printf("\n");
    }
}