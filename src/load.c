#include "load.h"
#include "state.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <raylib.h>
#include <assert.h>
#include <stdlib.h>

level *level_file(char *file)
{
    int num_linea = -1;         // Se usa para saber en que lina se esta.
    int dim = 0;                // Contador que sirve de puntero para calcuar posicion de las dimensiones de level.
    int numero = 0;             // Numero que encuentra.
    int count_x_y = 0;          // Para saber si el numero se encontro es de x o y de la dimenciones de level.
    int size_x = 0, size_y = 0; // tamños respectivos de level.

    //Se pide memoria para lvl.
    level *lvl = malloc(sizeof(level));

    //Se abre el archivo file.
    FILE *fich;
    fich = fopen(file, "r");

    char linea[1024];
    while (fgets(linea, 1024, (FILE *)fich))
    {
        num_linea++;

        // Array de char que guarda las dimensiones.
        char simbolo[strlen(linea)];

        if (num_linea == 0)
        {

            for (int i = 0; i < strlen(linea); i++)
            {
                if (linea[i] != ' ' && linea[i] != '\n')
                {
                    simbolo[i - dim] = linea[i];
                }
                else
                {
                    simbolo[i - dim] = '\0';

                    numero = 0;

                    for (int it = 0; it < strlen(simbolo); it++)
                    {
                        //Se convierte de un char a int.
                        //A simbolo se le resta '0' y esto da el valor numerico del digito
                        //Luego se eleva a la postenecia de 10 dependiendo de su posicion.
                        numero = numero + (simbolo[it] - '0') * pow(10, i - dim - it - 1);
                    }

                    dim = i + 1;

                    if (count_x_y == 0)
                    {
                        size_x = numero;
                        count_x_y++;
                    }
                    else
                    {
                        size_y = numero;
                    }

                    if (size_x != 0 && size_y != 0)
                    {
                        //Se asigna las dimenciones de lvl.
                        lvl->size_x = size_x;
                        lvl->size_y = size_y;

                        //Se pide memoria.
                        lvl->cells = malloc(lvl->size_y * sizeof(char *));

                        for (int y = 0; y < lvl->size_y; y++)
                        {
                            lvl->cells[y] = malloc(lvl->size_x * sizeof(char));
                        }

                        //Se asigna por defecto todas las posiciones en '.'.
                        for (int y = 0; y < lvl->size_y; y++)
                        {
                            for (int x = 0; x < lvl->size_x; x++)
                            {
                                lvl->cells[y][x] = '.';
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if (num_linea <= size_y)
            {
                for (int i = 0; i < 2 * size_x - 1; i++)
                {
                    if (linea[i] == ' ')
                    {
                        continue;
                    }

                    else if (linea[i] != '.')
                    {
                        lvl->cells[num_linea - 1][i / 2] = linea[i];
                    }
                }
            }
        }
    }

    fclose(fich);
    return lvl;
}

level *options_level(char *argv)
{
    level *lvl;
    if (argv != NULL)
    {
        lvl = level_file(argv);
    }
    else
    {
        lvl = level_new(50, 40);
        level_fill_random(lvl, 6);
    }
    return lvl;
}

state *state_file(level *lvl)
{
    state *sta = state_new();

    int n_enemiges = ContarEnemigos(lvl);

    for (int i = 0; i < lvl->size_y; i++)
    {
        for (int j = 0; j < lvl->size_x; j++)
        {
            if (lvl->cells[i][j] == 'p' || lvl->cells[i][j] == 'm' || lvl->cells[i][j] == 'b')
            {
                if (lvl->cells[i][j] == 'p')
                {
                    sta->pla.ent.x = TILE_SIZE * (j + 0.5);
                    sta->pla.ent.y = TILE_SIZE * (i + 0.5);

                    sta->pla.ent.rad = PLAYER_RAD;
                    sta->pla.ent.hp = PLAYER_HP;
                }

                else if (((lvl->cells[i][j] == 'b') || (lvl->cells[i][j] == 'm')) && sta->n_enemies <= n_enemiges && n_enemiges <= MAX_ENEMIES)
                {
                    enemy *new_enemy = &sta->enemies[sta->n_enemies];
                    sta->n_enemies++;

                    memset(new_enemy, 0, sizeof(enemy));

                    new_enemy->ent.x = TILE_SIZE * (j + 0.5);
                    new_enemy->ent.y = TILE_SIZE * (i + 0.5);

                    if (lvl->cells[i][j] == 'm')
                    {
                        new_enemy->kind = MINION;
                        new_enemy->ent.hp = BRUTE_HP;
                        new_enemy->ent.rad = BRUTE_RAD;
                    }
                    else
                    {
                        new_enemy->kind = BRUTE;
                        new_enemy->ent.hp = MINION_HP;
                        new_enemy->ent.rad = MINION_RAD;
                    }
                }
            }
        }
    }

    return sta;
}

state *option_state(level *lvl, char *argv)
{
    state *sta;
    if (argv != NULL)
    {
        sta = state_file(lvl);
    }
    else
    {
        sta = state_new();
        state_populate_random(lvl, sta, 40);
    }
    return sta;
}

int ContarEnemigos(level *lvl)
{
    int count = 0;
    for (int i = 0; i < lvl->size_y; i++)
    {
        for (int j = 0; j < lvl->size_x; j++)
        {
            if ((lvl->cells[i][j] == 'b') || (lvl->cells[i][j] == 'm'))
            {
                count++;
            }
        }
    }
    return count;
}
