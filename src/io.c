/* src/io.c — centraliza toda a entrada/saída e o loop de jogo */
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

    printf("Novo tamanho (6 a 26): ");
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

    if (novo < 6 || novo > 26) {
        printf("Valor fora do intervalo.\n");
        pause_screen();
        return *board_size;
    }

    *board_size = novo;
    printf("Tamanho alterado para %d.\n", novo);
    pause_screen();

    return *board_size;
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

    Game *g = create_game(nome1, nome2, board_size, true);
    if (!g) {
        printf("Erro ao criar o jogo.\n");
        pause_screen();
        return;
    }

    printf("\nFrotas posicionadas automaticamente.\n");
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

        /* usa a função pública parse_coord definida em game.c */
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