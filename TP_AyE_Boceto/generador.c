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

void seleccionarAccesos(tLaberinto *l) {
    srand(time(NULL));
    int lado = rand() % 4;

    int entradaFila, entradaCol;
    int salidaFila, salidaCol;

    if(lado%2==0){
        entradaFila = 0;
        entradaCol  = rand() % (l->columnas - 2) + 1;
        salidaFila  = l->filas - 1;
        salidaCol   = rand() % (l->columnas - 2) + 1;
    }else{
        entradaFila = rand() % (l->filas - 2) + 1;
        entradaCol  = l->columnas - 1;
        salidaFila  = rand() % (l->filas - 2) + 1;
        salidaCol   = 0;
    }

    switch (lado) {
        case 0: // ARRIBA
            l->entradaX = entradaFila+1;
            l->entradaY = entradaCol;
            l->salidaX  = salidaFila-1;
            l->salidaY  = salidaCol;

            l->lab[entradaFila][entradaCol] = 'E';
            l->lab[entradaFila + 1][entradaCol] = CAMINO;

            l->lab[salidaFila][salidaCol] = 'S';
            l->lab[salidaFila - 1][salidaCol] = CAMINO;
            break;

        case 1: // DERECHA
            l->entradaX = entradaFila;
            l->entradaY = entradaCol-1;
            l->salidaX  = salidaFila;
            l->salidaY  = salidaCol+1;

            l->lab[entradaFila][entradaCol] = 'E';
            l->lab[entradaFila][entradaCol - 1] = CAMINO;

            l->lab[salidaFila][salidaCol] = 'S';
            l->lab[salidaFila][salidaCol + 1] = CAMINO;
            break;

        case 2: // ABAJO
            l->entradaX = entradaFila-1;
            l->entradaY = entradaCol;
            l->salidaX  = salidaFila+1;
            l->salidaY  = salidaCol;

            l->lab[entradaFila][entradaCol] = 'E';
            l->lab[entradaFila - 1][entradaCol] = CAMINO;

            l->lab[salidaFila][salidaCol] = 'S';
            l->lab[salidaFila + 1][salidaCol] = CAMINO;
            break;

        case 3: // IZQUIERDA
            l->entradaX = entradaFila;
            l->entradaY = entradaCol-1;
            l->salidaX  = salidaFila;
            l->salidaY  = salidaCol-1;

            l->lab[entradaFila][entradaCol] = 'E';
            l->lab[entradaFila][entradaCol + 1] = CAMINO;

            l->lab[salidaFila][salidaCol] = 'S';
            l->lab[salidaFila][salidaCol - 1] = CAMINO;
            entradaCol++;
            break;
    }

    // Guardar en la struct

}

void generarCaminoAleatorio(tLaberinto *l, int x, int y) {
    l->lab[x][y] = CAMINO;

    int dirs[4] = {0, 1, 2, 3};

    // Mezclamos las direcciones
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4;
        int t = dirs[i]; dirs[i] = dirs[r]; dirs[r] = t;
    }

    for (int i = 0; i < 4; i++) {
        int nx = x, ny = y;

        if (dirs[i] == 0) nx -= 2;      // arriba
        else if (dirs[i] == 1) nx += 2; // abajo
        else if (dirs[i] == 2) ny -= 2; // izquierda
        else ny += 2;                   // derecha

        if (nx > 0 && ny > 0 && nx < l->filas - 1 && ny < l->columnas - 1) {
            int esSalida = (nx == l->salidaX && ny == l->salidaY);
            if (l->lab[nx][ny] == PARED || esSalida) {
                // abrir pared intermedia
                l->lab[(x + nx) / 2][(y + ny) / 2] = CAMINO;

                l->lab[nx][ny] = CAMINO;
                generarCaminoAleatorio(l, nx, ny);
            }
        }
    }
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

int moverFantasmas(char ** lab,tFantasma f[],int cantidadFantasmas){
    int dire, nX, nY;

    for (int i = 0; i < cantidadFantasmas; i++) {
        int movido = 0;
        int intentos = 0;
        while (!movido && intentos < 4) {
            dire = rand() % 4;
            nX = f[i].fil;
            nY = f[i].col;

            switch (dire) {
                case 0: // ARRIBA
                    switch(lab[nX+1][nY]){
                        case CAMINO:f[i].fil++; lab[nX+1][nY]=FANTASMA; lab[nX][nY] = CAMINO; movido=1; break;
                        case PLAYER:f[i].fil++; lab[nX+1][nY]=FANTASMA; lab[nX][nY] = CAMINO; return 1;
                    }
                    break;
                case 1: // DERECHA
                    switch(lab[nX][nY+1]){
                        case CAMINO: f[i].col++; lab[nX][nY+1] = FANTASMA; lab[nX][nY] = CAMINO; movido = 1; break;
                        case PLAYER: f[i].col++; lab[nX][nY+1] = FANTASMA; lab[nX][nY] = CAMINO; return 1;
                    }
                    break;
                case 2: // ABAJO
                    switch(lab[nX-1][nY]){
                        case CAMINO:f[i].fil--; lab[nX-1][nY]=FANTASMA; lab[nX][nY] = CAMINO; movido=1; break;
                        case PLAYER:f[i].fil--; lab[nX-1][nY]=FANTASMA; lab[nX][nY] = CAMINO; return 1;
                    }
                    break;
                case 3: // IZQUIERDA
                    switch(lab[nX][nY-1]){
                        case CAMINO: f[i].col--; lab[nX][nY-1] = FANTASMA; lab[nX][nY] = CAMINO; movido = 1; break;
                        case PLAYER: f[i].col--; lab[nX][nY-1] = FANTASMA; lab[nX][nY] = CAMINO; return 1;
                    }
                    break;
            }
            intentos++;
        }
    }
    return 0;
}
