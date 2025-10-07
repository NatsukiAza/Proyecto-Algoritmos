#ifndef MOVIMIENTO_H_INCLUDED
#define MOVIMIENTO_H_INCLUDED
#define PLAYER 'J'
#include "generador.h"
#include "cola.h"
#include <conio.h>

int terminarJuego(tLaberinto * l);
void ingresarMovimiento(tCola * colaJugador, int entradaFil, int entradaCol);
int handleMovimiento(char **lab, int entradaFil, int entradaCol);

#endif // MOVIMIENTO_H_INCLUDED
