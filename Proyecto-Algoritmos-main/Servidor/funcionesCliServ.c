#include "funcionesCliServ.h"



void verJugadores(char* text){

    FILE *pArchIndice;
    tIndice jugador;
    int p = 1;
    text[0] = '\0';
    pArchIndice = fopen("indice.dat", "rb");

    if(!pArchIndice){
        strcpy(text, "Error. No se pudo conectar con la base de datos. \n");
        return;
    }else{
        fread(&jugador, sizeof(tIndice), 1, pArchIndice);
        while(!feof(pArchIndice)){
            sprintf(text + strlen(text), "%-4d%-21s\n", p, jugador.nombre);
            p++;
            fread(&jugador, sizeof(tIndice), 1, pArchIndice);
        }
        fclose(pArchIndice);
    }
}

void rankingServidor(char* text){
    FILE *pArchRanking;
    tRanking jugador;
    int p = 1;
    text[0] = '\0';
    pArchRanking = fopen("ranking.dat", "rb");

    if(!pArchRanking){
        strcpy(text, "Error. No se pudo conectar con la base de datos. \n");
        return;
    }else{
        fread(&jugador, sizeof(tRanking), 1, pArchRanking);
        while(!feof(pArchRanking)){
            sprintf(text + strlen(text), "%-4d%-21s%-6d\n", p, jugador.nombre, jugador.puntaje);
            p++;
            fread(&jugador, sizeof(tRanking), 1, pArchRanking);
        }
        fclose(pArchRanking);
    }
}

void actualizarBDD(char datos[], tArbol* arbol, tVector* vec, char* text){
    int puntos = 0;
    tIndice nuevo;
    FILE *pArchInd, *pArchRanking;
    text[0] = '\0';

    pArchInd = fopen("indice.dat", "a+b");
    if(!pArchInd){
        strcpy(text, "Error. No se pudo conectar con la base de datos. \n");
    }
    pArchRanking = fopen("ranking.dat", "wb");
    if(!pArchRanking){
        strcpy(text, "Error. No se pudo conectar con la base de datos. \n");
    }
    datos = strchr(datos, ';');
    if(datos)
    {
        sscanf(datos+1, "%s %d", nuevo.nombre, &puntos);
    }

    int aux = buscarNodoArbolBinBusq(arbol, &nuevo, cmpJugador);

    if(aux){
        nuevo.indice = cantNodosArbolBin(arbol) + 1;
        insertarArbolRecu(arbol, &nuevo, sizeof(tIndice), cmpJugador);
        fwrite(&nuevo, sizeof(tIndice), 1, pArchInd);
    }
    if(actualizarRankingJugador(vec, nuevo.nombre, puntos, cmpRanking)){
        tRanking rankTmp;
        strcpy(rankTmp.nombre, nuevo.nombre);
        rankTmp.puntaje = puntos;
        vectorInsertarOrdenado(vec, &rankTmp, cmpRanking);  //ya que el vector lo seguire utilizando mientras la instancia del servidor este abierta
    }
    guardarRanking(vec, pArchRanking);

    strcpy(text, "Puntaje actualizado en la base de datos!");
    fclose(pArchInd);
    fclose(pArchRanking);
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
            // si encontro y la nuevaPunt es mayor a la actual, actualiza
            if(nuevaPunt > rankTmp->puntaje){
                rankTmp->puntaje = nuevaPunt;
                qsort(vec->info, vec->ce, vec->tam, cmp);
            }
            return OK; //me voy porq ya encontre el user
        }
    }
    return ERR;
}

int guardarRanking(tVector* vec, FILE* fp)
{
    if (!vec || !vec->info)
        return ERR;
    size_t i = 0;
    while(i < vec->ce)
    {
        tRanking* tmp = vec->info + i * vec->tam;
        fwrite(tmp, sizeof(tRanking), 1, fp);
        i++;
    }
    return OK;
}

int cmpRanking(const void *a, const void *b){
    tRanking* x = (tRanking*)a;
    tRanking* y = (tRanking*)b;
    return x->puntaje - y->puntaje;
}

int cmpJugador(const void* a, const void* b)
{
    const tIndice* x = (const tIndice*)a;
    const tIndice* y = (const tIndice*)b;
    return strcmp(x->nombre, y->nombre);
}
