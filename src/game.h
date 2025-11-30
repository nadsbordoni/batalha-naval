#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stddef.h>
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

// Cria e inicializa completamente um jogo
// - board_size = 0 → usa tamanho padrão = 10
// - auto_place = true → posiciona frotas automaticamente
Game *create_game(const char *name1, const char *name2, int board_size, bool auto_place);

// Libera todos os recursos do jogo (boards, frotas, struct Game)
void destroy_game(Game *g);

//Funções de jogo (tiros, turnos, vitória)

// Executa tiro do jogador "shooter" (1 ou 2) em (row,col)
// Retornos:
//  -1 → inválido (fora do tabuleiro ou repetido)
//   0 → água
//   1 → acerto
// Escreve texto explicativo em result_text (opcional)
int fire_shot(Game *g, int shooter, int row, int col,
              char *result_text, size_t bufsize);

// Checa se o jogador teve toda a frota destruída
bool is_fleet_destroyed(Player *p);

// Atualiza winner/game_over e retorna:
// 0 → ninguém ganhou
// 1 → p1 venceu
// 2 → p2 venceu
int check_victory(Game *g);

// Alterna turno entre jogador 1 e 2
void switch_turn(Game *g);


// Posiciona automaticamente toda a frota do jogador
bool auto_place_player_fleet(Player *p);

bool parse_coord(const char *str, int *out_row, int *out_col);

#endif