#ifndef SERVIDOR_LIB_H_INCLUDED
#define SERVIDOR_LIB_H_INCLUDED
#include <winsock2.h>
#include "arbol.h"
#include "TDAvectorDin.h"
#include <time.h>


#define TRACE(...) do { \
    time_t _t=time(NULL); struct tm _tm; localtime_s(&_tm,&_t); \
    fprintf(stderr, "[%02d:%02d:%02d] %s:%d %s() | ", \
        _tm.tm_hour,_tm.tm_min,_tm.tm_sec,__FILE__,__LINE__,__func__); \
    fprintf(stderr, __VA_ARGS__); \
    fputc('\n', stderr); fflush(stderr); \
} while(0)

#define PORT 12345
#define BUFFER_SIZE 1024

// Inicializa Winsock
int init_winsock();

// Crea el socket del servidor
SOCKET create_server_socket();

// Procesa el mensaje recibido y genera la respuesta
void process_request(char *request, char *response, tArbol* arbol, tVector* vec);

// Ejecuta el bucle principal del servidor
void run_server();
#endif // SERVIDOR_LIB_H_INCLUDED
