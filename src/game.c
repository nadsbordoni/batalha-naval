#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "game.h"

Game* Criar_tabuleiro_jogo(int board_size) {
    Game *game = (Game*)malloc(sizeof(Game));
    game->p1 = game->p2 = 0;
    game->current_player = 0;
    game->turn_number = 1;
    game->game_over = false;
    game->winner = -1;
    game->board_size = board_size;
    return game;
}

void limpar_tabuleiro(Game *game) {
    if (!game) return;
    
    if (game->p1) {
        destroy_board(game->p1->board);
        destroy_board(game->p1->shots);
        destroy_fleet(game->p1->fleet);
        free(game->p1);
    }
    
    if (game->p2) {
        destroy_board(game->p2->board);
        destroy_board(game->p2->shots);
        destroy_fleet(game->p2->fleet);
        free(game->p2);
    }
    
    free(game);
}

void iniciar_funcoes(Game *game, const char *name1, const char *name2) {
    game->p1 = (Player*)malloc(sizeof(Player));
    strncpy(game->p1->apelido, name1, 31);
    game->p1->apelido[31] = '\0';
    game->p1->board = create_board(game->board_size);
    game->p1->shots = create_board(game->board_size);
    game->p1->fleet = create_default_fleet();
    game->p1->total_shots = game->p1->hits = game->p1->misses = 0;
    
    game->p2 = (Player*)malloc(sizeof(Player));
    strncpy(game->p2->apelido, name2, 31);
    game->p2->apelido[31] = '\0';
    game->p2->board = create_board(game->board_size);
    game->p2->shots = create_board(game->board_size);
    game->p2->fleet = create_default_fleet();
    game->p2->total_shots = game->p2->hits = game->p2->misses = 0;
}

void navios_automaticos(Game *game, Player *player) {
    printf("\nPosicionando navios de %s...\n", player->apelido);
    place_fleet_random(player->board, player->fleet);
}

void navios_manuais(Game *game, Player *player) {
    for (int i = 0; i < player->fleet->totalShips; i++) {
        Ship *ship = &player->fleet->ships[i];
        bool placed = false;
        
        while (!placed) {
            print_board(player->board, true);
            printf("\n%s (tam %d) - Coord (ex: B5): ", ship->name, ship->length);
            
            char coord[10];
            scanf("%s", coord);
            int col = toupper(coord[0]) - 'A';
            int row;
            sscanf(coord + 1, "%d", &row);
            row--;
            
            char orient;
            printf("(H/V)? ");
            scanf(" %c", &orient);
            
            if (place_ship(player->board, player->fleet, i, row, col, 
                          toupper(orient) == 'H' ? ORIENT_H : ORIENT_V)) {
                placed = true;
            } else {
                printf("Inválido!\n");
            }
        }
    }
}

void iniciar_jogo(Game *game) {
    while (!game->game_over) {
        turno_completo(game);
        if (jogo_termina(game)) {
            game->game_over = true;
        } else {
            prox_jogador(game);
            game->turn_number++;
        }
    }
    resultado_jogador(game);
}

void turno_completo(Game *game) {
    Player *attacker = jogador_atual(game);
    Player *defender = oponente_pont(game);
    
    printf("\n=== Turno %d - %s ===\n", game->turn_number, attacker->apelido);
    print_board(attacker->board, true);
    printf("\nOponente:\n");
    print_board(defender->board, false);
    
    bool valid = false;
    while (!valid) {
        char coord[10];
        printf("\nTiro: ");
        scanf("%s", coord);
        
        int col = toupper(coord[0]) - 'A';
        int row;
        sscanf(coord + 1, "%d", &row);
        row--;
        
        if (!get_cell(defender->board, row, col)) {
            printf("Fora!\n");
            continue;
        }
        
        if (get_cell(attacker->shots, row, col)->state != WATER_CELL) {
            printf("Repetido!\n");
            continue;
        }
        
        valid = tiro_jogador(game, attacker, defender, row, col);
    }
    
    printf("\n[ENTER]");
    getchar();
    getchar();
}

bool tiro_jogador(Game *game, Player *attacker, Player *defender, 
                       int row, int col) {
    attacker->total_shots++;
    Cell *target = get_cell(defender->board, row, col);
    
    if (target->state == SHIP_INTACT_CELL) {
        attacker->hits++;
        target->state = SHIP_DAMAGED_CELL;
        get_cell(attacker->shots, row, col)->state = SHIP_DAMAGED_CELL;
        
        defender->fleet->ships[target->ship_id].hits++;
        Ship *ship = &defender->fleet->ships[target->ship_id];
        
        printf(ship->hits >= ship->length ? "\nVocê afundou um NAVIO\n" : "\nNavio Afundado!!\n");
    } else {
        attacker->misses++;
        target->state = SHOT_MISS_CELL;
        get_cell(attacker->shots, row, col)->state = SHOT_MISS_CELL;
        printf("\nAcertou a Água, cuidado para não acabar com a água desse jeito\n");
    }
    
    return true;
}

void prox_jogador(Game *game) {
    game->current_player = 1 - game->current_player;
}

bool jogo_termina(Game *game) {
    if (frota_destruida(game->p1->fleet)) {
        game->winner = 1;
        return true;
    }
    if (frota_destruida(game->p2->fleet)) {
        game->winner = 0;
        return true;
    }
    return false;
}

bool navio_afundou(Player *defender, int ship_id) {
    return defender->fleet->ships[ship_id].hits >= 
           defender->fleet->ships[ship_id].length;
}

void estado_jogo(Game *game) {
    printf("\n%s:\n", game->p1->apelido);
    print_board(game->p1->board, true);
    printf("\n%s:\n", game->p2->apelido);
    print_board(game->p2->board, true);
}

void atual_player(Game *game) {
    print_board(jogador_atual(game)->board, true);
}

void oponente(Game *game) {
    print_board(oponente_pont(game)->board, false);
}

void estatistica_jogador(Game *game) {
    printf("\n%s: %d tiros, %d acertos, %.1f%%\n",
           game->p1->apelido, game->p1->total_shots, 
           game->p1->hits, precisao_jogador(game->p1));
    printf("%s: %d tiros, %d acertos, %.1f%%\n",
           game->p2->apelido, game->p2->total_shots, 
           game->p2->hits, precisao_jogador(game->p2));
}

void resultado_jogador(Game *game) {
    Player *winner = game->winner == 0 ? game->p1 : game->p2;
    
    printf("\nO Vencedor foiii... \n", winner->apelido);
    printf("Tiros: %d | Acertos: %d | Precisão: %.1f%%\n",
           winner->total_shots, winner->hits, precisao_jogador(winner));
    
    estado_jogo(game);
    estatistica_jogador(game);
}

Player* jogador_atual(Game *game) {
    return game->current_player == 0 ? game->p1 : game->p2;
}

Player* oponente_pont(Game *game) {
    return game->current_player == 0 ? game->p2 : game->p1;
}

float precisao_jogador(Player *player) {
    return player->total_shots == 0 ? 0.0f : 
           (float)player->hits / player->total_shots * 100.0f;
}

int navios_vivos(Fleet *fleet) {
    int alive = 0;
    for (int i = 0; i < fleet->totalShips; i++) {
        if (fleet->ships[i].hits < fleet->ships[i].length) alive++;
    }
    return alive;
}

bool frota_destruida(Fleet *fleet) {
    for (int i = 0; i < fleet->totalShips; i++) {
        if (fleet->ships[i].hits < fleet->ships[i].length) return false;
    }
    return true;
}