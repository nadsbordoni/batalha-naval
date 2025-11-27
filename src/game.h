#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include "board.h"
#include "fleet.h"
// Vai representar um jogador dentro do jogo
typedef struct{
    char apelido[32]; //Apelido ou nome do jogador
    Board *board; //Tabuleiro em si com os navios
    Board *shots; //Misseis jogados nos navios
    Fleet *fleet; //Frota dos navios
    int total_shots; //Total de tiros
    int hits; // Total de acertos
    int misses; //Total de erros
}Player;

typedef struct{
    Player *p1; //Jogador 1
    Player *p2; //Jogador 2
    int current_player; //Jogador atual
    int turn_number; //Turno atual
    bool game_over; //O jogo terminou
    int winner; //Vencedor
    int board_size; //Tamanho do tabuleiro
}Game;

Game* Criar_tabuleiro_jogo(int board_size);

void limpar_tabuleiro(Game *game);

void iniciar_funcoes(Game *game, const char *name1, const char *name2);

void navios_automaticos(Game *game, Player *player);

void navios_manuais(Game *game, Player *player);

void iniciar_jogo(Game *game);

void turno_completo(Game *game);

bool tiro_jogador(Game *game, Player *attacker, Player *defender, 
                       int row, int col);

void prox_jogador(Game *game);

bool jogo_termina(Game *game);

void estado_jogo(Game *game);

void atual_player(Game *game);

void oponente(Game *game);

void estatistica_jogador(Game *game);

void resultado_jogador(Game *game);

Player* jogador_atual(Game *game);

Player* oponente_pont(Game *game);

float precisao_jogador(Player *player);

bool navio_afundou(Player *defender, int ship_id);

int navios_vivos(Fleet *fleet);

bool frota_destruida(Fleet *fleet);

#endif