#include"movimiento.h"



//int terminarJuego(tLaberinto * l){
//    int salidaFil=l->salidaY;
//    int salidaCol=l->salidaX;
//    int estado;
//    while(l->lab[salidaFil][salidaCol] != PLAYER){
//        estado = handleMovimiento(l->lab, l->entradaX, l->entradaY, &l->jugador.vidas, &l->jugador.premios);
//        if(estado==PERDIO){
//            l->jugador.vidas--;
//            l->jugador.fil = l->entradaX;
//            l->jugador.col = l->entradaY;
//            if(l->jugador.vidas <= 0)
//                return PERDIO;
//        }else if(estado==GANO)return GANO;
//
//        moverFantasmas(l->lab,l->fantasmas,l->cantFantasmas,l->filas,l->columnas);
//        system("cls");
//        imprimirLaberinto(l->lab,l->columnas,l->filas);
//    }
//    return 1;
//}
//
//void ingresarMovimiento(tCola *colaJugador, int entradaFil, int entradaCol, int* vidas, int* premios) {
//    // No llames a crearCola aquí. Debe estar en la función que lo invoca.
//    printf("Controles: WASD | Vidas: %d | Premios: %d\n", *vidas, *premios);
//    int mov = getch();
//    encolar(colaJugador, &mov, sizeof(mov));
//}
//
//int handleMovimiento(char **lab, int entradaFil, int entradaCol, int* vidas, int* premios) {
//    int mov;
//    int movFil = 0, movCol = 0;
//    static int playerCord[2], flag = 0;
//
//    if (!flag) {
//        playerCord[0] = entradaFil;
//        playerCord[1] = entradaCol;
//        flag = 1;
//    }
//
//    tCola colaJugador;
//    crearCola(&colaJugador);
//
//    ingresarMovimiento(&colaJugador, entradaFil, entradaCol, vidas, premios);
//
//    desencolar(&colaJugador, &mov, sizeof(mov));
//    switch(toupper(mov)){
//        case 'W':   switch(lab[playerCord[0]-1][playerCord[1]]){
//                        case CAMINO: movFil = -1;
//                        break;
//                        case VIDA: (*vidas)++; lab[playerCord[0]-1][playerCord[1]] = CAMINO; movFil = -1;
//                        break;
//                        case PREMIO: (*premios)++; lab[playerCord[0]-1][playerCord[1]] = CAMINO; movFil = -1;
//                        break;
//                        case FANTASMA: return PERDIO;
//                        case SALIDA:return GANO;
//                    }
//        break;
//        case 'A':   switch(lab[playerCord[0]][playerCord[1]-1]){
//                        case CAMINO: movCol = -1;
//                        break;
//                        case VIDA: (*vidas)++; lab[playerCord[0]][playerCord[1]-1] = CAMINO; movCol = -1;
//                        break;
//                        case PREMIO: (*premios)++; lab[playerCord[0]][playerCord[1]-1] = CAMINO; movCol = -1;
//                        break;
//                        case FANTASMA: return PERDIO;
//                        case SALIDA:return GANO;
//                    }
//        break;
//        case 'S':   switch(lab[playerCord[0]+1][playerCord[1]]){
//                        case CAMINO: movFil = 1;
//                        break;
//                        case VIDA: (*vidas)++; lab[playerCord[0]+1][playerCord[1]] = CAMINO; movFil = 1;
//                        break;
//                        case PREMIO: (*premios)++; lab[playerCord[0]+1][playerCord[1]] = CAMINO; movFil = 1;
//                        break;
//                        case FANTASMA: return PERDIO;
//                        case SALIDA:return GANO;
//                    }
//        break;
//        case 'D':   switch(lab[playerCord[0]][playerCord[1]+1]){
//                        case CAMINO: movCol = 1;
//                        break;
//                        case VIDA: (*vidas)++; lab[playerCord[0]][playerCord[1]+1] = CAMINO; movCol = 1;
//                        break;
//                        case PREMIO: (*premios)++; lab[playerCord[0]][playerCord[1]+1] = CAMINO; movCol = 1;
//                        break;
//                        case FANTASMA: return PERDIO;
//                        case SALIDA:return GANO;
//                    }
//        break;
//        default: printf("INPUT INVALIDO \n");
//        break;
//    }
//
//    lab[playerCord[0]][playerCord[1]] = CAMINO;
//    playerCord[0] += movFil;
//    playerCord[1] += movCol;
//    lab[playerCord[0]][playerCord[1]] = PLAYER;
//
//    vaciarCola(&colaJugador);
//    return JUGANDO;
//}

static void respawnJugador(tLaberinto *l) {
    // limpiar donde estaba el jugador (si sigue el PLAYER ahí)
    if (l->lab[l->jugador.fil][l->jugador.col] == PLAYER)
        l->lab[l->jugador.fil][l->jugador.col] = CAMINO;

    // volver a entrada
    l->jugador.fil = l->entradaX;
    l->jugador.col = l->entradaY;
    l->lab[l->jugador.fil][l->jugador.col] = PLAYER;
}

int terminarJuego(tLaberinto *l) {
    int salidaFil = l->salidaY;
    int salidaCol = l->salidaX;
    int estado;

    while (l->lab[salidaFil][salidaCol] != PLAYER) {
        estado = handleMovimiento(l);

        if (estado == PERDIO) {
            l->jugador.vidas--;
            if (l->jugador.vidas <= 0)
                return PERDIO;
            respawnJugador(l);
        } else if (estado == GANO) {
            return GANO;
        }

        // mover fantasmas y chequear si alguno te pisa
        int atrapado = moverFantasmas(l->lab, l->fantasmas, &l->cantFantasmas, l->filas, l->columnas);
        if (atrapado) {
            l->jugador.vidas--;
            if (l->jugador.vidas <= 0)
                return PERDIO;
            respawnJugador(l);
        }

        system("cls");
        imprimirLaberinto(l->lab, l->columnas, l->filas);
    }
    return 1;
}

void ingresarMovimiento(tCola *colaJugador, tLaberinto *l) {
    printf("Controles: WASD | Vidas: %d | Premios: %d\n", l->jugador.vidas, l->jugador.premios);
    int mov = getch();
    guardarMovimiento("movimientos.txt", mov);
    encolar(colaJugador, &mov, sizeof(mov));
}

int handleMovimiento(tLaberinto *l) {
    int mov;
    int movFil = 0, movCol = 0;

    tCola colaJugador;
    crearCola(&colaJugador);
    ingresarMovimiento(&colaJugador, l);
    desencolar(&colaJugador, &mov, sizeof(mov));

    // decidir delta
    switch (toupper(mov)) {
        case 'W': movFil = -1; l->jugador.cantMov++; break;
        case 'S': movFil =  1; l->jugador.cantMov++; break;
        case 'A': movCol = -1; l->jugador.cantMov++; break;
        case 'D': movCol =  1; l->jugador.cantMov++; break;
        default:
            printf("INPUT INVALIDO\n");
            vaciarCola(&colaJugador);
            return JUGANDO;
    }

    int nextFil = l->jugador.fil + movFil;
    int nextCol = l->jugador.col + movCol;

    char celda = l->lab[nextFil][nextCol];

    switch (celda) {
        case CAMINO:
            break;
        case VIDA:
            l->jugador.vidas++;
            l->lab[nextFil][nextCol] = CAMINO;
            break;
        case PREMIO:
            l->jugador.premios++;
            l->lab[nextFil][nextCol] = CAMINO;
            break;
        case FANTASMA:
            borrarFantasmaEn(l, nextFil, nextCol);
            vaciarCola(&colaJugador);
            return PERDIO;
        case SALIDA:
            vaciarCola(&colaJugador);
            return GANO;
        case PARED:
        default:
            vaciarCola(&colaJugador);
            return JUGANDO;
    }

    l->lab[l->jugador.fil][l->jugador.col] = CAMINO;
    l->jugador.fil = nextFil;
    l->jugador.col = nextCol;
    l->lab[l->jugador.fil][l->jugador.col] = PLAYER;

    vaciarCola(&colaJugador);
    return JUGANDO;
}

void borrarFantasmaEn(tLaberinto *l, int fil, int col) {
    //limpiar la celda del tablero si todavía tiene F
    if (l->lab[fil][col] == FANTASMA)
        l->lab[fil][col] = CAMINO;

    //elimino fantasma del vector
    for (int i = 0; i < l->cantFantasmas; i++) {
        if (l->fantasmas[i].fil == fil && l->fantasmas[i].col == col) {
            for (int j = i; j < l->cantFantasmas - 1; j++)
                l->fantasmas[j] = l->fantasmas[j + 1];
            l->cantFantasmas--;
            break;
        }
    }
}

int iniciarLogMovimientos(const char* archi, char nombre[])
{
    FILE*fp = fopen(archi, "at");
    if(!fp)
        return 1;

    fprintf(fp, "\n\n||||| User: %s |||||\n", nombre);
    fclose(fp);
    return TODO_OK;
}

int guardarMovimiento(const char* archi, int mov)
{
    FILE*fp = fopen(archi, "at");
    if(!fp)
        return 1;
    fprintf(fp, "%c", toupper(mov));
    fclose(fp);
    return TODO_OK;
}
