#ifndef LOAD_H
#define LOAD_H

#include "string.h"
#include "level.h"
#include "state.h"

// A partir de un archivo de texto crea un level.
level *level_file(char *file);

// Se crea un nuevo estado y se posiciona el player y sus enemigos.
state *state_file(level *lvl);

// Analiza si se entrego un PathFile si es asi se llama a la funcion level_file(...) sino se ejecuta normalmente.
state *option_state(level *lvl, char *argv);

// Analiza si se entrego un PathFile si es asi se llama a state_file(...) sino se ejecuta normalmente.
level *options_level(char *argv);

// Se encarga de contar los enemigos en el mapa.
int ContarEnemigos(level *lvl);

#endif