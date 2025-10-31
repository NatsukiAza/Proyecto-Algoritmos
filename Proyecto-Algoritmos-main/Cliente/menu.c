#include "menu.h"

int menu(int offline, SOCKET sock)
{
    int opcion;
    do{
        printf("Bienvenido al LABERINTO DEBUGUEADORES\n\n");
        printf("Menu:\n1- Jugar\n2- Ver ranking\n3- Ver jugadores\n4- Salir del juego\n\n");
        printf("Selecciona una opcion: ");
        fflush(stdin);
        scanf("%d", &opcion);

        if(opcion == 1 || opcion == 4)
        {
            system("cls");
        }
        else if(opcion == 2)
        {
            system("cls");
            if(offline){
                printf("Juego ejecutado en modo offline, 'Ver ranking' no disponible.");
            }else{
                printf("%-4s%-21s%-5s\n", "POS", "USER", "PUNT.");
                mensajeServidor("verranking;", sock);
            }
            printf("\n\nPresione ENTER para regresar al menu.");
            getch();
            system("cls");
        }
        else if(opcion == 3)
        {
            system("cls");
            if(offline){
                printf("Juego ejecutado en modo offline, 'Ver jugadores' no disponible.");
            }else{
                printf("%-4s%-21s%-6s%-9s\n", "POS", "USER", "PUNT.", "CANT.MOV");
                mensajeServidor("verjugadores;", sock);
            }
            printf("\n\nPresione ENTER para regresar al menu.");
            getch();
            system("cls");
        }
        else
        {
            printf("Opcion incorrecta, presione ENTER para continuar.");
            getch();
            system("cls");
        }
    }while(opcion != 1 && opcion != 4);
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

        snprintf(buffer, sizeof(buffer), "actualizarranking;%s %d %d", jugador.nombre, puntos, jugador.cantMov);
        mensajeServidor(buffer, sock);
    }

    return 0;
}
