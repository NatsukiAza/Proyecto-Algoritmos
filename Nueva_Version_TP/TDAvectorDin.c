#include "TDAvectorDin.h"
#include "generador.h"

int vectorInsertarOrdenado(tVector* vec, const void* clave, int(*cmp)(const void*, const void*))
{
    size_t i = 0;
    while(i < vec->ce && cmp((vec->info+i*vec->tam), clave)>=0)
    {
        i++;
    }

    memmove(vec->info + (i+1) * vec->tam, vec->info + i * vec->tam, (vec->ce - i) * vec->tam);
    memcpy(vec->info + i * vec->tam, clave, vec->tam);
    vec->ce++;

    return TODO_OK;
}

int vectorCrear(tVector* vec, size_t tam, size_t cap)
{
    vec->info = malloc(tam*cap);
    if(!vec->info)
        return SIN_MEM;
    vec->ce = 0;
    vec->tam = tam;
    vec->cap = cap;
    return TODO_OK;
}

void vectorDestruir(tVector* vec)
{
    free(vec->info);
    vec->info = NULL;
    vec->ce = 0;
    vec->tam = 0;
    vec->cap = 0;
}
