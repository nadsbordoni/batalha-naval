#include <stdio.h>
#include <stdlib.h>
#include "io.h"

int main(void) {
    int board_size = 10;

    while (1) {
        clear_screen();

        printf("Batalha Naval\n\n");
        printf("1 - Jogar\n");
        printf("2 - Configurações\n");
        printf("0 - Sair\n\n");

        printf("Opção: ");
        int op;
        if (scanf("%d", &op) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }

        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}

        if (op == 0) {
            clear_screen();
            printf("Saindo...\n");
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
                printf("Opção inválida.\n");
                pause_screen();
                break;
        }
    }

    return 0;
}