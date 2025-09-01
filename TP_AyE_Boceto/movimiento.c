#include "movimiento.h"

HANDLE mutex;

void ingresarJugador(tLaberinto * l){
    l->lab[l->entradaX][l->entradaY] = PLAYER;
    l->jugador.fil = l->entradaX;
    l->jugador.col = l->entradaY;
    l->estado=JUGANDO;
}

DWORD WINAPI hiloFantasmas(LPVOID arg) {
    ThreadArgs *args = (ThreadArgs*)arg;

    while (*(args->running)) {
        Sleep(3000); // 3 segundos
        if(moverFantasmas(args->lab, args->fantasmas, args->cantFant))
        {
            *args->estado=PERDIO;
            *args->running=0;
            WaitForSingleObject(mutex, INFINITE);
            system("cls");
            imprimirLaberinto(args->lab, args->col,args->fil);
            ReleaseMutex(mutex);
        }
        WaitForSingleObject(mutex, INFINITE);
        system("cls");
        imprimirLaberinto(args->lab, args->col,args->fil);
        ReleaseMutex(mutex);


    }

    return 0;
}

int terminarJuego(tLaberinto * l){
    int running = 1;
    ThreadArgs args;
    args.lab = l->lab;
    args.fil = l->filas;
    args.col = l->columnas;
    args.cantFant = l->cantFantasmas;
    args.fantasmas = l->fantasmas;
    args.running = &running;
    args.estado= &l->estado;

    mutex = CreateMutex(NULL, FALSE, NULL);
    HANDLE hThread = CreateThread(
        NULL,
        0,
        hiloFantasmas,
        &args,
        0,
        NULL
    );



    while ((l->jugador.fil != l->salidaX || l->jugador.col != l->salidaY) && l->estado == JUGANDO) {
        if (!handleMovimiento(l)) {
            // jugador chocó contra un fantasma
            l->estado = PERDIO;
            break;
        }

        WaitForSingleObject(mutex, INFINITE);
        system("cls");
        imprimirLaberinto(l->lab, l->columnas, l->filas);
        ReleaseMutex(mutex);
        Sleep(1000);
    }

    // detener hilo de fantasmas
    running = 0;
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    CloseHandle(mutex);

    if (l->estado == PERDIO) {
        printf("¡Perdiste!\n");
    } else if (l->jugador.fil == l->salidaX && l->jugador.col == l->salidaY) {
        l->estado = GANO;
        printf("¡Ganaste!\n");
    }
    return 1;
}

int handleMovimiento(tLaberinto *l){
    int mov;
    int movFil = 0, movCol = 0;

    if (!_kbhit()) {
        return 1;  // no hacer nada, seguir jugando
    }

    mov = _getch();
    switch(toupper(mov)){
        case 'W':   // arriba
                    switch(l->lab[l->jugador.fil-1][l->jugador.col]){
                        case CAMINO: movFil = -1; break;
                        case FANTASMA: return 0; // Perdió
                    }
                    break;
        case 'A':   // izquierda
                    switch(l->lab[l->jugador.fil][l->jugador.col-1]){
                        case CAMINO: movCol = -1; break;
                        case FANTASMA: return 0; // Perdió
                    }
                    break;
        case 'S':   // abajo
                    switch(l->lab[l->jugador.fil+1][l->jugador.col]){
                        case CAMINO: movFil = 1; break;
                        case FANTASMA: return 0; // Perdió
                    }
                    break;
        case 'D':   // derecha
                    switch(l->lab[l->jugador.fil][l->jugador.col+1]){
                        case CAMINO: movCol = 1; break;
                        case FANTASMA: return 0; // Perdió
                    }
                    break;
        default: printf("INPUT INVALIDO \n");
                    return 1;
    }

    if (movFil != 0 || movCol != 0) {
        l->lab[l->jugador.fil][l->jugador.col] = CAMINO;
        l->jugador.fil += movFil;
        l->jugador.col += movCol;
        l->lab[l->jugador.fil][l->jugador.col] = PLAYER;
    }

    return 1;
}

