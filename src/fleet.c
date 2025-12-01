#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fleet.h"
#include "board.h"

//Adiciona navio usando realloc

static void add_ship(Fleet *f, const char *name, int length) {
    f->count++;
    f->ships = realloc(f->ships, f->count * sizeof(Ship));

    if (!f->ships) {
        printf("Erro de memória ao adicionar navio!\n");
        exit(1);
    }

    Ship *s = &f->ships[f->count - 1];

    strcpy(s->name, name);
    s->length = length;
    s->hits = 0;
    s->placed = 0;
}

// Frota mínima padrão

static void add_minimum_fleet(Fleet *f) {
    add_ship(f, "Porta-avioes", 5);
    add_ship(f, "Encouracado", 4);
    add_ship(f, "Cruzador", 3);
    add_ship(f, "Cruzador", 3);
    add_ship(f, "Destroyer", 2);
    add_ship(f, "Destroyer", 2);
}

// Criação da frota padrão

Fleet *create_default_fleet(void) {
    Fleet *f = malloc(sizeof(Fleet));
    if (!f) {
        printf("Erro ao alocar Fleet!\n");
        exit(1);
    }

    f->ships = NULL;
    f->count = 0;

    add_minimum_fleet(f);

    return f;
}


// Intervalo 11–15

static void add_interval_2(Fleet *f) {
    add_minimum_fleet(f);
    add_ship(f, "Cruzador", 3);
    add_ship(f, "Destroyer", 2);
}


// Intervalo 16–20

static void add_interval_3(Fleet *f) {
    add_ship(f, "Porta-avioes", 5);
    add_ship(f, "Porta-avioes", 5);

    add_ship(f, "Encouracado", 4);
    add_ship(f, "Encouracado", 4);
    add_ship(f, "Encouracado", 4);

    for (int i = 0; i < 6; i++) add_ship(f, "Cruzador", 3);
    for (int i = 0; i < 6; i++) add_ship(f, "Destroyer", 2);
}


// Intervalo 21–26

static void add_interval_4(Fleet *f) {
    for (int i = 0; i < 5; i++) add_ship(f, "Porta-avioes", 5);
    for (int i = 0; i < 8; i++) add_ship(f, "Encouracado", 4);
    for (int i = 0; i < 14; i++) add_ship(f, "Cruzador", 3);
    for (int i = 0; i < 18; i++) add_ship(f, "Destroyer", 2);
}

// Cria frota dinâmica

Fleet *create_fleet(int boardSize) {
    Fleet *f = malloc(sizeof(Fleet));
    if (!f) {
        printf("Erro ao alocar Fleet!\n");
        exit(1);
    }

    f->ships = NULL;
    f->count = 0;

    if (boardSize >= 6 && boardSize <= 10) {
        add_minimum_fleet(f);

    } else if (boardSize >= 11 && boardSize <= 15) {
        add_interval_2(f);

    } else if (boardSize >= 16 && boardSize <= 20) {
        add_interval_3(f);

    } else if (boardSize >= 21 && boardSize <= 26) {
        add_interval_4(f);

    } else {
        printf("Erro: tamanho inválido em create_fleet.\n");
        exit(1);
    }

    return f;
}
// Libera memória da frota

void destroy_fleet(Fleet *fleet) {
    if (!fleet) return;

    if (fleet->ships)
        free(fleet->ships);

    free(fleet);
}

