#ifndef MOVIMIENTO_H_INCLUDED
#define MOVIMIENTO_H_INCLUDED
#define PLAYER 'J'
#include "generador.h"
#include <conio.h>

void ingresarJugador(char **lab, int entradaFil, int entradaCol);
int terminarJuego(char **lab, int entradaFil, int entradaCol, int salidaFil, int salidaCol, int n, int cantFant, Fantasma *fantasmas, int* cantVidas, int* cantVidasExtra);
int handleMovimiento(char **lab, int entradaFil, int entradaCol, int* cantVidas, int* cantVidasExtra);

#endif // MOVIMIENTO_H_INCLUDED
