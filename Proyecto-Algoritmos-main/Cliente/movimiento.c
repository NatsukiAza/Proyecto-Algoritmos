#include"movimiento.h"

void respawnJugador(tLaberinto *l) {
    // limpiar donde estaba el jugador (si sigue ahí)
    if (l->lab[l->jugador.fil][l->jugador.col] == PLAYER)
        l->lab[l->jugador.fil][l->jugador.col] = CAMINO;

    // volver a entrada
    l->jugador.fil = l->entradaX;
    l->jugador.col = l->entradaY;
    l->lab[l->jugador.fil][l->jugador.col] = PLAYER;
}

int terminarJuego(tLaberinto *l) {
    int salidaFil = l->salidaX;
    int salidaCol = l->salidaY;
    int estado = -1;
    int contCheat = 0;

    while (l->lab[salidaFil][salidaCol] != PLAYER) {
        estado = handleMovimiento(l, &contCheat); //llamo a captar movimiento

        if (estado == PERDIO) {
            l->jugador.vidas--;
            if (l->jugador.vidas <= 0){
                return PERDIO;
            }
            respawnJugador(l);
        }else if (estado == GANO) {
            return GANO;
        }

        //mover fantasmas y chequear si alguno te atrapa
        int atrapado = moverFantasmas(l->lab, l->fantasmas, &l->cantFantasmas, l->filas, l->columnas);
        if (atrapado) {
            l->jugador.vidas--;
            if (l->jugador.vidas <= 0) {
//                printf("perdiste por fantasma (post-mov)\n"); getch();
                return PERDIO;
            }
//            printf("te piso un fantasma, respawn\n"); getch();
            respawnJugador(l);
        }

        system("cls");
        imprimirLaberinto(l->lab, l->columnas, l->filas);
    }
    return estado;
}

int guardarMovimiento(const char* archi, int mov)
{
    FILE*fp = fopen(archi, "at");
    if(!fp)
        return 1;
    fprintf(fp, "%c", toupper(mov));
    fclose(fp);
    return 0;
}

void ingresarMovimiento(tCola *colaJugador, tLaberinto *l) {
    printf("Controles: WASD | Vidas: %d | Premios: %d\n", l->jugador.vidas, l->jugador.premios);
    int mov = getch();
    guardarMovimiento("movimientos.txt", mov);
    encolar(colaJugador, &mov, sizeof(mov));
}

int handleMovimiento(tLaberinto *l, int* contCheat) {
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
        case 'N': (*contCheat)++; break;
        default:
            printf("INPUT INVALIDO\n");
            vaciarCola(&colaJugador);
    }

    if(*contCheat == 3){ //en caso de activar el "cheat"
        l->lab[l->jugador.fil][l->jugador.col] = CAMINO;
        l->jugador.fil = l->salidaX;
        l->jugador.col = l->salidaY;
        l->lab[l->jugador.fil][l->jugador.col] = PLAYER;
        vaciarCola(&colaJugador);
        return GANO;;
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
            borrarFantasma(l, nextFil, nextCol);
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
    //rescribo el lab con las nuevas celdas
    l->lab[l->jugador.fil][l->jugador.col] = CAMINO;
    l->jugador.fil = nextFil;
    l->jugador.col = nextCol;
    l->lab[l->jugador.fil][l->jugador.col] = PLAYER;

    vaciarCola(&colaJugador);
    return JUGANDO;
}

void borrarFantasma(tLaberinto *l, int fil, int col) {
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

int iniciarLogMovimientos(const char* archi)
{
    FILE*fp = fopen(archi, "wt");
    if(!fp)
        return 1;

    fclose(fp);
    return 0;
}

int verMovimientosPartida(const char* archi)
{
    FILE*fp = fopen(archi, "rt");
    if(!fp)
        return 1;

    char buffer[BUFFSIZE];
    fgets(buffer, BUFFSIZE, fp);
    printf("\nTus movimientos fueron:\n%s", buffer);
    fclose(fp);
    return 0;
}
