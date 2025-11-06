#include <stdio.h>
#include <stdlib.h>
#include "servidor_lib.h"
#include "funcionesCliServ.h"
#include "arbol.h"

int main()
{
    tArbol arbol;
    tVector vec;
    crearArbol(&arbol);
    vectorCrear(&vec, sizeof(tRanking), 10);

    run_server(&arbol, &vec);

    vaciarArbol(&arbol);
    vectorDestruir(&vec);
    return 0;
}
