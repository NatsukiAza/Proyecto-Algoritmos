#include "generador.h"
#include "movimiento.h"

int main() {
    int n, cantidadFantasmas;
    int sX,sY,eX,eY;

    puts("\nIngrese el numero de filas y columnas del laberinto:");
    scanf("%d",&n);
    puts("\nIngrese la cantidad de fantasmas:");
    scanf("%d",&cantidadFantasmas);

    Fantasma fantasmas[cantidadFantasmas];
    char ** lab=(char**)malloc(n*sizeof(char*));
    for(int i=0;i<n;i++){
        lab[i]=(char*) malloc(n*sizeof(char));
        for(int j=0;j<n;j++){
            lab[i][j] = PARED;
        }
    }

    rellenarBordes(lab,n);
    seleccionarAccesos(lab,n,&sX,&sY,&eX,&eY);
    generarCaminoAleatorio(lab, n, eX, eY, sX, sY);
    generarFantasmas(lab, n, eX, eY, cantidadFantasmas,fantasmas);
    ingresarJugador(lab, eX, eY);
    imprimirLaberinto(lab, n);
    if(!terminarJuego(lab, eX, eY, sX, sY, n, cantidadFantasmas, fantasmas)){
        printf("PERDISTE MALETA!!! \n");
        getch();
    } else {
        printf("GANASTE LOCO!!!! \n");
        getch();
    }


    for(int i=0;i<n;i++) free(lab[i]);
    free(lab);
    return 0;
}
