#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED

#include <stdlib.h>

typedef struct sNodo {
    void *info;
    unsigned tamInfo;
    struct sNodo *sig;
} tNodo;

typedef struct {
    tNodo *pri;
    tNodo *ult;
} tCola;

void crearCola(tCola *c);
int colaVacia(tCola *c);
int encolar(tCola *c, const void *dato, unsigned cantBytes);
int desencolar(tCola *c, void *dato, unsigned cantBytes);
void vaciarCola(tCola *c);

#endif // COLA_H_INCLUDEDs
