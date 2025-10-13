#ifndef MOVIMIENTO_H_INCLUDED
#define MOVIMIENTO_H_INCLUDED

#include "generador.h"
#include <conio.h>
#include <windows.h>

typedef struct {
    char **lab;
    int fil;
    int col;
    int cantFant;
    tFantasma *fantasmas;
    int *running;
    int *estado;
} ThreadArgs;

DWORD WINAPI hiloFantasmas(LPVOID arg);
void ingresarJugador(tLaberinto * l);
int terminarJuego(tLaberinto * l);
int handleMovimiento(tLaberinto * l);

#endif // MOVIMIENTO_H_INCLUDED
