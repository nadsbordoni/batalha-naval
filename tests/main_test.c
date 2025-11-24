#include <stdio.h>
#include <stdlib.h>

#include "../src/board.h"
#include "../src/fleet.h"

int main() {
    printf("=== TESTE DO PROJETO ===\n\n");

    /* 1. Criar tabuleiro */
    Board *b = create_board(10); 
    if (!b) {
        printf("Erro ao criar o tabuleiro!\n");
        return 1;
    }

    printf("Tabuleiro 10x10 criado com sucesso!\n");
    printf("\nEstado inicial (showShips = true):\n\n");
    print_board(b, true);

    /* 2. Criar frota */
    Fleet *f = create_default_fleet();
    if (!f) {
        printf("Erro ao criar frota!\n");
        destroy_board(b);
        return 1;
    }

    printf("\nFrota criada com sucesso (%d navios):\n", f->totalShips);
    for (int i = 0; i < f->totalShips; i++) {
        printf(" - %s (%d células)\n", f->ships[i].name, f->ships[i].length);
    }

    /* 3. Posicionar automaticamente a frota */
    printf("\nPosicionando frota automaticamente...\n");

    if (!place_fleet_random(b, f)) {
        printf("Erro: não foi possível posicionar a frota!\n");
        destroy_board(b);
        destroy_fleet(f);
        return 1;
    }

    printf("Frota posicionada!\n");

    printf("\nTabuleiro com os navios (showShips = true):\n\n");
    print_board(b, true);

    printf("\nTabuleiro ocultando navios (showShips = false):\n\n");
    print_board(b, false);

    /* 4. Teste de acesso a célula */
    printf("\nTestando acesso à célula (0,0)...\n");
    Cell *cell = get_cell(b, 0, 0);
    if (cell) {
        printf("Célula (0,0) -> state=%d, ship_id=%d\n",
               cell->state, cell->ship_id);
    } else {
        printf("Erro: get_cell retornou NULL.\n");
    }

    /* 5. Finalização */
    destroy_board(b);
    destroy_fleet(f);

    printf("\nTestes concluídos e memória liberada.\n");
    printf("=== FIM DO TESTE ===\n");

    return 0;
}