#include "servidor_lib.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Funciones auxiliares

void rankingServidor(char text[]){
    FILE *pArchRanking;
    tRanking jugador;
    int p = 0;

    pArchRanking = fopen("ranking.dat", "rb");

    if(!pArchRanking){
        strcpy(text, "Error. No se pudo conectar con la base de datos. \n");
    }else{
        fread(&jugador, sizeof(tJugador), 1, pArchRanking);
        while(!feof(pArchRanking)){
            sprintf(text+strlen(text), " %d %s %d \n", p, jugador.nombre, jugador.puntaje);
            p++;
            fread(&jugador, sizeof(tJugador), 1, pArchRanking);
        }
        fclose(pArchRanking);
    }
}

void actualizarBDD(const char datos[], tArbol* arbol, tVector* vec, char text[]){
    char nombre[22];
    int puntos = 0;
    char *aux = strrchr(text, '|');
    tJugador nuevo;
    FILE *pArchInd, *pArchRanking;

    pArchInd = fopen("indice.dat", "wb");
    if(!pArchInd){
        strcpy(text, "Error. No se pudo conectar con la base de datos. \n");
    }
    pArchRanking = fopen("ranking.dat", "wb");
    if(!pArchRanking){
        strcpy(text, "Error. No se pudo conectar con la base de datos. \n");
    }
    sscanf(aux+1, "%d", &puntos);
    *aux = '\0';
    sscanf(text, "%s", nombre);
    verNodoDesdeN(arbol, 0);
    if(buscarArbol(arbol, nombre)){
        strcpy(nuevo.nombre, nombre);
        nuevo.indice = contarNodos(arbol);
        insertarArbolRecu(arbol, &nuevo, sizeof(tJugador), cmpJugador);
        fwrite(&nuevo, sizeof(tJugador), 1, pArchInd);
    }
    verNodoDesdeN(arbol, 0);
    if(actualizarRanking(vec, nombre, puntos)){
        vectorOrdenarBurbujeo(vec, cmpRanking);
    }else{
        vectorInsertarOrdenado(vec, &nuevo, cmpRanking);
    }

    strcpy(text, "Puntaje actualizado en la base de datos!");
    fclose(pArchRanking);
    fclose(pArchInd);
}

// Implementación de funciones públicas
int init_winsock()
{
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2, 2), &wsa);
}

SOCKET create_server_socket()
{
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) return INVALID_SOCKET;

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
    {
        closesocket(s);
        return INVALID_SOCKET;
    }

    if (listen(s, 1) == SOCKET_ERROR)
    {
        closesocket(s);
        return INVALID_SOCKET;
    }

    return s;
}

void process_request(const char *request, char *response, tArbol* arbol, tVector* vec)
{
    char operation[25], text[BUFFER_SIZE];

    sscanf(request, "%[^;];%s", operation, text);

    if (strcmp(operation, "verranking") == 0)
    {
        rankingServidor(text);
        snprintf(response, BUFFER_SIZE, "%s", text);
    }
    else if (strcmp(operation, "actualizarranking") == 0)
    {
        actualizarBDD(request, arbol, vec, text);
        snprintf(response, BUFFER_SIZE, "%s", text);
    }
    else
    {
        snprintf(response, BUFFER_SIZE, "Operacion no valida");
    }
}

int cargarArbol(tArbol* arbol){
    FILE *pArchInd = fopen("indice.dat", "rb");
    tJugador jugador;
    if(!pArchInd){
        return ERR_ARCH;
    }
    fread(&jugador, sizeof(tJugador), 1, pArchInd);
    while(!feof(pArchInd)){
        insertarArbolRecu(arbol, &jugador, sizeof(tJugador), cmpJugador);
        fread(&jugador, sizeof(tJugador), 1, pArchInd);
    }
    return OK;
}

int cargarRanking(tVector* vec){
    FILE *pArchRanking = fopen("ranking.dat", "rb");
    tRanking jugador;
    if(!pArchRanking){
        return ERR_ARCH;
    }
    fread(&jugador, sizeof(tRanking), 1, pArchRanking);
    while(!feof(pArchRanking)){
        vectorInsertarOrdenado(vec, &jugador.puntaje, cmpRanking);
        fread(&jugador, sizeof(tRanking), 1, pArchRanking);
    }
    return OK;
}

void run_server(tArbol* arbol, tVector* vec)
{
    if (init_winsock() != 0)
    {
        printf("Error al inicializar Winsock\n");
        return;
    }

    SOCKET server_socket = create_server_socket();
    if (server_socket == INVALID_SOCKET)
    {
        printf("Error al crear socket del servidor\n");
        WSACleanup();
        return;
    }

    printf("Servidor escuchando en puerto %d...\n", PORT);

    if(cargarArbol(arbol)){
        printf("Error al abrir archivo indice. \n");
    }

    if(cargarRanking(vec)){
        printf("Error al abrir archivo ranking. \n");
    }

    struct sockaddr_in client_addr;
    int client_addr_size = sizeof(client_addr);

    SOCKET client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size);
    if (client_socket == INVALID_SOCKET)
    {
        printf("Error en accept()\n");
        closesocket(server_socket);
        WSACleanup();
        return;
    }

    printf("Cliente conectado.\n");

    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    int bytes_received;

    while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0)) > 0)
    {
        buffer[bytes_received] = '\0';
        printf("Recibido: %s\n", buffer);
        process_request(buffer, response, arbol, vec);
        send(client_socket, response, strlen(response), 0);
        printf("Enviado:  %s\n", response);
    }
    printf("Conexion cerrada.\n");
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();
}
