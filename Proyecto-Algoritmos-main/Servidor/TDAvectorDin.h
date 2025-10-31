#ifndef TDAVECTORDIN_H_INCLUDED
#define TDAVECTORDIN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arbol.h"
typedef struct
{
    void* info;
    size_t ce;
    size_t tam;
    size_t cap;

}tVector;

int vectorCrear(tVector* vec, size_t tam, size_t cap);
int vectorInsertarOrdenado(tVector* vec, const void* clave, int(*cmp)(const void*, const void*));
void vectorDestruir(tVector* vec);
int vectorOrdenarBurbujeo(tVector* vec, int(cmp)(const void *a, const void *b));


#endif // TDAVECTORDIN_H_INCLUDED
