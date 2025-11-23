#include "fleet.h"
#include <stdlib.h>
#include <string.h>

Fleet *create_default_fleet() {
    // Aloca memória para a estrutura Fleet
    Fleet *fleet = malloc(sizeof(Fleet));
    if (!fleet) return NULL;

    // Define o número total de navios de uma frota padrão
    fleet->totalShips = 6;
    fleet->ships = malloc(fleet->totalShips * sizeof(Ship));
    if (!fleet->ships) {
        free(fleet);
        return NULL;
    }

    // Preenche a frota padrão
    strcpy(fleet->ships[0].name, "Porta-avioes");
    fleet->ships[0].length = 5;

    strcpy(fleet->ships[1].name, "Encouracado");
    fleet->ships[1].length = 4;

    strcpy(fleet->ships[2].name, "Cruzador 1");
    fleet->ships[2].length = 3;

    strcpy(fleet->ships[3].name, "Cruzador 2");
    fleet->ships[3].length = 3;

    strcpy(fleet->ships[4].name, "Destroyer 1");
    fleet->ships[4].length = 2;

    strcpy(fleet->ships[5].name, "Destroyer 2");
    fleet->ships[5].length = 2;

    // Inicializa hits e placed para todos os navios em zero porque não foram atingidos e nem colocados ainda
    for (int i = 0; i < fleet->totalShips; i++) {
        fleet->ships[i].hits = 0;
        fleet->ships[i].placed = 0;
    }

    return fleet;
}

void destroy_fleet(Fleet *fleet) {
    if (!fleet) return;

    free(fleet->ships);
    free(fleet);        
}
