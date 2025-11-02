#include "arbol.h"

void crearArbol(tArbol* arbol){
    *arbol =  NULL;
}

int insertarArbolIte(tArbol* arbol, const void* info, unsigned tam, int (*cmp)(const void* valor1, const void* valor2)){
    tNodo* nodo;
    while(*arbol){
        if(cmp((*arbol)->info, info) < 0){
            arbol = &(*arbol)->der;
        } else if(cmp((*arbol)->info, info) > 0){
            arbol = &(*arbol)->izq;
        } else{
            return ERR_DUP;
        }
    }
    nodo = (tNodo*)malloc(sizeof(tNodo));
    if(!nodo){
        return SIN_MEM;
    }
    nodo->info = malloc(tam);
    if(!(nodo->info)){
        free(nodo);
        return SIN_MEM;
    }
    memcpy(nodo->info, info, tam);
    nodo->tamInfo = tam;
    nodo->der = NULL;
    nodo->izq = NULL;
    *arbol = nodo;
    return OK;
}

int insertarArbolRecu(tArbol* arbol, const void* info, unsigned tam, int (*cmp)(const void* valor1, const void* valor2)){
    tNodo* nodo;
    if(*arbol){
        if(cmp((*arbol)->info, info) < 0){
            insertarArbolRecu(&(*arbol)->der, info, tam, cmp);
        } else if(cmp((*arbol)->info, info) > 0){
            insertarArbolRecu(&(*arbol)->izq, info, tam, cmp);
        } else {
            return ERR_DUP;
        }
    } else {
        nodo = (tNodo*)malloc(sizeof(tNodo));
        if(!nodo){
            return SIN_MEM;
        }
        nodo->info = malloc(tam);
        if(!(nodo->info)){
            free(nodo);
            return SIN_MEM;
        }
        memcpy(nodo->info, info, tam);
        nodo->tamInfo = tam;
        nodo->der = NULL;
        nodo->izq = NULL;
        *arbol = nodo;
        return OK;
    }
    return ERR;
}

void IRD(const tArbol *arbol, void* param, int nivel, void (*accion)(tArbol arbol, void* param, int nivel)){
    if(*arbol){
        IRD(&(*arbol)->izq, param, nivel+1, accion);
        accion(*arbol, param, nivel);
        IRD(&(*arbol)->der, param, nivel+1, accion);
    }
}

void RID(const tArbol *arbol, void* param, int nivel, void (*accion)(tArbol arbol, void* param, int nivel)){
    if(*arbol){
        accion(*arbol, param, nivel);
        RID(&(*arbol)->izq, param, nivel+1, accion);
        RID(&(*arbol)->der, param, nivel+1, accion);
    }
}

void IDR(const tArbol *arbol, void* param, int nivel, void (*accion)(tArbol arbol, void* param, int nivel)){
    if(*arbol){
        IDR(&(*arbol)->izq, param, nivel+1, accion);
        IDR(&(*arbol)->der, param, nivel+1, accion);
        accion(*arbol, param, nivel);
    }
}

int busquedaRID(const tArbol *arbol, void* param, int nivel, int flag, int (*accion)(tArbol arbol, void* param, int nivel)){
    if(*arbol && flag){
        if(accion(*arbol, param, nivel)){
            flag = 0;
        } else if(busquedaRID(&(*arbol)->izq, param, nivel+1, flag, accion)){
            flag = 0;
        } else if(busquedaRID(&(*arbol)->izq, param, nivel+1, flag, accion)){
            flag = 0;
        }
    }
    return flag;
}

void esHoja(tArbol* arbol, void* param, int nivel){
    int hojas = *(int*)param;
    if((*arbol)->izq == NULL && (*arbol)->der == NULL){
        hojas++;
    }
    memcpy(param, &hojas, sizeof(int));
}

void eliminar(tArbol* arbol, void* elim, int nivel){
    if((*arbol)->izq == NULL && (*arbol)->der == NULL){
        free((*arbol)->info);
        free(*arbol);
        *arbol = NULL;
        (*(int*)elim)++;
    }
}

void eliminarDesde(tArbol* arbol, void* min, int nivel){
    if(*(int*)min <= nivel){
        if((*arbol)->izq == NULL && (*arbol)->der == NULL){
            free((*arbol)->info);
            free(*arbol);
            *arbol = NULL;
        }
    }
}

void contarAltura(tArbol* arbol, void* param, int nivel){
    int max = *(int*)param;
    if(!max || nivel > max){
        max = nivel;
        memcpy(param, &nivel, sizeof(int));
    }

}

void mostrarNodoHasta(tArbol* arbol, void* param, int nivel){
    if(*(int*)param >= nivel){
        printf("Nivel: %d, InfoNodo: %d \n", nivel, *(int*)(*arbol)->info);
    }
}

void mostrarNodoNivel(tArbol* arbol, void* param, int nivel){
    if(*(int*)param == nivel){
        printf("Nivel: %d, InfoNodo: %d \n", nivel, *(int*)(*arbol)->info);
    }
}

void mostrarNodoDesde(tArbol* arbol, void* param, int nivel){
    if(*(int*)param <= nivel){
        printf("Nivel: %d, InfoNodo: %d \n", nivel, *(int*)(*arbol)->info);
    }
}

void contar(tArbol* arbol, void* param, int nivel){
    (*(int*)param)++;
}

//int vaciarArbol(tArbol* arbol){
//    int elim = 0, nivel = 0;
//    IDR(arbol, &elim, nivel, eliminar);
//    return elim;
//}

//int podarArbolDesde(tArbol* arbol, int nivelMin){
//    int elim = 0, nivel = 0;
//    IDR(arbol, &nivelMin, nivel, eliminarDesde);
//    return OK;
//}
//
//int contarHojas(tArbol* arbol){
//    int hojas = 0, nivel = 0;
//    RID(arbol, &hojas, nivel, esHoja);
//    return hojas;
//}
//
//int contarNodos(tArbol* arbol){
//    int nodos = 0, nivel = 0;
//    RID(arbol, &nodos, nivel, contar);
//    return nodos;
//}
//
//int alturaArbol(tArbol* arbol){
//    int altura = 0, nivel = 0;
//    RID(arbol, &altura, nivel, contarAltura);
//    return altura+1;
//}
//
//void verNodoHastaN(tArbol* arbol, int nivelMax){
//    int nivel = 0;
//    RID(arbol, &nivelMax, nivel, mostrarNodoHasta);
//}
//
//void verNodoNivel(tArbol* arbol, int nivelParam){
//    int nivel = 0;
//    RID(arbol, &nivelParam, nivel, mostrarNodoNivel);
//}
//
//void verNodoDesdeN(tArbol* arbol, int nivelMin){
//    int nivel = 0;
//    RID(arbol, &nivelMin, nivel, mostrarNodoDesde);
//}
//
//int esCompleto(tArbol* arbol){
//    int altura, nodos;
//    altura = alturaArbol(arbol);
//    nodos = contarNodos(arbol);
//    if(nodos+1 == pow(2, altura)){
//        return OK;
//    }else{
//        return ERR;
//    }
//}

tNodo** buscarNodoArbolBinBusq(tArbol* a, const void* elem, int (*cmp)(const void*, const void*))
{
    int rc;
    while(*a && (rc = cmp(elem, (*a)->info)))
    {
        if(rc < 0)
            a = &((*a)->izq);
        else
            a = &((*a)->der);
    }
    if(!*a)
        return NULL;
    return (tNodo**)a;
}

unsigned cantNodosArbolBin(const tArbol* a)
{
    if(!*a)
        return 0;
    return cantNodosArbolBin(&((*a)->izq)) +
        cantNodosArbolBin(&((*a)->der)) + 1;
}

int cargarDesdeDatosOrdenadosRec(tArbol *p, void *source, unsigned (*leer)(void **, void *, unsigned, void *params), int li, int ls, void *params)
{
    int m = (li + ls) / 2, r;

    if (li > ls)
        return 0;

    *p = (tNodo*)malloc(sizeof(tNodo));
    if (!*p || !((*p)->tamInfo = leer(&(*p)->info, source, (unsigned)m, params)))
    {
        free(*p);
        return 1;
    }

    (*p)->izq = (*p)->der = NULL;

    if ((r = cargarDesdeDatosOrdenadosRec(&(*p)->izq, source, leer, li, m - 1, params)) != 0)
        return r;

    return cargarDesdeDatosOrdenadosRec(&(*p)->der, source, leer, m + 1, ls, params);
}
