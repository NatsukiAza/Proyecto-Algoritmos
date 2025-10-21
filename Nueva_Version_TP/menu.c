#include "menu.h"
#include "generador.h"
#include "TDAvectorDin.h"

int menu()
{
    int opcion;
    do{
        printf("Bienvenido al LABERINTO DEBUGUEADORES\n\n");
        printf("Menu:\n1- Jugar\n2- Ver ranking\n3- Salir del juego\n\n");
        printf("Selecciona una opcion: ");
        fflush(stdin);
        scanf("%d", &opcion);

        if(opcion == 1 || opcion == 3)
        {
            system("cls");
            return opcion;
        }
        else if(opcion == 2)
        {
            system("cls");
            verRanking("ranking.dat");
            system("cls");
        }
        else
        {
            printf("Opcion incorrecta, presione ENTER para continuar.");
            getch();
            system("cls");
        }
    }while(opcion == 2);
}

void verRanking(const char* archi)
{
    tRanking rankTmp;
    FILE* fp = fopen(archi, "rb");
    if(!fp)
    {
        printf("No se han encontrado partidas guardadas.\n\nPresione ENTER para regresar al menu.");
        getch();
        return;
    }
    fread(&rankTmp, sizeof(tRanking), 1, fp);
    while(!feof(fp))
    {
        printf("%-21s%-6.2f\n", rankTmp.nombre, rankTmp.puntos);
        fread(&rankTmp, sizeof(tRanking), 1, fp);
    }
    printf("\n\nPresione ENTER para regresar al menu.");
    getch();
    fclose(fp);
    return;
}

int actualizarRanking(const char* archi, tJugador jugador, int flagWin, int(*cmp)(const void*, const void*))
{
    double puntos = 0.0;
    tVector ranking;
    FILE*fp = NULL;

    if(!vectorCrear(&ranking, sizeof(tRanking), 10))
        return SIN_MEM;

    if(existeRanking(archi) == 0) //verifico que exista el ranking
    {
        fp = fopen(archi, "rb");
        if(!fp)
        {
            vectorDestruir(&ranking);
            return 1;
        }

        descargarRanking(fp, &ranking, cmp);
        fclose(fp);
    }
    printf("hola");
    if(flagWin == 1) //si el usuario gano
        puntos = 50.0;
    puntos += jugador.premios * 10.0;
    puntos -= jugador.cantMov * 0.1;
    if (puntos < 0.0) puntos = 0.0;

    tRanking entry;
    strcpy(entry.nombre, jugador.nombre);
    entry.puntos = puntos;

    vectorInsertarOrdenado(&ranking, &entry, cmp); //vector ranking cargado
    fp = fopen(archi, "wb");
    if(!fp)
    {
        vectorDestruir(&ranking);
        return 1;
    }

    escribirRanking(fp, &ranking); //guardo el ranking en el archivo

    fclose(fp);
    return 0;
}

int existeRanking(const char* archi)
{
    FILE* fp = fopen(archi, "rb");
    if(!fp)
        return 1;
    fclose(fp);
    return 0;
}

void descargarRanking(FILE* fp, tVector* vector, int(*cmp)(const void*, const void*))
{
    size_t i = 0;
    tRanking rankTmp;
    fread(&rankTmp, sizeof(tRanking), 1, fp);
    while(!feof(fp))
    {
        vectorInsertarOrdenado(vector, &rankTmp, cmp);
        i++;
        fread(&rankTmp, sizeof(tRanking), 1, fp);
    }
    fclose(fp);
    return TODO_OK;
}

void escribirRanking(FILE* fp, tVector* vector)
{
    size_t i = 0;
    tRanking rankTmp;
    while(i < vector->ce)
    {
        memcpy(&rankTmp, vector->info + i * vector->tam, vector->tam);
        fwrite(&rankTmp, sizeof(tRanking), 1, fp);
        i++;
    }
}
