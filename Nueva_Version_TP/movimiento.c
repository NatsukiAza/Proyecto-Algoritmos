#include"movimiento.h"



int terminarJuego(tLaberinto * l){
    int salidaFil=l->salidaY;
    int salidaCol=l->salidaX;
    int estado;
    while(l->lab[salidaFil][salidaCol] != PLAYER){
        estado = handleMovimiento(l->lab, l->entradaX, l->entradaY);
        if(estado==PERDIO){
            return PERDIO;
        }else if(estado==GANO)return GANO;

        moverFantasmas(l->lab,l->fantasmas,l->cantFantasmas,l->filas,l->columnas);
        system("cls");
        imprimirLaberinto(l->lab,l->columnas,l->filas);
    }
    return 1;
}

void ingresarMovimiento(tCola *colaJugador, int entradaFil, int entradaCol) {
    // No llames a crearCola aquí. Debe estar en la función que lo invoca.
    printf("Controles: WASD.... \n");
    int mov = getch();
    encolar(colaJugador, &mov, sizeof(mov));
}

int handleMovimiento(char **lab, int entradaFil, int entradaCol) {
    int mov;
    int movFil = 0, movCol = 0;
    static int playerCord[2], flag = 0;

    if (!flag) {
        playerCord[0] = entradaFil;
        playerCord[1] = entradaCol;
        flag = 1;
    }

    tCola colaJugador;
    crearCola(&colaJugador);

    ingresarMovimiento(&colaJugador, entradaFil, entradaCol);

    desencolar(&colaJugador, &mov, sizeof(mov));
    switch(toupper(mov)){
        case 'W':   switch(lab[playerCord[0]-1][playerCord[1]]){
                        case CAMINO: movFil = -1;
                        break;
                        case FANTASMA: return PERDIO;
                        case SALIDA:return GANO;
                    }
        break;
        case 'A':   switch(lab[playerCord[0]][playerCord[1]-1]){
                        case CAMINO: movCol = -1;
                        break;
                        case FANTASMA: return PERDIO;
                        case SALIDA:return GANO;
                    }
        break;
        case 'S':   switch(lab[playerCord[0]+1][playerCord[1]]){
                        case CAMINO: movFil = 1;
                        break;
                        case FANTASMA: return PERDIO;
                        case SALIDA:return GANO;
                    }
        break;
        case 'D':   switch(lab[playerCord[0]][playerCord[1]+1]){
                        case CAMINO: movCol = 1;
                        break;
                        case FANTASMA: return PERDIO;
                        case SALIDA:return GANO;
                    }
        break;
        default: printf("INPUT INVALIDO \n");
        break;
    }

    lab[playerCord[0]][playerCord[1]] = CAMINO;
    playerCord[0] += movFil;
    playerCord[1] += movCol;
    lab[playerCord[0]][playerCord[1]] = PLAYER;

    vaciarCola(&colaJugador);
    return JUGANDO;
}
