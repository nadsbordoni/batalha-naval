#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "game.h"
#include "board.h"
#include "fleet.h"
-
// Funções auxiliares de menu

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause_screen() {
    printf("\nPressione ENTER para continuar...");
    getchar();
}

// Exibe o tabuleiro
void print_two_boards(Game *g) {
    printf("\n===== SITUAÇÃO DO JOGO =====\n\n");
    
    printf("Tabuleiro de %s (SEUS NAVIOS):\n", g->p1.name);
    print_board(g->p1.board, true);

    printf("\nMapa de tiros de %s (onde ele já atirou):\n", g->p1.name);
    print_board(g->p1.shots, true);

    printf("\n------------------------------\n\n");

    printf("Tabuleiro de %s (SEUS NAVIOS):\n", g->p2.name);
    print_board(g->p2.board, true);

    printf("\nMapa de tiros de %s (onde ele já atirou):\n", g->p2.name);
    print_board(g->p2.shots, true);
}

// MENU DE CONFIGURAÇÕES

int settings_menu(int *board_size) {
    clear_screen();
    printf("===== CONFIGURAÇÕES =====\n\n");
    printf("Tamanho atual do tabuleiro: %d x %d\n\n", *board_size, *board_size);

    printf("Escolha o novo tamanho (6 a 26): ");
    int novo;
    scanf("%d", &novo);
    getchar(); 

    if (novo < MIN_BOARD_SIZE_SMALL || novo > MAX_BOARD_SIZE_HUGE) {
        printf("Tamanho inválido!\n");
        pause_screen();
        return *board_size;
    }

    *board_size = novo;
    printf("Tamanho alterado para %d x %d.\n", novo, novo);
    pause_screen();

    return *board_size;
}

// JOGO PRINCIPAL

void play_game(int board_size) {
    clear_screen();

    printf("Digite o nome do Jogador 1: ");
    char nome1[32];
    fgets(nome1, sizeof(nome1), stdin);
    nome1[strcspn(nome1, "\n")] = '\0';

    printf("Digite o nome do Jogador 2: ");
    char nome2[32];
    fgets(nome2, sizeof(nome2), stdin);
    nome2[strcspn(nome2, "\n")] = '\0';

    // criar jogo com posicionamento automático
    Game *g = create_game(nome1, nome2, board_size, true);
    if (!g) {
        printf("Erro ao criar o jogo!\n");
        return;
    }

    printf("\nFrotas posicionadas automaticamente!\n");
    pause_screen();

    // loop principal do jogo
    while (!g->game_over) {

        clear_screen();
        print_two_boards(g);

        Player *p = (g->current_player == 1) ? &g->p1 : &g->p2;

        printf("\nVez de %s!\n", p->name);

        char entrada[16];
        int row, col;

        printf("Digite coordenada para atacar (ex: A5): ");
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = '\0';

        if (!parse_coord(entrada, &row, &col)) {
            printf("Entrada inválida!\n");
            pause_screen();
            continue;
        }

        char resultado[64];
        int r = fire_shot(g, g->current_player, row, col, resultado, sizeof(resultado));

        if (r == -1) {
            printf("Jogada inválida: %s\n", resultado);
            pause_screen();
            continue;
        }

        printf("\nResultado: %s\n", resultado);
        pause_screen();

        // verifica vitória
        int w = check_victory(g);
        if (w == 1 || w == 2) break;

        // troca turno se não ganhou
        switch_turn(g);
    }

    // fim de jogo
    clear_screen();
    print_two_boards(g);

    if (g->winner == 1)
        printf("\n===== %s VENCEU A PARTIDA! =====\n", g->p1.name);
    else if (g->winner == 2)
        printf("\n===== %s VENCEU A PARTIDA! =====\n", g->p2.name);

    printf("\nEstatísticas:\n");
    printf("%s: %d tiros, %d acertos\n", g->p1.name, g->p1.total_shots, g->p1.total_hits);
    printf("%s: %d tiros, %d acertos\n", g->p2.name, g->p2.total_shots, g->p2.total_hits);

    destroy_game(g);

    pause_screen();
}

// MAIN PRINCIPAL

int main(void) {
    int board_size = 10;

    while (1) {
        clear_screen();

        printf("===== BATALHA NAVAL =====\n\n");
        printf("1 - Jogar\n");
        printf("2 - Configurações\n");
        printf("0 - Sair\n\n");

        printf("Escolha: ");
        int op;
        scanf("%d", &op);
        getchar(); 

        if (op == 0) {
            clear_screen();
            printf("Encerrando...\n");
            break;
        }

        switch (op) {
        case 1:
            play_game(board_size);
            break;

        case 2:
            settings_menu(&board_size);
            break;

        default:
            printf("Opção inválida!\n");
            pause_screen();
            break;
        }
    }

    return 0;
}
