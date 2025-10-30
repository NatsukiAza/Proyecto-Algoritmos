#include "cola.h"
#include <string.h>

void crearCola(tCola *c) {
    c->pri = NULL;
    c->ult = NULL;
}

int colaVacia(tCola *c) {
    return c->pri == NULL;
}

int encolar(tCola *c, const void *dato, unsigned cantBytes) {
    tNodo *nuevoNodo = (tNodo *)malloc(sizeof(tNodo));
    if (!nuevoNodo) {
        return 0; // Error de memoria
    }
    nuevoNodo->info = malloc(cantBytes);
    if (!nuevoNodo->info) {
        free(nuevoNodo);
        return 0; // Error de memoria
    }
    memcpy(nuevoNodo->info, dato, cantBytes);
    nuevoNodo->tamInfo = cantBytes;
    nuevoNodo->sig = NULL;

    if (c->ult) {
        c->ult->sig = nuevoNodo;
    } else {
        c->pri = nuevoNodo;
    }
    c->ult = nuevoNodo;
    return 1;
}

int desencolar(tCola *c, void *dato, unsigned cantBytes) {
    if (!c->pri) {
        return 0; // Cola vacía
    }
    tNodo *nodoAEliminar = c->pri;
    memcpy(dato, nodoAEliminar->info, nodoAEliminar->tamInfo);
    c->pri = nodoAEliminar->sig;

    if (!c->pri) {
        c->ult = NULL;
    }

    free(nodoAEliminar->info);
    free(nodoAEliminar);
    return 1;
}

void vaciarCola(tCola *c) {
    while (c->pri) {
        tNodo *nodoAEliminar = c->pri;
        c->pri = nodoAEliminar->sig;
        free(nodoAEliminar->info);
        free(nodoAEliminar);
    }
    c->ult = NULL;
}
