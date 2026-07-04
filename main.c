#include<stdio.h>
#include<stdlib.h>

main(){
    int option=1, menu;
    char flag;
    do{
        printf("Hola");
        printf("Ingrese opcion: ");
        scanf("%d", &menu);
        switch (menu){
            case 1:
                printf("%d\n", menu);
                break;
            case 2:
                printf("%d\n", menu);
                break;
            case 0:
                printf("Desea salir? Y/N ");
                scanf(" %c", &flag); //El espacio antes del scanf evita problemas de lectura
                if (flag == 'Y' || flag == 'y'){
                    printf("gracias por jugar\n");
                    option=0;
                }
                if (flag != 'N' && flag != 'n' && flag != 'Y' && flag != 'y'){
                    printf("Option invalida\n");
                }
                break;
            default:
                printf("default\n");
                break;
        }
    }while (option != 0);
    return 0;
}