#include "funcionesCliServ.h"
#include "servidor_lib.h"


void verJugadores(char* text) //funcion numero 3 del menu principal
{
    FILE *pArch;
    tJugador jugador;
    int p = 1;
    text[0] = '\0';
    pArch = fopen("jugadores.dat", "rb");

    if(!pArch)
    {
        strcpy(text, "Error. No se pudo conectar con la base de datos. \n");
        return;
    }
    size_t leidos = fread(&jugador, sizeof(tJugador), 1, pArch); //controlo haber leido al menos 1 jugador para mostrarlos
    if(leidos > 0)
    {
        while(!feof(pArch))
        {
            sprintf(text + strlen(text), "%-4d%-21s%-6d%-9d\n", p, jugador.nombre, jugador.puntos, jugador.cantMov);
            p++;
            fread(&jugador, sizeof(tJugador), 1, pArch);
        }
    }
    else
        strcpy(text, "Error. Base de jugadores vacia. \n");

    fclose(pArch);
}

void rankingServidor(char* text) //funcion numero 2 del menu principal, muestro el ranking recorriendo el archivo
{
    FILE *pArchRanking;
    tRanking jugador;
    int p = 1;
    text[0] = '\0';
    pArchRanking = fopen("ranking.dat", "rb");

    if(!pArchRanking)
    {
        strcpy(text, "Error. No se pudo conectar con la base de datos. \n");
        return;
    }
    fread(&jugador, sizeof(tRanking), 1, pArchRanking);
    while(!feof(pArchRanking))
    {
        sprintf(text + strlen(text), "%-4d%-21s%-6d\n", p, jugador.nombre, jugador.puntaje);
        p++;
        fread(&jugador, sizeof(tRanking), 1, pArchRanking);
    }
    fclose(pArchRanking);
}

void actualizarBDD(char datos[], tArbol* arbol, tVector* vec, char* text) //actualizacion general de la base de datos al terminar la partida
{

    tIndice indiceTmp;
    tJugador jugadorTmp, jugadorAux;
    FILE *pArchInd, *pArchRanking, *pArchJugador;
    text[0] = '\0';

    pArchInd = fopen("indice.dat", "a+b");
    if(!pArchInd)
    {
        strcpy(text, "Error. No se pudo conectar con la base de datos. \n");
    }
    pArchRanking = fopen("ranking.dat", "wb");
    if(!pArchRanking)
    {
        strcpy(text, "Error. No se pudo conectar con la base de datos. \n");
        fclose(pArchInd);
        return;
    }
    pArchJugador = fopen("jugadores.dat", "r+b");
    if(!pArchJugador)
    {
        strcpy(text, "Error. No se pudo conectar con la base de datos. \n");
        fclose(pArchInd);
        fclose(pArchRanking);
        return;
    }

    //escaneo los datos enviados por el cliente
    datos = strchr(datos, ';');
    if(datos)
    {
        sscanf(datos+1, "%s %d %d", jugadorTmp.nombre, &jugadorTmp.puntos, &jugadorTmp.cantMov);
    }

    strcpy(indiceTmp.nombre, jugadorTmp.nombre);
    tNodo** nodoAux = buscarNodoArbolBinBusq(arbol, &indiceTmp, cmpJugador); //busco si ya existe el jugador

    if(!nodoAux) //si no existe
    {
        //inserto en el arbol e indice
        indiceTmp.indice = cantNodosArbolBin(arbol);
        insertarArbolRecu(arbol, &indiceTmp, sizeof(tIndice), cmpJugador);
        fwrite(&indiceTmp, sizeof(tIndice), 1, pArchInd);
        //inserto en archivo jugadores
        fseek(pArchJugador, 0, SEEK_END);
        fwrite(&jugadorTmp, sizeof(tJugador), 1, pArchJugador);
    }
    else //en caso de que el jugador ya exista, actualizo sus puntos y movimientos
    {
        memcpy(&indiceTmp, (*nodoAux)->info, sizeof(tIndice));
        fseek(pArchJugador, indiceTmp.indice * sizeof(tJugador), SEEK_SET); //me posiciono en el registro del jugador en el archivo
        fread(&jugadorAux, sizeof(tJugador), 1, pArchJugador);
        jugadorAux.cantMov += jugadorTmp.cantMov;
        jugadorAux.puntos += jugadorTmp.puntos;
        fseek(pArchJugador, -(long)sizeof(tJugador), SEEK_CUR);
        fwrite(&jugadorAux, sizeof(tJugador), 1, pArchJugador);
    }

    //trabajo con el ranking
    if(actualizarRankingJugador(vec, jugadorTmp.nombre, jugadorTmp.puntos, cmpRanking))
    {
        tRanking rankTmp;
        strcpy(rankTmp.nombre, jugadorTmp.nombre);
        rankTmp.puntaje = jugadorTmp.puntos;
        vectorInsertarOrdenado(vec, &rankTmp, cmpRanking);  //ya que el vector lo seguire utilizando mientras la instancia del servidor este abierta
    }
    guardarRanking(vec, pArchRanking);

    strcpy(text, "Base de datos actualizada.");
    fclose(pArchInd);
    fclose(pArchRanking);
    fclose(pArchJugador);
}

//actualiza la puntuacion de un jugador si ya aparece en el ranking y reeordena el mismo
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
            if(nuevaPunt > rankTmp->puntaje)
            {
                rankTmp->puntaje = nuevaPunt;
                qsort(vec->info, vec->ce, vec->tam, cmp);
            }
            return OK; //me voy porq ya encontre el user
        }
    }
    return ERR;
}

//recorro el vector ranking y lo persisto en un archivo binario
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

int cmpRanking(const void *a, const void *b)
{
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

int crearArchivoJugadores(const char* archi)
{
    FILE* fp = fopen(archi, "a+b");
    if(!fp)
        return 1;
    fclose(fp);
    return 0;
}
