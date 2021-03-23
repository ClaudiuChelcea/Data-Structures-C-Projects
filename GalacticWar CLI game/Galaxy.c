#include "Galaxy.h"

// Initialise galaxy
galaxy_t* init_galaxy() {
    galaxy_t* new_galaxy = NULL;
    new_galaxy = malloc(sizeof(galaxy_t));
    DIE(!new_galaxy, "Couldn't create galaxy!\n");
    new_galaxy->size = 0;
    new_galaxy->head = NULL;
    return new_galaxy;
}

