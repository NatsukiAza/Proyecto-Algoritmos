#include "menu.h"

int menu(int offline, SOCKET sock)
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
        }
        else if(opcion == 2)
        {
            if(offline){
                system("cls");
                printf("Juego ejecutado en modo offline, ranking no disponible.");
                printf("\n\nPresione ENTER para regresar al menu.");
                getch();
                system("cls");
            }else{
                system("cls");
                printf("[CLIENTE] antes de mensajeServidor\n"); fflush(stdout);
                mensajeServidor("verranking;", sock);
                printf("[CLIENTE] despues de mensajeServidor\n"); fflush(stdout);
                printf("\n\nPresione ENTER para regresar al menu.");
                getch();
                system("cls");
            }

        }
        else
        {
            printf("Opcion incorrecta, presione ENTER para continuar.");
            getch();
            system("cls");
        }
    }while(opcion == 2);
    return opcion;
}

void mensajeServidor(const char accion[], SOCKET sock){
    char response[BUFFER_SIZE];

    if (send_request(sock, accion, response) == 0) {
        printf("%s\n", response);
    } else {
        printf("Error al enviar o recibir datos\n");
    }

}

int actualizarRanking(tJugador jugador, int flagWin, SOCKET sock)
{
    char buffer[BUFFER_SIZE];
    int puntos = 0;

    if (sock != INVALID_SOCKET) {
        if(flagWin == 1) //si el usuario gano
            puntos = 50;
        puntos += jugador.premios * 10;
        puntos -= (jugador.cantMov / 10);
        if (puntos < 0) puntos = 0;

        snprintf(buffer, sizeof(buffer), "actualizarranking;%s %d", jugador.nombre, puntos);
        mensajeServidor(buffer, sock);
    }

    return 0;
}
