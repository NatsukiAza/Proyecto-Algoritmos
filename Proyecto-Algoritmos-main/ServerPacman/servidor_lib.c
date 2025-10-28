#include "servidor_lib.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Funciones auxiliares



void rankingServidor(char* text){
    FILE *pArchRanking;
    tRanking jugador;
    int p = 1;
    //text[0] = '\0';
    pArchRanking = fopen("ranking.dat", "rb");

    if(!pArchRanking){
        strcpy(text, "Error. No se pudo conectar con la base de datos. \n");
        return;
    }else{
        fread(&jugador, sizeof(tRanking), 1, pArchRanking);
        while(!feof(pArchRanking)){
            sprintf(text + strlen(text), "%-3d%-21s%-6d\n", p, jugador.nombre, jugador.puntaje);
            p++;
            fread(&jugador, sizeof(tRanking), 1, pArchRanking);
        }
        fclose(pArchRanking);
    }
}

void actualizarBDD(char datos[], tArbol* arbol, tVector* vec, char text[]){
    int puntos = 0;
    tJugador nuevo;
    FILE *pArchInd, *pArchRanking;

    pArchInd = fopen("indice.dat", "r+b");
    if(!pArchInd){
        strcpy(text, "Error. No se pudo conectar con la base de datos. \n");
    }
    pArchRanking = fopen("ranking.dat", "r+b");
    if(!pArchRanking){
        strcpy(text, "Error. No se pudo conectar con la base de datos. \n");
    }
    datos = strchr(datos, ';');
    if(datos)
    {
        sscanf(datos+1, "%s %d", nuevo.nombre, &puntos);
    }


    int aux = buscarNodoArbolBinBusq(arbol, &nuevo, cmpJugador);

    if(aux){
        nuevo.indice = cantNodosArbolBin(arbol) + 1;
        insertarArbolRecu(arbol, &nuevo, sizeof(tJugador), cmpJugador);
        fseek(pArchInd, 0, SEEK_END);
        fwrite(&nuevo, sizeof(tJugador), 1, pArchInd);
    }
    if(actualizarRankingJugador(vec, nuevo.nombre, puntos)){
        tRanking rankTmp;
        strcpy(rankTmp.nombre, nuevo.nombre);
        rankTmp.puntaje = puntos;
        vectorInsertarOrdenado(vec, &rankTmp, cmpRanking);
        guardarRanking(vec, pArchRanking);
    }



    strcpy(text, "Puntaje actualizado en la base de datos!");
    TRACE("llegue hasta aca");
    fclose(pArchInd);
    fclose(pArchRanking);
}

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
        TRACE("termine de actualizar bd");
    }
    else
    {
        snprintf(response, BUFFER_SIZE, "Operacion no valida");
    }
}

int cmpJugador(const void* a, const void* b)
{
    const tJugador* x = (const tJugador*)a;
    const tJugador* y = (const tJugador*)b;
    return strcmp(x->nombre, y->nombre);
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
        TRACE("respuesta: %s", response);
        printf("Enviado:  %s\n", response);
    }

    printf("Conexion cerrada.\n");
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();
}
