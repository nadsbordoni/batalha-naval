#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#include "board.h"
#include "fleet.h"

typedef struct {
    char name[32];

    Board *board;     
    Board *shots;      

    Fleet *fleet;      

    int total_shots;
    int total_hits;
} Player;

typedef struct {
    Player p1;
    Player p2;

    int current_player; 
    bool game_over;
    int winner;           

    int board_size;  
} Game;

//Funções principais de criação e destruição do jogo
Game *create_game(const char *name1, const char *name2, int board_size, bool auto_place);

// Libera todos os recursos do jogo (boards, frotas, struct Game)
void destroy_game(Game *g);

//Funções de jogo (tiros, turnos, vitória)

int fire_shot(Game *g, int shooter, int row, int col,
              char *result_text, unsigned int bufsize);

// Checa se o jogador teve toda a frota destruída
bool is_fleet_destroyed(Player *p);

int check_victory(Game *g);

// Alterna turno
void switch_turn(Game *g);


// Posiciona automaticamente a frota
bool auto_place_player_fleet(Player *p);

bool parse_coord(const char *str, int *out_row, int *out_col);

#endif
