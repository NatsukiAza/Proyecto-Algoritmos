#include "TDAvectorDin.h"
#include "servidor_lib.h"
#include <stdlib.h>

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

int cmpRanking(const void *puntaje1, const void *puntaje2){
    return *((int*)puntaje1) - *((int*)puntaje2);
}

int vectorOrdenarBurbujeo(tVector* vec, int(cmp)(const void *a, const void *b)) {
    if (!vec || !vec->info || vec->tam == 0 || vec->ce < 2) return 1;

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

int actualizarRankingJugador(tVector* vec, const char* nombre, int nuevaPunt, int(*cmp)(const void*, const void*))
{
    if (!vec || !vec->info)
        return ERR;
    for (size_t i = 0; i < vec->ce; i++)
    {
        tRanking* rankTmp = (tRanking*)(vec->info + i * vec->tam);
        if (strcmp(rankTmp->nombre, nombre) == 0)
         {
            TRACE("encontre jugador en el rank");
            // si encontro y la nuevaPunt es mayor a la actual, actualiza
            if(nuevaPunt > rankTmp->puntaje){
                rankTmp->puntaje = nuevaPunt;
                vectorOrdenarBurbujeo(vec, cmp);
                TRACE("y actualice");
            }
            TRACE("no actualice");
            return OK; //me voy porq ya encontre el user
        }
    }
    TRACE("no encontre jugador en rank");
    return ERR;
}
