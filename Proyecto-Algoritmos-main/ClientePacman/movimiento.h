#ifndef MOVIMIENTO_H_INCLUDED
#define MOVIMIENTO_H_INCLUDED
#define PLAYER 'J'
#include "generador.h"
#include "cola.h"
#include <conio.h>

//int terminarJuego(tLaberinto * l);
//void ingresarMovimiento(tCola * colaJugador, int entradaFil, int entradaCol, int*, int*);
//int handleMovimiento(char **lab, int entradaFil, int entradaCol, int*, int*);

int terminarJuego(tLaberinto *l);
void ingresarMovimiento(tCola *colaJugador, tLaberinto *l);
int iniciarLogMovimientos(const char* archi, char nombre[]);
int handleMovimiento(tLaberinto *l);
void borrarFantasmaEn(tLaberinto *l, int fil, int col);

#endif // MOVIMIENTO_H_INCLUDED
