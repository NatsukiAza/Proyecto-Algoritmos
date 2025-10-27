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
    int opMenu, offline = 0;

    if (init_winsock() != 0) {
        printf("Error al inicializar Winsock\n");
        return 1;
    }

    SOCKET sock = connect_to_server(SERVER_IP, PORT);
    if (sock == INVALID_SOCKET) {
        printf("No se pudo conectar al servidor, jugando en modo offline\n");
        WSACleanup();
        offline = 1;
    }

    printf("Conectado al servidor.\n");

    // codigo sin server

    srand(time(NULL));
    leerDatos(&laberinto);

    opMenu = menu(offline, sock);

    if(opMenu == 1)
    {
        printf("Ingrese su nombre de usuario: ");
        scanf("%s", laberinto.jugador.nombre);
        system("cls");

        iniciarLogMovimientos("movimientos.txt", laberinto.jugador.nombre);

        inicializarLaberinto(&laberinto);
        //rellenarBordes(&laberinto);

        seleccionarAccesos(&laberinto); //aca falla a veces

        generarCaminoAleatorio(laberinto.lab, laberinto.filas, laberinto.columnas, laberinto.entradaX, laberinto.entradaY, 0, 0);
        generarFantasmas(&laberinto);
        generarVidasExtra(laberinto.lab, laberinto.maxVidasExtra, laberinto.filas, laberinto.columnas);
        generarPremios(laberinto.lab, laberinto.maximosPremios, laberinto.filas, laberinto.columnas);
        generarJugador(&laberinto);
        imprimirLaberinto(laberinto.lab,laberinto.columnas,laberinto.filas);

        if(terminarJuego(&laberinto)==PERDIO)
        {
            printf("PERDISTE MALETA!!!\n");
            actualizarRanking(laberinto.jugador, 0, sock);
            getch();
        }
        else
        {
            printf("GANASTE LOCO!!!\n");
            actualizarRanking(laberinto.jugador, 1, sock);
            getch();
        }


        for(int i=0; i<laberinto.filas; i++) free(laberinto.lab[i]);
        free(laberinto.lab);
        free(laberinto.fantasmas);
    }

    return 0;
}
