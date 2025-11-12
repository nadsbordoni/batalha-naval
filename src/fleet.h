#ifndef FLEET_H
#define FLEET_H

//estrutura de um navio padrão
typedef struct {
    char name[20];
    int length;
    int hits;
    int placed;
} Ship;

typedef struct {
    Ship *ships;
    int totalShips;
} Fleet;

//nós temos uma frota padrão esperada pelo professor, essa função será responsável por isso
Fleet *create_default_fleet();
//Função para liberar a memória alocada para a frota
void destroy_fleet(Fleet *fleet);

#endif