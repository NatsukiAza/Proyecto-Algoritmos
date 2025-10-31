#ifndef FUNCIONESCLISERV_H_INCLUDED
#define FUNCIONESCLISERV_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TDAvectorDin.h"

typedef struct{
    char nombre[20];
    int puntaje;
}tRanking;

typedef struct{
    unsigned indice;
    char nombre[20];
}tIndice;

typedef struct
{
    char nombre[20];
    int cantMov;
    int puntos;
}tJugador;

int cmpRanking(const void *puntaje1, const void *puntaje2);
int cmpJugador(const void *, const void *);
int actualizarRanking(tVector* vec, const char* nombre, int nuevaPunt);
int guardarRanking(tVector* vec, FILE* fp);
int actualizarRankingJugador(tVector* vec, const char* nombre, int nuevaPunt, int(*cmp)(const void*, const void*));
void rankingServidor(char* text);
void actualizarBDD(char datos[], tArbol* arbol, tVector* vec, char* text);
void verJugadores(char* text);
int crearArchivoJugadores(const char* archi);

#endif // FUNCIONESCLISERV_H_INCLUDED
