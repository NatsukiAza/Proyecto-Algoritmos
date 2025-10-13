#include "movimiento.h"


void ingresarJugador(char **lab, int entradaFil, int entradaCol){

    lab[entradaFil][entradaCol] = PLAYER;
}

int terminarJuego(char **lab, int entradaFil, int entradaCol, int salidaFil, int salidaCol, int n, int cantFant, Fantasma *fantasmas, int* cantVidas, int* cantVidasExtra){
    time_t tiempoInicial, tiempoActual;
    tiempoInicial = time(NULL);
    while(lab[salidaFil][salidaCol] != PLAYER){
        tiempoActual = time(NULL);
        if(!handleMovimiento(lab, entradaFil, entradaCol, cantVidas, cantVidasExtra)){
            return 0;
        }
        if (difftime(tiempoActual, tiempoInicial) >= 2.0) {
            moverFantasmas(lab,n,fantasmas,cantFant);
            tiempoInicial = tiempoActual;
        }
        system("cls");
        imprimirLaberinto(lab, n);
    }
    return 1;
}

int handleMovimiento(char **lab, int entradaFil, int entradaCol, int* cantVidas, int* cantVidasExtra){
    static int playerCord[2], flag = 0;
    int mov, movFil = 0, movCol = 0;
    if(!flag){
        playerCord[0] = entradaFil;
        playerCord[1] = entradaCol;
        flag = 1;
    }
    int cantVidasActual = *cantVidas;
    printf("Controles: WASD....  Vidas: %d\n", *cantVidas);
    mov = getch();
    switch(toupper(mov)){
        case 'W':   switch(lab[playerCord[0]-1][playerCord[1]]){
                        case CAMINO: movFil = -1;
                        break;
                        case VIDA: movFil = -1; lab[playerCord[0]][playerCord[1]] = CAMINO; (*cantVidas)++; *(cantVidasExtra)--; break;
                        case FANTASMA: (*cantVidas)--; lab[playerCord[0]][playerCord[1]] = CAMINO; return 0; //lab[playerCord[0]-1][playerCord[1]] = CAMINO; return 0;
                    }
        break;
        case 'A':   switch(lab[playerCord[0]][playerCord[1]-1]){
                        case CAMINO: movCol = -1;
                        break;
                        case VIDA: movCol = -1; lab[playerCord[0]][playerCord[1]] = CAMINO; (*cantVidas)++; *(cantVidasExtra)--; break;
                        case FANTASMA: (*cantVidas)--; lab[playerCord[0]][playerCord[1]] = CAMINO; return 0; //lab[playerCord[0]][playerCord[1]-1] = CAMINO; return 0;
                    }
        break;
        case 'S':   switch(lab[playerCord[0]+1][playerCord[1]]){
                        case CAMINO: movFil = 1;
                        break;
                        case VIDA: movFil = 1; lab[playerCord[0]][playerCord[1]] = CAMINO; (*cantVidas)++; *(cantVidasExtra)--; break;
                        case FANTASMA: (*cantVidas)--; lab[playerCord[0]][playerCord[1]] = CAMINO; return 0; //lab[playerCord[0]+1][playerCord[1]] = CAMINO; return 0;
                    }
        break;
        case 'D':   switch(lab[playerCord[0]][playerCord[1]+1]){
                        case CAMINO: movCol = 1;
                        break;
                        case VIDA: movCol = 1; lab[playerCord[0]][playerCord[1]] = CAMINO; (*cantVidas)++; *(cantVidasExtra)--; break;
                        case FANTASMA: (*cantVidas)--; lab[playerCord[0]][playerCord[1]] = CAMINO; return 0; //lab[playerCord[0]][playerCord[1]+1] = CAMINO;
                    }
        break;
        default: printf("INPUT INVALIDO \n");
        break;
    }
    if(cantVidasActual == *cantVidas || cantVidasActual < *cantVidas)
    {
        lab[playerCord[0]][playerCord[1]] = CAMINO;
        playerCord[0] += movFil;
        playerCord[1] += movCol;
        lab[playerCord[0]][playerCord[1]] = PLAYER;
    }
    else if(cantVidasActual > *cantVidas)
    {
        lab[playerCord[0]][playerCord[1]] = CAMINO;
    }

    return 1;
}
