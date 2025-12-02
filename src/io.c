#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "game.h"
#include "board.h"
#include "fleet.h"
#include "io.h"

void clear_screen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause_screen(void) {
    printf("\nPressione ENTER para continuar...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void print_two_boards(Game *g) {
    if (!g) return;

    Player *cur = (g->current_player == 1) ? &g->p1 : &g->p2;

    printf("\nSituação do jogo (visão de %s):\n\n", cur->name);

    printf("%s - seu tabuleiro:\n", cur->name);
    print_board(cur->board, true);

    printf("\n%s - seu mapa de tiros:\n", cur->name);
    print_board(cur->shots, false);

    printf("\n");
}

int settings_menu(int *board_size) {
    if (!board_size) return 10;

    clear_screen();
    printf("Configurações\n\n");
    printf("Tamanho atual: %d x %d\n\n", *board_size, *board_size);

    printf("Novo tamanho (%d a %d): ", MIN_BOARD_SIZE_SMALL, MAX_BOARD_SIZE_HUGE);
    int novo;

    if (scanf("%d", &novo) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        printf("Valor inválido.\n");
        pause_screen();
        return *board_size;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}

    if (novo < MIN_BOARD_SIZE_SMALL || novo > MAX_BOARD_SIZE_HUGE) {
        printf("Valor fora do intervalo.\n");
        pause_screen();
        return *board_size;
    }

    *board_size = novo;
    printf("Tamanho alterado para %d.\n", novo);
    pause_screen();

    return *board_size;
}

bool manual_place_player_fleet(Player *p) {
    if (!p || !p->fleet || !p->board) return false;

    // limpa tabuleiro (marca água / sem navio)
    for (int r = 0; r < p->board->rows; r++) {
        for (int c = 0; c < p->board->cols; c++) {
            Cell *cell = get_cell(p->board, r, c);
            if (cell) {
                cell->state = WATER_CELL;
                cell->ship_id = NO_SHIP_ID;
            }
        }
    }

    char entrada[64];

    for (int s = 0; s < p->fleet->count; s++) {
        Ship *ship = &p->fleet->ships[s];
        ship->placed = 0;

        bool placed = false;
        while (!placed) {
            clear_screen();
            printf("Posicionamento manual - jogador %s\n", p->name);
            printf("Colocando navio: %s (tamanho %d)\n\n", ship->name, ship->length);
            printf("Tabuleiro atual:\n");
            print_board(p->board, true);

            printf("\nDigite coordenada inicial (ex: A5): ");
            if (!fgets(entrada, sizeof(entrada), stdin)) return false;
            entrada[strcspn(entrada, "\n")] = '\0';
            if (strlen(entrada) == 0) {
                printf("Entrada vazia. Tente novamente.\n");
                pause_screen();
                continue;
            }

            int row, col;
            if (!parse_coord(entrada, &row, &col)) {
                printf("Coordenada inválida. Formato esperado: letra+numero (ex: A5).\n");
                pause_screen();
                continue;
            }

            if (!in_bounds(p->board, row, col)) {
                printf("Coordenada fora do tabuleiro.\n");
                pause_screen();
                continue;
            }

            printf("Orientação - (H)orizontal ou (V)ertical? ");
            if (!fgets(entrada, sizeof(entrada), stdin)) return false;
            entrada[strcspn(entrada, "\n")] = '\0';
            if (strlen(entrada) == 0) {
                printf("Orientação vazia. Tente novamente.\n");
                pause_screen();
                continue;
            }

            char ch = toupper((unsigned char)entrada[0]);
            Orientation orient;
            if (ch == 'H') orient = ORIENT_H;
            else if (ch == 'V') orient = ORIENT_V;
            else {
                printf("Orientação inválida. Use H ou V.\n");
                pause_screen();
                continue;
            }

            if (!can_place_ship(p->board, row, col, ship->length, orient)) {
                printf("Não é possível posicionar aí (sobreposição ou fora do tabuleiro).\n");
                pause_screen();
                continue;
            }

            if (!place_ship(p->board, p->fleet, s, row, col, orient)) {
                printf("Falha ao posicionar navio. Tente outra posição.\n");
                pause_screen();
                continue;
            }

            placed = true;
            printf("Navio %s posicionado.\n", ship->name);
            pause_screen();
        }
    }

    return true;
}

void play_game(int board_size) {
    clear_screen();

    char nome1[32];
    char nome2[32];

    printf("Nome do jogador 1: ");
    if (!fgets(nome1, sizeof(nome1), stdin)) return;
    nome1[strcspn(nome1, "\n")] = '\0';

    printf("Nome do jogador 2: ");
    if (!fgets(nome2, sizeof(nome2), stdin)) return;
    nome2[strcspn(nome2, "\n")] = '\0';

  /* perguntar modo de posicionamento */
char modo_buf[8];
printf("Posicionamento (M)anual ou (A)utomático? [A]: ");
if (!fgets(modo_buf, sizeof(modo_buf), stdin)) return;
modo_buf[strcspn(modo_buf, "\n")] = '\0';
char modo = (modo_buf[0] == '\0') ? 'A' : toupper((unsigned char)modo_buf[0]);

/* criar jogo sem auto-place para termos controle */
Game *g = create_game(nome1, nome2, board_size, false);
if (!g) {
    printf("Erro ao criar o jogo.\n");
    pause_screen();
    return;
}

if (modo == 'A') {
    if (!auto_place_player_fleet(&g->p1) || !auto_place_player_fleet(&g->p2)) {
        printf("Falha ao posicionar automaticamente as frotas.\n");
        destroy_game(g);
        pause_screen();
        return;
    }
    printf("\nFrotas posicionadas automaticamente.\n");
    pause_screen();
} else {
    /* Manual para jogador 1 */
    printf("\nJogador %s - posicionamento manual.\n", g->p1.name);
    pause_screen();
    if (!manual_place_player_fleet(&g->p1)) {
        printf("Falha no posicionamento manual do jogador 1.\n");
        destroy_game(g);
        pause_screen();
        return;
    }

    /* Pergunta se o jogador 2 quer manual também */
    char buf2[8];
    printf("Jogador %s quer posicionar manualmente também? (S/N) [N]: ", g->p2.name);
    if (!fgets(buf2, sizeof(buf2), stdin)) { destroy_game(g); return; }
    buf2[strcspn(buf2, "\n")] = '\0';
    char choice2 = (buf2[0] == '\0') ? 'N' : toupper((unsigned char)buf2[0]);

    if (choice2 == 'S') {
        printf("\nJogador %s - posicionamento manual.\n", g->p2.name);
        pause_screen();
        if (!manual_place_player_fleet(&g->p2)) {
            printf("Falha no posicionamento manual do jogador 2.\n");
            destroy_game(g);
            pause_screen();
            return;
        }
    } else {
        if (!auto_place_player_fleet(&g->p2)) {
            printf("Falha ao posicionar automaticamente o jogador 2.\n");
            destroy_game(g);
            pause_screen();
            return;
        }
    }
}
    pause_screen();

    while (!g->game_over) {
        clear_screen();
        print_two_boards(g);

        Player *p = (g->current_player == 1) ? &g->p1 : &g->p2;

        printf("\nVez de %s.\n", p->name);

        char entrada[32];
        int row, col;

        printf("Coordenada (ex: A5): ");
        if (!fgets(entrada, sizeof(entrada), stdin)) {
            destroy_game(g);
            return;
        }
        entrada[strcspn(entrada, "\n")] = '\0';

        if (!parse_coord(entrada, &row, &col)) {
            printf("Entrada inválida.\n");
            pause_screen();
            continue;
        }

        char resultado[128];
        int r = fire_shot(g, g->current_player, row, col, resultado, sizeof(resultado));

        if (r == -1) {
            printf("Jogada inválida: %s\n", resultado);
            pause_screen();
            continue;
        }

        printf("\n%s\n", resultado);
        pause_screen();

        int w = check_victory(g);
        if (w == 1 || w == 2) break;

        switch_turn(g);
    }

    clear_screen();
    print_two_boards(g);

    if (g->winner == 1)
        printf("\n%s venceu.\n", g->p1.name);
    else if (g->winner == 2)
        printf("\n%s venceu.\n", g->p2.name);

    printf("\nResumo:\n");
    printf("%s: %d tiros, %d acertos\n", g->p1.name, g->p1.total_shots, g->p1.total_hits);
    printf("%s: %d tiros, %d acertos\n", g->p2.name, g->p2.total_shots, g->p2.total_hits);

    destroy_game(g);
    pause_screen();
}