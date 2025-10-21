#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "generador.h"

int menu();
int actualizarRanking(const char*, tJugador, int, int(*cmp)(const void*, const void*));

#endif // MENU_H_INCLUDED
