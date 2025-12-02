#include "board.h"
#include "fleet.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Criação do tabuleiro dinâmico
Board *create_board(int size) {
    if (size < MIN_BOARD_SIZE_SMALL || size > MAX_BOARD_SIZE_HUGE) {
        fprintf(stderr,
            "[ERRO] create_board: tamanho inválido (%d). Deve ser entre %d e %d\n",
            size, MIN_BOARD_SIZE_SMALL, MAX_BOARD_SIZE_HUGE);
        return NULL;
    }

    Board *b = malloc(sizeof(Board));
    if (!b) return NULL;

    b->rows = size;
    b->cols = size;

    b->cells = calloc((size_t)size * (size_t)size, sizeof(Cell));
    if (!b->cells) {
        free(b);
        return NULL;
    }

    for (int i = 0; i < size * size; i++) {
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

    if (row < 0 || row >= b->rows) return NULL;
    if (col < 0 || col >= b->cols) return NULL;

    return &b->cells[row * b->cols + col];
}

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
                ch = '!';
            } else {
                switch (cell->state) {
                    case WATER_CELL:         ch = '~'; break;
                    case SHIP_INTACT_CELL:   ch = showShips ? 'S' : '~'; break;
                    case SHIP_DAMAGED_CELL:  ch = 'X'; break;
                    case SHOT_MISS_CELL:     ch = '.'; break;
                    default:                 ch = '?'; break;
                }
            }

            printf("%c ", ch);
        }
        printf("\n");
    }
}

bool in_bounds(Board *b, int row, int col) {
    if (!b) return false;
    return row >= 0 && row < b->rows && col >= 0 && col < b->cols;
}

bool can_place_ship(Board *b, int row, int col, int length, Orientation orient) {
    if (!b || length <= 0) return false;

    for (int i = 0; i < length; i++) {
        int rr = row + (orient == ORIENT_V ? i : 0);
        int cc = col + (orient == ORIENT_H ? i : 0);

        if (!in_bounds(b, rr, cc)) return false;

        Cell *main_cell = get_cell(b, rr, cc);
        if (!main_cell) return false;

        if (main_cell->ship_id != NO_SHIP_ID)
            return false;

        for (int dr = -1; dr <= 1; dr++) {
            for (int dc = -1; dc <= 1; dc++) {
                int nr = rr + dr;
                int nc = cc + dc;

                if (!in_bounds(b, nr, nc)) continue;

                Cell *n = get_cell(b, nr, nc);
                if (!n) return false;

                if (n->ship_id != NO_SHIP_ID)
                    return false;
            }
        }
    }

    return true;
}

bool place_ship(Board *b, Fleet *fleet, int ship_index, int row, int col, Orientation orient) {
    if (!b || !fleet) return false;
    if (ship_index < 0 || ship_index >= fleet->count) return false;

    int length = fleet->ships[ship_index].length;

    if (!can_place_ship(b, row, col, length, orient))
        return false;

    for (int i = 0; i < length; i++) {
        int rr = row + (orient == ORIENT_V ? i : 0);
        int cc = col + (orient == ORIENT_H ? i : 0);

        Cell *cell = get_cell(b, rr, cc);
        cell->state = SHIP_INTACT_CELL;
        cell->ship_id = ship_index;
    }

    fleet->ships[ship_index].placed = 1;
    return true;
}

bool place_fleet_random(Board *b, Fleet *fleet) {
    if (!b || !fleet) return false;

    for (int i = 0; i < fleet->count; i++)
        fleet->ships[i].placed = 0;

    static int seeded = 0;
    if (!seeded) {
        srand((unsigned)time(NULL));
        seeded = 1;
    }

    for (int s = 0; s < fleet->count; s++) {
        int length = fleet->ships[s].length;
        bool placed = false;

        int max_attempts = 5000;

        for (int attempt = 0; attempt < max_attempts && !placed; attempt++) {
            Orientation orient = (rand() % 2) ? ORIENT_H : ORIENT_V;

            int max_row = b->rows - (orient == ORIENT_V ? length : 1);
            int max_col = b->cols - (orient == ORIENT_H ? length : 1);

            if (max_row < 0 || max_col < 0)
                continue;

            int row = rand() % (max_row + 1);
            int col = rand() % (max_col + 1);

            if (can_place_ship(b, row, col, length, orient)) {
                placed = place_ship(b, fleet, s, row, col, orient);
            }
        }

        if (!placed) {
            return false;
        }
    }

    return true;
}