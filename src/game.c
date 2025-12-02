#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


static Player *get_player(Game *g, int idx) {
    if (!g) return NULL;
    if (idx == 1) return &g->p1;
    if (idx == 2) return &g->p2;
    return NULL;
}

static Player *get_opponent(Game *g, int idx) {
    if (!g) return NULL;
    return (idx == 1) ? &g->p2 : &g->p1;
}

//Criar jogo

Game *create_game(const char *name1, const char *name2, int board_size, bool auto_place) {
    Game *g = malloc(sizeof(Game));
    if (!g) return NULL;

    memset(g, 0, sizeof(Game));

    if (board_size == 0) board_size = 10; 

    if (board_size < MIN_BOARD_SIZE_SMALL || board_size > MAX_BOARD_SIZE_HUGE) {
        fprintf(stderr, "[ERRO] create_game: board_size inválido (%d)\n", board_size);
        free(g);
        return NULL;
    }

    g->board_size = board_size;
    g->current_player = 1;
    g->game_over = false;
    g->winner = 0;

    // inicializa jogadores
    if (name1)
        snprintf(g->p1.name, sizeof(g->p1.name), "%s", name1);
    else
        snprintf(g->p1.name, sizeof(g->p1.name), "Jogador1");

    if (name2)
        snprintf(g->p2.name, sizeof(g->p2.name), "%s", name2);
    else
        snprintf(g->p2.name, sizeof(g->p2.name), "Jogador2");

    // criar boards
    g->p1.board = create_board(board_size);
    g->p1.shots = create_board(board_size);
    g->p2.board = create_board(board_size);
    g->p2.shots = create_board(board_size);

    if (!g->p1.board || !g->p1.shots || !g->p2.board || !g->p2.shots) {
        destroy_game(g);
        return NULL;
    }

    // criar frotas
    if (board_size == 10) {
        g->p1.fleet = create_default_fleet();
        g->p2.fleet = create_default_fleet();
    } else {
        g->p1.fleet = create_fleet(board_size);
        g->p2.fleet = create_fleet(board_size);
    }

    if (!g->p1.fleet || !g->p2.fleet) {
        destroy_game(g);
        return NULL;
    }

    // inicializar estatísticas
    g->p1.total_shots = 0;
    g->p1.total_hits = 0;
    g->p2.total_shots = 0;
    g->p2.total_hits = 0;

    // auto colocar se pedido
    if (auto_place) {
        if (!auto_place_player_fleet(&g->p1)) {
            destroy_game(g);
            return NULL;
        }
        if (!auto_place_player_fleet(&g->p2)) {
            destroy_game(g);
            return NULL;
        }
    }

    return g;
}

void destroy_game(Game *g) {
    if (!g) return;

    // liberar boards
    if (g->p1.board) destroy_board(g->p1.board);
    if (g->p1.shots) destroy_board(g->p1.shots);
    if (g->p2.board) destroy_board(g->p2.board);
    if (g->p2.shots) destroy_board(g->p2.shots);

    // liberar frotas
    if (g->p1.fleet) destroy_fleet(g->p1.fleet);
    if (g->p2.fleet) destroy_fleet(g->p2.fleet);

    // free game
    free(g);
}

bool parse_coord(const char *str, int *out_row, int *out_col) {
    if (!str || !out_row || !out_col) return false;

    while (isspace((unsigned char)*str)) str++;

    if (!isalpha((unsigned char)str[0])) return false;

    char colch = toupper((unsigned char)str[0]);
    int col = colch - 'A';
    str++;

    while (isspace((unsigned char)*str)) str++;

    if (!isdigit((unsigned char)str[0])) return false;

    int rownum = atoi(str);
    if (rownum <= 0) return false;

    int row = rownum - 1;

    *out_row = row;
    *out_col = col;
    return true;
}


//Checa se toda a frota foi destruida
bool is_fleet_destroyed(Player *p) {
    if (!p || !p->fleet) return false;

    for (int i = 0; i < p->fleet->count; i++) {
        Ship *s = &p->fleet->ships[i];
        if (s->hits < s->length) return false;
    }
    return true;
}

//Checa quem venceu
int check_victory(Game *g) {
    if (!g) return 0;

    if (is_fleet_destroyed(&g->p1)) {
        g->game_over = true;
        g->winner = 2; 
        return 2;
    }
    if (is_fleet_destroyed(&g->p2)) {
        g->game_over = true;
        g->winner = 1;
        return 1;
    }

    g->winner = 0;
    return 0;
}


//alterna turno
void switch_turn(Game *g) {
    if (!g) return;
    g->current_player = (g->current_player == 1) ? 2 : 1;
}

bool auto_place_player_fleet(Player *p) {
    if (!p || !p->fleet || !p->board) return false;

    for (int i = 0; i < p->board->rows * p->board->cols; i++) {
        p->board->cells[i].state = WATER_CELL;
        p->board->cells[i].ship_id = NO_SHIP_ID;
    }
    bool ok = place_fleet_random(p->board, p->fleet);
    return ok;
}

int fire_shot(Game *g, int shooter, int row, int col, char *result_text, unsigned int bufsize) {
    if (!g) return -1;
    if (shooter != 1 && shooter != 2) return -1;

    Player *attacker = get_player(g, shooter);
    Player *target = get_opponent(g, shooter);
    if (!attacker || !target) return -1;
    if (!in_bounds(target->board, row, col)) {
        if (result_text && bufsize > 0) snprintf(result_text, bufsize, "Coordenada fora do tabuleiro.");
        return -1;
    }

    Cell *target_cell = get_cell(target->board, row, col);
    Cell *attacker_shot_cell = get_cell(attacker->shots, row, col);
    if (!target_cell || !attacker_shot_cell) return -1;
    if (target_cell->state == SHIP_DAMAGED_CELL || target_cell->state == SHOT_MISS_CELL) {
        if (result_text && bufsize > 0) snprintf(result_text, bufsize, "Coordenada ja atingida.");
        return -1;
    }

    attacker->total_shots++;

    // Verificar se há navio na célula
    if (target_cell->ship_id == NO_SHIP_ID) {
        // água
        target_cell->state = SHOT_MISS_CELL;
        attacker_shot_cell->state = SHOT_MISS_CELL;

        if (result_text && bufsize > 0) snprintf(result_text, bufsize, "ÁGUA.");
        return 0;
    } else {
        // acerto
        int sid = target_cell->ship_id;
        if (sid < 0 || sid >= target->fleet->count) {
            if (result_text && bufsize > 0) snprintf(result_text, bufsize, "Erro interno: ship_id inválido.");
            return -1;
        }

        Ship *ship = &target->fleet->ships[sid];
        // marcar célula como danificada no tabuleiro do alvo
        target_cell->state = SHIP_DAMAGED_CELL;
        // marcar também no mapa de tiros do atacante
        attacker_shot_cell->state = SHIP_DAMAGED_CELL;

        // atualizar estatísticas
        attacker->total_hits++;
        ship->hits++;

        // verificar se afundou
        if (ship->hits >= ship->length) {
            if (result_text && bufsize > 0)
                snprintf(result_text, bufsize, "AFUNDOU %s!", ship->name);
            // depois do afundamento checa vitória
            check_victory(g);
        } else {
            if (result_text && bufsize > 0)
                snprintf(result_text, bufsize, "ACERTOU no %s!", ship->name);
        }

        return 1;
    }
}

