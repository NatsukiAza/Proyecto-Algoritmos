#include <stdlib.h>
#include "TDAvectorDin.h"
#include "servidor_lib.h"
#include "funcionesCliServ.h"

int vectorInsertarOrdenado(tVector* vec, const void* clave, int(*cmp)(const void*, const void*))
{
    size_t i = 0;
    while(i < vec->ce && cmp((vec->info+i*vec->tam), clave)>0)
    {
        i++;
    }

    if (vec->ce < vec->cap)
    {
        memmove(vec->info + (i+1) * vec->tam, vec->info + i * vec->tam, (vec->ce - i) * vec->tam);
        memcpy(vec->info + i * vec->tam, clave, vec->tam);
        vec->ce++;
    }
    else if (i < vec->cap)
    {
        memmove(vec->info + (i + 1) * vec->tam, vec->info + i * vec->tam, (vec->cap - i - 1) * vec->tam);
        memcpy(vec->info + i * vec->tam, clave, vec->tam);
    }

    return OK;
}

int vectorCrear(tVector* vec, size_t tam, size_t cap)
{
    vec->info = malloc(tam*cap);
    if(!vec->info)
        return SIN_MEM;
    vec->ce = 0;
    vec->tam = tam;
    vec->cap = cap;
    return OK;
}

void vectorDestruir(tVector* vec)
{
    free(vec->info);
    vec->info = NULL;
    vec->ce = 0;
    vec->tam = 0;
    vec->cap = 0;
}

int vectorOrdenarBurbujeo(tVector* vec, int(cmp)(const void *a, const void *b)) {
    if (!vec || !vec->info || vec->tam == 0 || vec->ce == 1) return 1;

    void *baseTmp = malloc(vec->tam);
    if (!baseTmp) return 0;

    size_t ult = vec->ce - 1;
    while (ult > 0) {
        size_t nuevo_ult = 0;              // última posición donde hubo swap
        for (size_t i = 0; i < ult; ++i) {
            void* a = vec->info + i * vec->tam;
            void* b = vec->info + (i+1) * vec->tam;
            if (cmp(a, b) > 0) {          // estricto > para mantener estabilidad
                memcpy(baseTmp, a, vec->tam);
                memcpy(a,   b, vec->tam);
                memcpy(b, baseTmp, vec->tam);
                nuevo_ult = i;
            }
        }
        ult = nuevo_ult;
    }

    free(baseTmp);
    return 1;
}

