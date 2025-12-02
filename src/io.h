#ifndef IO_H
#define IO_H

#include <stdbool.h>
#include "game.h"

void clear_screen(void);
void pause_screen(void);

void print_two_boards(Game *g);

int settings_menu(int *board_size);

void play_game(int board_size);

#endif