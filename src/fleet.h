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

// Cria frota padrão
Fleet *create_default_fleet(void);
// Cria frota tabuleiro personalizado
Fleet *create_fleet(int boardSize);

// Libera frota criada dinamicamente
void destroy_fleet(Fleet *fleet);

#endif

