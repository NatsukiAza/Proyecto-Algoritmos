#ifndef GENERADOR_H_INCLUDED
#define GENERADOR_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define PARED '#'
#define CAMINO '.'
#define FANTASMA 'F'
#define VIDA 'V'

typedef struct{
    int x;
    int y;
}Fantasma;


void rellenarBordes(char ** lab,int n);
void seleccionarAccesos(char ** lab,int n,int * sX,int * sY,int * eX,int * eY);
void generarCaminoAleatorio(char **lab, int n, int eX, int eY, int sX, int sY);
void imprimirLaberinto(char **lab, int n);
void generarFantasmas(char **lab, int n, int eX, int eY, int cantidadFantasmas,Fantasma f[]);
void moverFantasmas(char ** lab,int n,Fantasma f[],int cantidadFantasmas);
void generarVidasExtra(char** lab, int n, int cantVidas);

#endif // GENERADOR_H_INCLUDED
