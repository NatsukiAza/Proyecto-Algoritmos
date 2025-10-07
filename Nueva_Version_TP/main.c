#include "generador.h"
#include "movimiento.h"

void leerDatos(tLaberinto * l){
    FILE * arch = fopen("config.txt","r");
    if (arch == NULL) {
        printf("Error: No se pudo abrir el archivo de configuracion.\n");
        return;
    }

    int leidos = 0;

    leidos = fscanf(arch," filas:%d",&(l->filas));
    if (leidos != 1) { printf("Error al leer filas.\n"); return; }

    leidos = fscanf(arch," columnas:%d",&(l->columnas));
    if (leidos != 1) { printf("Error al leer columnas.\n"); return; }

    leidos = fscanf(arch," vidas_inicio:%d",&(l->jugador.vidas));
    if (leidos != 1) { printf("Error al leer vidas.\n"); return; }

    leidos = fscanf(arch," maximo_numero_fantasmas:%d",&(l->cantFantasmas));
    if (leidos != 1) { printf("Error al leer fantasmas.\n"); return; }

    leidos = fscanf(arch," maximo_numero_premios:%d",&(l->maximosPremios));
    if (leidos != 1) { printf("Error al leer premios.\n"); return; }

    leidos = fscanf(arch," maximo_vidas_extra:%d",&(l->maxVidasExtra));
    if (leidos != 1) { printf("Error al leer vidas extra.\n"); return; }

    fclose(arch);

    //printf("\n%d  %d %d %d %d %d",l->filas,l->columnas,l->jugador.vidas,l->cantFantasmas,l->maximosPremios,l->maxVidasExtra);
}


void  inicializarLaberinto(tLaberinto * l){
    l->lab=(char**)malloc(l->filas * sizeof(char*));
    for(int i=0;i<l->filas;i++){
        l->lab[i]=(char*) malloc(l->columnas * sizeof(char));
        for(int j=0;j<l->columnas;j++){
            l->lab[i][j]=PARED;
        }
    }
}

int main() {
    tLaberinto laberinto;

    leerDatos(&laberinto);

    inicializarLaberinto(&laberinto);
    //rellenarBordes(&laberinto);
    seleccionarAccesos(&laberinto);
    generarCaminoAleatorio(laberinto.lab,laberinto.filas,laberinto.columnas,laberinto.salidaX,laberinto.salidaY, laberinto.entradaX, laberinto.entradaY);
    generarFantasmas(&laberinto);
    generarJugador(&laberinto);
    imprimirLaberinto(laberinto.lab,laberinto.columnas,laberinto.filas);

    if(terminarJuego(&laberinto)==PERDIO){
        printf("PERDISTE MALETA!!! \n");
        getch();
    } else {
        printf("GANASTE LOCO!!!! \n");
        getch();
    }


    for(int i=0;i<laberinto.filas;i++) free(laberinto.lab[i]);
    free(laberinto.lab);
    return 0;
}
