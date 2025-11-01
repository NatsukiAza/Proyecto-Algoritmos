#include "generador.h"

int dx[4] = { -2, 2, 0, 0 };
int dy[4] = { 0, 0, -2, 2 };

void leerDatos(tLaberinto * l)
{
    FILE * arch = fopen("config.txt","r");
    int limSupFantasmas,limInfFantasmas;
    if (arch == NULL)
    {
        printf("Error: No se pudo abrir el archivo de configuracion.\n");
        return;
    }

    int leidos = 0;

    leidos = fscanf(arch,"filas:%d",&(l->filas));
    if (leidos != 1)
    {
        printf("Error al leer filas.\n");
        return;
    }
    if(l->filas<8)l->filas=8;///COMO MINIMO EL TABLERO SERA DE 8X8 PARA UN LABERINTO JUGABLE

    leidos = fscanf(arch," columnas:%d",&(l->columnas));
    if (leidos != 1)
    {
        printf("Error al leer columnas.\n");
        return;
    }
    if(l->columnas<8)l->columnas=8;///COMO MINIMO EL TABLERO SERA DE 8X8 PARA UN LABERINTO JUGABLE

    limSupFantasmas=(l->columnas * l->filas)*0.08;
    limInfFantasmas=(l->columnas * l->filas)*0.02;

    leidos = fscanf(arch," vidas_inicio:%d",&(l->jugador.vidas));
    if (leidos != 1)
    {
        printf("Error al leer vidas.\n");
        return;
    }


    leidos = fscanf(arch," maximo_numero_fantasmas:%d",&(l->cantFantasmas));
    if (leidos != 1)
    {
        printf("Error al leer fantasmas.\n");
        return;
    }
    if(l->cantFantasmas<limInfFantasmas){
        l->cantFantasmas=limInfFantasmas;
    }
    else if(l->cantFantasmas>limSupFantasmas){
        l->cantFantasmas=limSupFantasmas;
    }

    leidos = fscanf(arch," maximo_numero_premios:%d",&(l->maximosPremios));
    if (leidos != 1)
    {
        printf("Error al leer premios.\n");
        return;
    }

    leidos = fscanf(arch," maximo_vidas_extra:%d",&(l->maxVidasExtra));
    if (leidos != 1)
    {
        printf("Error al leer vidas extra.\n");
        return;
    }

    fclose(arch);

    l->jugador.premios = 0;
    //printf("\n%d  %d %d %d %d %d",l->filas,l->columnas,l->jugador.vidas,l->cantFantasmas,l->maximosPremios,l->maxVidasExtra);
}


void  inicializarLaberinto(tLaberinto * l)
{
    l->lab=(char**)malloc(l->filas * sizeof(char*));
    for(int i=0; i<l->filas; i++)
    {
        l->lab[i]=(char*) malloc(l->columnas * sizeof(char));
        for(int j=0; j<l->columnas; j++)
        {
            l->lab[i][j]=PARED;
        }
    }
}

void generarCaminoAleatorio(char **lab, int filas, int columnas, int entradaX, int entradaY, int flag1, int flag2) {
    lab[entradaX][entradaY] = CAMINO;

    if(entradaX % 2 == 0 && !flag1){
        lab[entradaX][entradaY] = CAMINO;
        entradaX--;
    }

    if(entradaY % 2 == 0 && !flag2){
        lab[entradaX][entradaY] = CAMINO;
        entradaY--;
    }

    int dirs[4] = {0,1,2,3};
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4;
        int t = dirs[i];
        dirs[i] = dirs[r];
        dirs[r] = t;
    }

    for (int i = 0; i < 4; i++) {
        int nx = entradaX, ny = entradaY;

        if (dirs[i] == 0) nx -= 2;
        else if (dirs[i] == 1) nx += 2;
        else if (dirs[i] == 2) ny -= 2;
        else ny += 2;

        if (nx > 0 && ny > 0 && nx < filas-1 && ny < columnas-1) {
            if (lab[nx][ny] == PARED){
                lab[(entradaX + nx) / 2][(entradaY + ny)/2] = CAMINO;
                lab[nx][ny] = CAMINO;
//                imprimirLaberinto(lab, columnas, filas);
                generarCaminoAleatorio(lab, filas, columnas, nx, ny, 1, 1);
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


//void generarVidas(char ** lab,tFantasma * fantasmas,int n){
//
//}

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

void generarArchLaberinto(tLaberinto * l){
    FILE * arch=fopen("laberinto.txt","w");
    if(!arch){
        puts("\nError al imprimir laberinto.txt\n");
        return;
    }
    for(int i=0;i<l->filas;i++){
        for(int j=0;j<l->columnas;j++){
            fprintf(arch," %c ",l->lab[i][j]);
        }
        fprintf(arch,"\n");
    }
    fclose(arch);
}

int esBorde(int filas, int columnas, int i, int j)
{
    if(i == 0 || i == (filas - 1))
        return 1;
    if(j == 0 || j == (columnas - 1))
        return 1;
    return 0; //no es borde
}

int despejarSalida(char **lab, int filas, int columnas, int salidaX, int salidaY)
{
    int i, j;
    for(i = -1; i<2; i++)
    {
        for(j = -1; j<2; j++)
        {
            int x = salidaX + i;
            int y = salidaY + j;
            if(x > -1 && y > -1) //chequeo que este dentro de limites del laberinto
            {
                if(!esBorde(filas, columnas, x, y) && (x != salidaX || y != salidaY)) //chequeo que no sea borde del laberinto y que no sea la salida
                {
                    lab[x][y] = CAMINO;
                }
            }
        }
    }
    return 0;
}


