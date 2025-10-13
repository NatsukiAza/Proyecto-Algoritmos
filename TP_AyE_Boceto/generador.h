#ifndef GENERADOR_H_INCLUDED
#define GENERADOR_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

#define PARED '#'
#define CAMINO '.'
#define FANTASMA 'F'
#define PLAYER 'J'
#define GANO 0
#define PERDIO 1
#define JUGANDO 2
#define MIN(x,y) ((x)<(y) ? (x) : (y))

typedef struct{
    int vidas;
    int fil;
    int col;
}tJugador;

typedef struct{
    int fil;
    int col;
}tFantasma;
typedef struct{
    int fil;
    int col;
}tVida;
typedef struct{
    int fil;
    int col;
}tPremio;

typedef struct{
    char ** lab;
    int estado;
    int cantFantasmas;
    int maxVidasExtra;
    int maximosPremios;
    int filas;
    int columnas;
    int entradaX,entradaY;
    int salidaX,salidaY;
    tFantasma * fantasmas;
    tVida * vidas;
    tPremio * premios;
    tJugador jugador;
}tLaberinto;

//void rellenarBordes(char ** lab,int n);
void seleccionarAccesos(tLaberinto * l);
void generarCaminoAleatorio(tLaberinto * l,int entradaX,int entradaY);
void imprimirLaberinto(char **lab, int col,int fil);
void generarFantasmas(tLaberinto * l);
void generarVidas(char ** lab,tFantasma * fantasmas,int n);
int moverFantasmas(char ** lab,tFantasma f[],int cantidadFantasmas);

#endif // GENERADOR_H_INCLUDED
