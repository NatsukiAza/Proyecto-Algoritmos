#include "generador.h"
#include "movimiento.h"

int main() {
    int n, cantidadFantasmas, vidasInit, vidasExtra, aux = 0;
    int sX,sY,eX,eY;

    puts("\nIngrese el numero de filas y columnas del laberinto:");
    scanf("%d",&n);
    puts("\nIngrese la cantidad de fantasmas:");
    scanf("%d",&cantidadFantasmas);
    puts("\nIngrese la cantidad de vidas inicial:");
    scanf("%d",&vidasInit);
    puts("\nIngrese la cantidad de vidas extra:");
    scanf("%d",&vidasExtra);

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
    generarVidasExtra(lab, n, vidasExtra);
    generarFantasmas(lab, n, eX, eY, cantidadFantasmas,fantasmas);
    ingresarJugador(lab, eX, eY);
    imprimirLaberinto(lab, n);

    while(vidasInit > 0 && aux != 1)
    {
        aux = terminarJuego(lab, eX, eY, sX, sY, n, cantidadFantasmas, fantasmas, &vidasInit, &vidasExtra);
        if(aux == 0)
        {
            if(vidasInit > 0)
                ingresarJugador(lab, eX, eY);
            else if(vidasInit == 0)
            {
                printf("PERDISTE MALETA!!! \n");
                getch();
            }
        }
        else
        {
            printf("GANASTE LOCO!!!! \n");
            getch();
        }
    }

//    if(!terminarJuego(lab, eX, eY, sX, sY, n, cantidadFantasmas, fantasmas, &vidasInit, &vidasExtra)){
//        if(vidasInit > 0) //pierde vida
//        {
//            ingresarJugador(lab, eX, eY);
//            terminarJuego(lab, eX, eY, sX, sY, n, cantidadFantasmas, fantasmas, &vidasInit, &vidasExtra);
//        }
//        else if(vidasInit == 0)
//        {
//            printf("PERDISTE MALETA!!! \n");
//            getch();
//        }
//    } else {
//        printf("GANASTE LOCO!!!! \n");
//        getch();
//    }


    for(int i=0;i<n;i++) free(lab[i]);
    free(lab);
    return 0;
}
