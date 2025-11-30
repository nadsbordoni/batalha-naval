#ifndef FLEET_H
#define FLEET_H

// Estrutura de um navio padrão
typedef struct {
    char name[20];
    int length;
    int hits;
    int placed;
} Ship;

// Estrutura da frota
typedef struct {
    Ship *ships;
    int count;
} Fleet;

// Cria e retorna ponteiro para frota padrão (tabuleiro 10x10)
Fleet *create_default_fleet(void);

// Cria e retorna ponteiro para frota proporcional (tamanho 6–26)
Fleet *create_fleet(int boardSize);

// Libera frota criada dinamicamente
void destroy_fleet(Fleet *fleet);

#endif

