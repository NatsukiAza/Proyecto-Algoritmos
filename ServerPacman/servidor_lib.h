#ifndef SERVIDOR_LIB_H_INCLUDED
#define SERVIDOR_LIB_H_INCLUDED
#include <winsock2.h>
#include "arbol.h"
#include "TDAvectorDin.h"


#define PORT 12345
#define BUFFER_SIZE 1024

// Inicializa Winsock
int init_winsock();

// Crea el socket del servidor
SOCKET create_server_socket();

// Procesa el mensaje recibido y genera la respuesta
void process_request(const char *request, char *response, tArbol* arbol, tVector* vec);

// Ejecuta el bucle principal del servidor
void run_server();
#endif // SERVIDOR_LIB_H_INCLUDED
