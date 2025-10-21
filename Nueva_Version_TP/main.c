#include "generador.h"
#include "movimiento.h"
#include "menu.h"

void leerDatos(tLaberinto * l)
{
    FILE * arch = fopen("config.txt","r");
    if (arch == NULL)
    {
        printf("Error: No se pudo abrir el archivo de configuracion.\n");
        return;
    }

    int leidos = 0;

    leidos = fscanf(arch," filas:%d",&(l->filas));
    if (leidos != 1)
    {
        printf("Error al leer filas.\n");
        return;
    }

    leidos = fscanf(arch," columnas:%d",&(l->columnas));
    if (leidos != 1)
    {
        printf("Error al leer columnas.\n");
        return;
    }

    leidos = fscanf(arch," vidas_inicio:%d",&(l->jugador.vidas));
    if (leidos != 1)
    {
        printf("Error al leer vidas.\n");
        return;
    }

    leidos = fscanf(arch," maximo_numero_fantasmas:%d",&(l->cantFantasmas));
    if (leidos != 1)
    {
        printf("Error al leer fantasmas.\n");
        return;
    }

    leidos = fscanf(arch," maximo_numero_premios:%d",&(l->maximosPremios));
    if (leidos != 1)
    {
        printf("Error al leer premios.\n");
        return;
    }

    leidos = fscanf(arch," maximo_vidas_extra:%d",&(l->maxVidasExtra));
    if (leidos != 1)
    {
        printf("Error al leer vidas extra.\n");
        return;
    }

    fclose(arch);

    l->jugador.premios = 0;
    //printf("\n%d  %d %d %d %d %d",l->filas,l->columnas,l->jugador.vidas,l->cantFantasmas,l->maximosPremios,l->maxVidasExtra);
}


void  inicializarLaberinto(tLaberinto * l)
{
    l->lab=(char**)malloc(l->filas * sizeof(char*));
    for(int i=0; i<l->filas; i++)
    {
        l->lab[i]=(char*) malloc(l->columnas * sizeof(char));
        for(int j=0; j<l->columnas; j++)
        {
            l->lab[i][j]=PARED;
        }
    }
}

int compararRanking(const void* a, const void* b)
{
    tRanking* x = (tRanking*)a;
    tRanking* y = (tRanking*)b;
    return x->puntos - y->puntos;
}

int main()
{
    tLaberinto laberinto;
    int opMenu;

    srand(time(NULL));
    leerDatos(&laberinto);

    opMenu = menu();

    if(opMenu == 1)
    {
        printf("Ingrese su nombre de usuario: ");
        scanf("%s", laberinto.jugador.nombre);
        system("cls");

        inicializarLaberinto(&laberinto);
        //rellenarBordes(&laberinto);

        seleccionarAccesos(&laberinto); //aca falla a veces

        generarCaminoAleatorio(laberinto.lab,laberinto.filas,laberinto.columnas,laberinto.salidaX,laberinto.salidaY, laberinto.entradaX, laberinto.entradaY);
        generarFantasmas(&laberinto);
        generarVidasExtra(laberinto.lab, laberinto.maxVidasExtra, laberinto.filas, laberinto.columnas);
        generarPremios(laberinto.lab, laberinto.maximosPremios, laberinto.filas, laberinto.columnas);
        generarJugador(&laberinto);
        imprimirLaberinto(laberinto.lab,laberinto.columnas,laberinto.filas);

        if(terminarJuego(&laberinto)==PERDIO)
        {
            printf("PERDISTE MALETA!!!\n");
            actualizarRanking("ranking.dat", laberinto.jugador, 0, compararRanking);
            getch();
        }
        else
        {
            printf("GANASTE LOCO!!!\n");
            actualizarRanking("ranking.dat", laberinto.jugador, 1, compararRanking);
            getch();
        }


        for(int i=0; i<laberinto.filas; i++) free(laberinto.lab[i]);
        free(laberinto.lab);
        free(laberinto.fantasmas);
    }

    return 0;
}
