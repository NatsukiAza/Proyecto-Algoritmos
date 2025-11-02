#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "servidor_lib.h"
#include "funcionesCliServ.h"

// Funciones auxiliares

unsigned leerIndice(void **info, void *source, unsigned pos, void *params)
{
    FILE *fp = (FILE*)source;
    tIndice idx;

    //me posiciono en el registro del archivo que tengo que leer (m)
    fseek(fp, pos * sizeof(tIndice), SEEK_SET);
    fread(&idx, sizeof(tIndice), 1, fp);

    //reservo el espacio y copio info
    *info = malloc(sizeof(tIndice));
    if (!*info)
        return 0;

    memcpy(*info, &idx, sizeof(tIndice));

    return sizeof(tIndice);
}

int generarArbolBinBusqBalanceado(tArbol* a)
{
    FILE* fp = fopen("indice.dat", "rb");
    if(!fp)
        return 1;

    fseek(fp, 0, SEEK_END);
    long n = ftell(fp) / sizeof(tIndice);
    rewind(fp);

    cargarDesdeDatosOrdenadosRec(a, fp, leerIndice, 0, n - 1,  NULL);

    fclose(fp);
    return 0;
}

////carga arbol inicial desbalanceado
//int cargarArbol(tArbol* arbol){
//    FILE *pArchInd = fopen("indice.dat", "rb");
//    tIndice jugador;
//    if(!pArchInd){
//        return ERR_ARCH;
//    }
//    fread(&jugador, sizeof(tIndice), 1, pArchInd);
//    while(!feof(pArchInd)){
//        insertarArbolRecu(arbol, &jugador, sizeof(tIndice), cmpJugador);
//        fread(&jugador, sizeof(tIndice), 1, pArchInd);
//    }
//    return OK;
//}

int cargarRanking(tVector* vec){
    FILE *pArchRanking = fopen("ranking.dat", "rb");
    tRanking jugador;
    if(!pArchRanking){
        return ERR_ARCH;
    }
    fread(&jugador, sizeof(tRanking), 1, pArchRanking);
    while(!feof(pArchRanking)){
        vectorInsertarOrdenado(vec, &jugador, cmpRanking);
        fread(&jugador, sizeof(tRanking), 1, pArchRanking);
    }
    return OK;
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

void process_request(char *request, char *response, tArbol* arbol, tVector* vec)
{
    char operation[16], text[BUFFER_SIZE];
    sscanf(request, "%[^;];%[^\n]", operation, text);

    if (strcmp(operation, "verranking") == 0)
    {
        rankingServidor(text);
        snprintf(response, BUFFER_SIZE, "%s", text);
    }
    else if (strcmp(operation, "actualizarranking") == 0)
    {
        actualizarBDD(request, arbol, vec, text);
        snprintf(response, BUFFER_SIZE, "%s", text);
        TRACE("base actualizada");
    }
    else if (strcmp(operation, "verjugadores") == 0)
    {
        verJugadores(text);
        snprintf(response, BUFFER_SIZE, "%s", text);
    }
    else
    {
        snprintf(response, BUFFER_SIZE, "Operacion no valida");
    }
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

    if(generarArbolBinBusqBalanceado(arbol)){
        printf("Error al abrir archivo indice. \n");
    }

    if(cargarRanking(vec)){
        printf("Error al abrir archivo ranking. \n");
    }

    if(crearArchivoJugadores("jugadores.dat")){
        printf("Error al abrir archivo de jugadores. \n");
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
