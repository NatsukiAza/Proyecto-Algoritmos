#include "generador.h"

int dx[4] = { -2, 2, 0, 0 };
int dy[4] = { 0, 0, -2, 2 };

//void rellenarBordes(char ** lab,int n){
//    for(int i=0;i<n;i+=n-1){
//        for(int j=0;j<n;j++){
//            lab[i][j]=PARED;
//        }
//    }
//    for(int i=0;i<n;i+=n-1){
//        for(int j=0;j<n;j++){
//            lab[j][i]=PARED;
//        }
//    }
//
//}

//void seleccionarAccesos(tLaberinto *l) {
//    srand(time(NULL));
//    int lado = rand() % 4;
//
//    int entradaFila, entradaCol;
//    int salidaFila, salidaCol;
//
//    if(lado%2==0){
//        entradaFila = 0;
//        entradaCol  = rand() % (l->columnas - 2) + 1;
//        salidaFila  = l->filas - 1;
//        salidaCol   = rand() % (l->columnas - 2) + 1;
//    }else{
//        entradaFila = rand() % (l->filas - 2) + 1;
//        entradaCol  = l->columnas - 1;
//        salidaFila  = rand() % (l->filas - 2) + 1;
//        salidaCol   = 0;
//    }
//
//    switch (lado) {
//        case 0: // ARRIBA
//            l->entradaX = entradaFila+1;
//            l->entradaY = entradaCol;
//            l->salidaX  = salidaFila-2;
//            l->salidaY  = salidaCol;
//
//            l->lab[entradaFila][entradaCol] = ENTRADA;
//            l->lab[entradaFila + 1][entradaCol] = CAMINO;
//
//            l->lab[salidaFila][salidaCol] = SALIDA;
//            l->lab[salidaFila - 1][salidaCol] = CAMINO;
//            break;
//
//        case 1: // DERECHA
//            l->entradaX = entradaFila;
//            l->entradaY = entradaCol-1;
//            l->salidaX  = salidaFila;
//            l->salidaY  = salidaCol+1;
//
//
//            l->lab[entradaFila][entradaCol] = ENTRADA;
//            l->lab[entradaFila][entradaCol - 1] = CAMINO;
//
//            l->lab[salidaFila][salidaCol] = SALIDA;
//            l->lab[salidaFila][salidaCol + 1] = CAMINO;
//            break;
//
//        case 2: // ABAJO
//            l->entradaX = entradaFila-1;
//            l->entradaY = entradaCol;
//            l->salidaX  = salidaFila+1;
//            l->salidaY  = salidaCol;
//
//            l->lab[entradaFila][entradaCol] = ENTRADA;
//            l->lab[entradaFila - 1][entradaCol] = CAMINO;
//
//            l->lab[salidaFila][salidaCol] = SALIDA;
//            l->lab[salidaFila + 1][salidaCol] = CAMINO;
//            break;
//
//        case 3: // IZQUIERDA
//            l->entradaX = entradaFila;
//            l->entradaY = entradaCol-1;
//            l->salidaX  = salidaFila;
//            l->salidaY  = salidaCol-2;
//
//            l->lab[entradaFila][entradaCol] = ENTRADA;
//            l->lab[entradaFila][entradaCol + 1] = CAMINO;
//
//            l->lab[salidaFila][salidaCol] = SALIDA;
//            l->lab[salidaFila][salidaCol - 1] = CAMINO;
//            entradaCol++;
//            break;
//    }
//    l->jugador.fil = l->entradaX;
//    l->jugador.col = l->entradaY;
//
//}
//
void generarCaminoAleatorio(char **lab, int filas, int columnas, int x, int y, int salidaX, int salidaY) {
    lab[x][y] = CAMINO;

    int dirs[4] = {0,1,2,3};
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4;
        int t = dirs[i]; dirs[i] = dirs[r]; dirs[r] = t;
    }

    for (int i = 0; i < 4; i++) {
        int nx = x, ny = y;

        if (dirs[i] == 0) nx -= 2;
        else if (dirs[i] == 1) nx += 2;
        else if (dirs[i] == 2) ny -= 2;
        else ny += 2;

        // ✅ Ajuste: controlar con filas/columnas, no asumir cuadrado
        if (nx > 0 && ny > 0 && nx < filas && ny < columnas) {
            int esSalida = (nx == salidaX && ny == salidaY);

            if ((nx < filas-1 && ny < columnas-1) && (lab[nx][ny] == PARED || esSalida)) {
                // abrir la pared intermedia
                lab[(x + nx) / 2][(y + ny) / 2] = CAMINO;

                lab[nx][ny] = CAMINO;
                generarCaminoAleatorio(lab, filas, columnas, nx, ny, salidaX, salidaY);
            }
        }
    }
}
void seleccionarAccesos(tLaberinto *l) {

    int lado = rand() % 4;

    int filas = l->filas;
    int cols  = l->columnas;

    //pedir tamañp minimo
    if (filas < 3 || cols < 3) {
        fprintf(stderr, "Error: el laberinto debe ser al menos 3x3\n");
        exit(1);
    }

    int entradaFila, entradaCol;
    int salidaFila,  salidaCol;

    switch (lado) {
        case 0: //ARRIBA (entrada arriba, salida abajo)
            entradaFila = 0;
            entradaCol  = rand() % (cols - 2) + 1;
            salidaFila  = filas - 1;
            salidaCol   = rand() % (cols - 2) + 1;

            l->entradaX = entradaFila + 1;
            l->entradaY = entradaCol;
            l->salidaX  = salidaFila - 1;
            l->salidaY  = salidaCol;

            l->lab[entradaFila][entradaCol] = ENTRADA;
            l->lab[entradaFila + 1][entradaCol] = CAMINO;

            l->lab[salidaFila][salidaCol] = SALIDA;
            l->lab[salidaFila - 1][salidaCol] = CAMINO;
            break;

        case 1: //DERECHA (entrada derecha, salida izquierda)
            entradaFila = rand() % (filas - 2) + 1;  //
            entradaCol  = cols - 1;
            salidaFila  = rand() % (filas - 2) + 1;
            salidaCol   = 0;

            l->entradaX = entradaFila;
            l->entradaY = entradaCol - 1;
            l->salidaX  = salidaFila;
            l->salidaY  = salidaCol + 1;

            l->lab[entradaFila][entradaCol] = ENTRADA;
            l->lab[entradaFila][entradaCol - 1] = CAMINO;

            l->lab[salidaFila][salidaCol] = SALIDA;
            l->lab[salidaFila][salidaCol + 1] = CAMINO;
            break;

        case 2: //ABAJO (entrada abajo, salida arriba)
            entradaFila = filas - 1;
            entradaCol  = rand() % (cols - 2) + 1;
            salidaFila  = 0;
            salidaCol   = rand() % (cols - 2) + 1;

            l->entradaX = entradaFila - 1;
            l->entradaY = entradaCol;
            l->salidaX  = salidaFila + 1;
            l->salidaY  = salidaCol;

            l->lab[entradaFila][entradaCol] = ENTRADA;
            l->lab[entradaFila - 1][entradaCol] = CAMINO;

            l->lab[salidaFila][salidaCol] = SALIDA;
            l->lab[salidaFila + 1][salidaCol] = CAMINO;
            break;

        case 3: //IZQUIERDA (entrada izquierda, salida derecha)
            entradaFila = rand() % (filas - 2) + 1;
            entradaCol  = 0;
            salidaFila  = rand() % (filas - 2) + 1;
            salidaCol   = cols - 1;

            l->entradaX = entradaFila;
            l->entradaY = entradaCol + 1;
            l->salidaX  = salidaFila;
            l->salidaY  = salidaCol - 1;

            l->lab[entradaFila][entradaCol] = ENTRADA;
            l->lab[entradaFila][entradaCol + 1] = CAMINO;

            l->lab[salidaFila][salidaCol] = SALIDA;
            l->lab[salidaFila][salidaCol - 1] = CAMINO;
            break;
    }

    //posición inicial del jugador
    l->jugador.fil = l->entradaX;
    l->jugador.col = l->entradaY;
}


void generarVidas(char ** lab,tFantasma * fantasmas,int n){

}

void imprimirLaberinto(char **lab,int col,int fil){
    for(int i=0;i<fil;i++){
        for(int j=0;j<col;j++){
            printf("%c ", lab[i][j]);
        }
        printf("\n");
    }
}

void generarFantasmas(tLaberinto * l) {
    int fantasmasColocados = 0;
    l->fantasmas=malloc(l->cantFantasmas * sizeof(tFantasma));
    int radioSeguridad = (int)(((l->columnas+l->filas)/2) * 0.40);

    radioSeguridad=MIN(2,radioSeguridad);

    // Inicializa el generador de n�meros aleatorios si no se ha hecho
    // srand(time(NULL));

    while (fantasmasColocados < l->cantFantasmas) {
        int fX = rand() % (l->columnas - 2) + 1;
        int fY = rand() % (l->filas - 2) + 1;

        int distancia = abs(fX - l->entradaX) + abs(fY - l->entradaY);

        if (l->lab[fX][fY] == CAMINO && distancia > radioSeguridad) {
            l->lab[fX][fY] = FANTASMA;
            l->fantasmas[fantasmasColocados].fil=fX;
            l->fantasmas[fantasmasColocados].col=fY;
            fantasmasColocados++;
        }
    }
}

int moverFantasmas(char **lab, tFantasma *f, int *cantidadFantasmas, int laberintoFilas, int laberintoCols)
{
    tCola colaMovimientos;
    crearCola(&colaMovimientos);

    for (int i = 0; i < *cantidadFantasmas; i++) {
        int dire = rand() % 4; // 0:Arriba, 1:Derecha, 2:Abajo, 3:Izquierda
        encolar(&colaMovimientos, &i, sizeof(int));
        encolar(&colaMovimientos, &dire, sizeof(int));
    }

    int fantasmaID, direccion;

    while (!colaVacia(&colaMovimientos)) {
        desencolar(&colaMovimientos, &fantasmaID, sizeof(int));
        desencolar(&colaMovimientos, &direccion, sizeof(int));

        int nX = f[fantasmaID].fil;
        int nY = f[fantasmaID].col;
        int nuevoX = nX;
        int nuevoY = nY;

        switch (direccion) {
            case 0: // ARRIBA
                nuevoX++;
                break;
            case 1: // DERECHA
                nuevoY++;
                break;
            case 2: // ABAJO
                nuevoX--;
                break;
            case 3: // IZQUIERDA
                nuevoY--;
                break;
        }

        if (nuevoX >= 0 && nuevoX < laberintoFilas && nuevoY >= 0 && nuevoY < laberintoCols) {
            if (lab[nuevoX][nuevoY] == CAMINO) {
                lab[nX][nY] = CAMINO;
                f[fantasmaID].fil = nuevoX;
                f[fantasmaID].col = nuevoY;
                lab[nuevoX][nuevoY] = FANTASMA;
            }
            else if (lab[nuevoX][nuevoY] == PLAYER) {
                //borrar del tablero la celda anterior del fantasma
                lab[nX][nY] = CAMINO;
                for (int j = fantasmaID; j < * cantidadFantasmas - 1; j++)
                    f[j] = f[j + 1];
                (*cantidadFantasmas)--;

                vaciarCola(&colaMovimientos);
                return 1; //jugador capturado
            }
        }
    }

    vaciarCola(&colaMovimientos);
    return 0; // Retorna 0 si nadie fue atrapado
}
void generarJugador(tLaberinto * laberinto){
    laberinto->jugador.fil = laberinto->entradaX;
    laberinto->jugador.col = laberinto->entradaY;
    int fila = laberinto->jugador.fil;
    int col  = laberinto->jugador.col;
    laberinto->lab[fila][col] = PLAYER;
}

void generarVidasExtra(char** lab, int cantVidas, int f, int c)
{
    int x, y;

    while(cantVidas>0)
    {
        x = rand()%f;
        y = rand()%c;
        if(lab[x][y] == CAMINO)
        {
            lab[x][y] = VIDA;
            cantVidas--;
        }
    }
}

void generarPremios(char** lab, int cantPremios, int f, int c)
{
    int x, y;

    while(cantPremios>0)
    {
        x = rand()%f;
        y = rand()%c;
        if(lab[x][y] == CAMINO)
        {
            lab[x][y] = PREMIO;
            cantPremios--;
        }
    }
}
