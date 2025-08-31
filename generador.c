#include "generador.h"

int dx[4] = { -2, 2, 0, 0 };
int dy[4] = { 0, 0, -2, 2 };

void rellenarBordes(char ** lab,int n){
    for(int i=0;i<n;i+=n-1){
        for(int j=0;j<n;j++){
            lab[i][j]=PARED;
        }
    }
    for(int i=0;i<n;i+=n-1){
        for(int j=0;j<n;j++){
            lab[j][i]=PARED;
        }
    }

}

void seleccionarAccesos(char ** lab,int n,int * sX,int * sY,int * eX,int * eY){
    srand(time(NULL));
    int lado=rand()%4,
    salida=rand()%(n-2)+1,
    entrada=rand()%(n-2) + 1;

    switch(lado){
        case 0:///0=ARRRIBA
            *sX=n-2;
            *sY=salida;
            *eX=1;
            *eY=entrada;

            lab[0][entrada]='E';


            lab[n-1][salida]='S';
            lab[n-2][salida]=CAMINO;
            break;

        case 1:///1=DERECHA
            *sX = salida;
            *sY = n - 2;
            *eX = entrada;
            *eY = 1;

            lab[entrada][0] = 'E';

            lab[salida][n - 1] = 'S';
            lab[salida][n - 2] = CAMINO;
            break;

        case 2:///ABAJO
            *sX=1;
            *sY=salida;
            *eX=n-2;
            *eY=entrada;

            lab[n-1][entrada]='E';

            lab[0][salida]='S';
            lab[1][salida]=CAMINO;
            break;

        case 3:///IZQUIERDA
            *sX = salida;
            *sY = 1;
            *eX = entrada;
            *eY = n - 2;

            lab[entrada][n - 1] = 'E';

            lab[salida][0] = 'S';
            lab[salida][1] = CAMINO;
            break;
    }
}
void generarCaminoAleatorio(char **lab, int n, int eX, int eY, int sX, int sY){
     // Marco la celda actual por las dudas (despues pruebo sin ellas cuando lo pase a TDA)
    lab[eX][eY] = CAMINO;

    int dirs[4] = {0,1,2,3};

    for (int i = 0; i < 4; i++) {
        int r = rand() % 4;
        int t = dirs[i]; dirs[i] = dirs[r]; dirs[r] = t;
    }

    for (int i = 0; i < 4; i++) {
        int nx = eX, ny = eY;

        if (dirs[i] == 0) nx -= 2;      /// arriba
        else if (dirs[i] == 1) nx += 2; /// abajo
        else if (dirs[i] == 2) ny -= 2; /// izquierda
        else ny += 2;                   /// derecha

        if (nx > 0 && ny > 0 && nx < n-1 && ny < n-1) {
            int esSalida = (nx == sX && ny == sY);
            if (lab[nx][ny] == PARED || esSalida) {
                // abro la pared intermedia
                lab[(eX + nx) / 2][(eY + ny) / 2] = CAMINO;

                lab[nx][ny] = CAMINO;
                generarCaminoAleatorio(lab, n, nx, ny, sX, sY);
            }
        }
    }
}

void imprimirLaberinto(char **lab,int n){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            printf("%c ", lab[i][j]);
        }
        printf("\n");
    }
}

void generarFantasmas(char **lab, int n, int eX, int eY, int cantidadFantasmas,Fantasma f[]) {
    int fantasmasColocados = 0;
    int radioSeguridad = (int)(n * 0.20);

    // Forzar un mínimo de 2 celdas de radio en laberintos pequeños
    if (radioSeguridad < 2) {
        radioSeguridad = 2;
    }

    // Inicializa el generador de números aleatorios si no se ha hecho
    // srand(time(NULL));

    while (fantasmasColocados < cantidadFantasmas) {
        // Generar coordenadas aleatorias dentro de los límites del laberinto
        int fX = rand() % (n - 2) + 1;
        int fY = rand() % (n - 2) + 1;

        // Calcular la distancia de Manhattan para la verificación rápida
        int distancia = abs(fX - eX) + abs(fY - eY);

        // Verificar si la celda es un camino y está fuera del radio de seguridad
        if (lab[fX][fY] == CAMINO && distancia > radioSeguridad) {
            lab[fX][fY] = FANTASMA;
            f[fantasmasColocados].x=fX;
            f[fantasmasColocados].y=fY;
            fantasmasColocados++;
        }
    }
}

void moverFantasmas(char ** lab,int n,Fantasma f[],int cantidadFantasmas){
    int dire,i=0,nX,nY;
    while(i<cantidadFantasmas){
        dire=rand()%(n-2)+1;
        nX=f[i].x;
        nY=f[i].y;
        switch(dire){
            case 0:///ARRIBA
                if(lab[nX+1][nY]==CAMINO){
                        f[i].x++;
                        lab[nX+1][nY]=FANTASMA;
                        lab[nX][nY]=CAMINO;
                        i++;
                }
                break;
            case 1:///DERECHA
                if(lab[nX][nY+1]==CAMINO){
                        f[i].y++;
                        lab[nX][nY+1]=FANTASMA;
                        lab[nX][nY]=CAMINO;
                        i++;
                }
                break;
            case 2:///ABAJO
                if(lab[nX-1][nY]==CAMINO){
                        f[i].x--;
                        lab[nX-1][nY]=FANTASMA;
                        lab[nX][nY]=CAMINO;
                        i++;
                }
                break;
            case 3:///IZQUIERDA
                if(lab[nX][nY-1]==CAMINO){
                        f[i].y--;
                        lab[nX][nY-1]=FANTASMA;
                        lab[nX][nY]=CAMINO;
                        i++;
                }
                break;
        }
    }
}
