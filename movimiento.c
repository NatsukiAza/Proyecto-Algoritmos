#include "movimiento.h"


void ingresarJugador(char **lab, int entradaFil, int entradaCol){

    lab[entradaFil][entradaCol] = PLAYER;
}

int terminarJuego(char **lab, int entradaFil, int entradaCol, int salidaFil, int salidaCol, int n, int cantFant, Fantasma *fantasmas){
    time_t tiempoInicial, tiempoActual;
    tiempoInicial = time(NULL);
    while(lab[salidaFil][salidaCol] != PLAYER){
        tiempoActual = time(NULL);
        if(!handleMovimiento(lab, entradaFil, entradaCol)){
            return 0;
        }
        if (difftime(tiempoActual, tiempoInicial) >= 3.0) {
            moverFantasmas(lab,n,fantasmas,cantFant);
            tiempoInicial = tiempoActual;
        }
        system("cls");
        imprimirLaberinto(lab, n);
    }
    return 1;
}

int handleMovimiento(char **lab, int entradaFil, int entradaCol){
    static int playerCord[2], flag = 0;
    int mov, movFil = 0, movCol = 0;
    if(!flag){
        playerCord[0] = entradaFil;
        playerCord[1] = entradaCol;
        flag = 1;
    }

    printf("Controles: WASD.... \n");
    mov = getch();
    switch(toupper(mov)){
        case 'W':   switch(lab[playerCord[0]-1][playerCord[1]]){
                        case CAMINO: movFil = -1;
                        break;
                        case FANTASMA: return 0;
                    }
        break;
        case 'A':   switch(lab[playerCord[0]][playerCord[1]-1]){
                        case CAMINO: movCol = -1;
                        break;
                        case FANTASMA: return 0;
                    }
        break;
        case 'S':   switch(lab[playerCord[0]+1][playerCord[1]]){
                        case CAMINO: movFil = 1;
                        break;
                        case FANTASMA: return 0;
                    }
        break;
        case 'D':   switch(lab[playerCord[0]][playerCord[1]+1]){
                        case CAMINO: movCol = 1;
                        break;
                        case FANTASMA: return 0;
                    }
        break;
        default: printf("INPUT INVALIDO \n");
        break;
    }
    lab[playerCord[0]][playerCord[1]] = CAMINO;
    playerCord[0] += movFil;
    playerCord[1] += movCol;
    lab[playerCord[0]][playerCord[1]] = PLAYER;
    return 1;
}
