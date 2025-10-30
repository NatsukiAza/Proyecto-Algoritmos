#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <winsock2.h>
#include "generador.h"
#include "conexiones.h"

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

int menu(int offline, SOCKET sock);
int actualizarRanking(tJugador jugador, int flagWin, SOCKET sock);
void mensajeServidor(const char accion[], SOCKET sock);

#endif // MENU_H_INCLUDED
