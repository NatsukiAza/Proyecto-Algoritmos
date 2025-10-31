#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define ERR 1
#define SIN_MEM 2
#define ERR_DUP 3
#define ERR_ARCH 4
#define OK 0
typedef struct sNodo{
    void* info;
    unsigned tamInfo;
    struct sNodo* izq;
    struct sNodo* der;
} tNodo;

typedef tNodo* tArbol;

void crearArbol(tArbol* arbol);
int insertarArbolIte(tArbol* arbol, const void* info, unsigned tam, int (*cmp)(const void* valor1, const void* valor2));
int insertarArbolRecu(tArbol* arbol, const void* info, unsigned tam, int (*cmp)(const void* valor1, const void* valor2));
void IRD(const tArbol *arbol, void* param, int nivel, void (*accion)(tArbol arbol, void* param, int nivel));
void RID(const tArbol *arbol, void* param, int nivel, void (*accion)(tArbol arbol, void* param, int nivel));
void IDR(const tArbol *arbol, void* param, int nivel, void (*accion)(tArbol arbol, void* param, int nivel));
int contarHojas(tArbol *arbol);
int vaciarArbol(tArbol* arbol);
int alturaArbol(tArbol* arbol);
void verNodoDesdeN(tArbol* arbol, int nivelMin);
void verNodoNivel(tArbol* arbol, int nivelParam);
void verNodoHastaN(tArbol* arbol, int nivelMax);
void esHoja(tArbol* arbol, void* param, int nivel);
void eliminar(tArbol* arbol, void* elim, int nivel);
void contarAltura(tArbol* arbol, void* param, int nivel);
void mostrarNodoHasta(tArbol* arbol, void* param, int nivel);
void mostrarNodoNivel(tArbol* arbol, void* param, int nivel);
void mostrarNodoDesde(tArbol* arbol, void* param, int nivel);
int esCompleto(tArbol* arbol);
unsigned cantNodosArbolBin(const tArbol* a);
tNodo** buscarNodoArbolBinBusq(tArbol* a, const void* elem, int (*cmp)(const void*, const void*));


#endif // HEADER_H_INCLUDED
