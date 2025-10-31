#include "generador.h"
#include "movimiento.h"
#include "menu.h"
#include "conexiones.h"



//int compararRanking(const void* a, const void* b)
//{
//    tRanking* x = (tRanking*)a;
//    tRanking* y = (tRanking*)b;
//    return x->puntos - y->puntos;
//}

int main()
{

    tLaberinto laberinto;
    int opMenu = 1, offline = 0;

    if (init_winsock() != 0)
    {
        printf("Error al inicializar Winsock\n");
        return 1;
    }

    SOCKET sock = connect_to_server(SERVER_IP, PORT);
    if (sock == INVALID_SOCKET)
    {
        printf("No se pudo conectar al servidor, jugando en modo offline\n");
        WSACleanup();
        offline = 1;
    }
    else
        printf("Conectado al servidor.\n");


    // codigo sin server

    do
    {
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
            generarArchLaberinto(&laberinto);
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

            printf("\n\nPresione ENTER para regresar al menu.");
            getch();
            system("cls");

        }
    }while(opMenu!=4);

    if (sock != INVALID_SOCKET)
    {
        closesocket(sock);
    }
    WSACleanup();
    return 0;
}
