#include"movimiento.h"

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

int guardarMovimiento(const char* archi, int mov)
{
    FILE*fp = fopen(archi, "at");
    if(!fp)
        return 1;
    fprintf(fp, "%c", toupper(mov));
    fclose(fp);
    return TODO_OK;
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

int iniciarLogMovimientos(const char* archi)
{
    FILE*fp = fopen(archi, "wt");
    if(!fp)
        return 1;

    fclose(fp);
    return TODO_OK;
}

int verMovimientosPartida(const char* archi)
{
    FILE*fp = fopen(archi, "rt");
    if(!fp)
        return 1;

    char buffer[BUFFSIZE];
    fgets(buffer, BUFFSIZE, fp);
    printf("\n\nTus movimientos fueron:\n%s", buffer);
    fclose(fp);
    return 0;
}
