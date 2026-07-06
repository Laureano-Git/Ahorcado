#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Constantes define
#define MAX_PALABRA 30 //Significa que cada palabra puede tener hasta
#define MAX_NOMBRE 30 //Es el tamaño máximo del nombre del jugador.
#define MAX_PALABRAS 100 //Cada categoría podrá almacenar hasta 100 palabras
#define MAX_LETRAS 26 //La usaremos para guardar las letras ingresadas.
#define CANT_CATEGORIAS 3 //Cantidad de categorias
#define MAX_LINEA 100 //Cantidad maxima de lineas

//Structs
//Jugador:
typedef struct Jugador{ //Usamos typedef para poder usar el alias "Jugador" más adelante
    char nombre[MAX_NOMBRE];
    int jugadas;
    int ganadas;
    int perdidas;
    int rachaActual;
    int rachaMaxima;
    struct Jugador *sig; //Se convierte el registro en un nodo de una lista enlazada
}Jugador;
//Categorias:
typedef struct Categoria{
    char nombre[20];
    char palabras[MAX_PALABRAS][MAX_PALABRA];
    int cantidad;
} Categoria;

//Protitipos
//Procedimiento cargar jugadores
void cargarJugadores(Jugador **listaJugadores); //Paso el puntero por referencia para modificarlo
//Procedimiento guardar jugadores
void guardarJugadores(Jugador *listaJugadores); //Paso el puntero por copia ya que no lo voy a modificar
//Procedimiento cargar palabras
void cargarPalabras(Categoria categorias[]); //Paso el arreglo de categorias por referencia 
//Procedimiento agregar palabras
void agregarPalabra(Categoria categorias[]); //Paso el arreglo de categorias por referencia
//Funcuion elegir jugador
Jugador *elegirJugador(Jugador **listaJugadores); //Paso el puntero por referencia
//Funcion elegir categoría
int elegirCategoria(Categoria categorias[]); //Paso la lista de categoria y retornara una posición (palabra elegida)
//Funcion elegir dificultad
int elegirDificultad(); //No se pasa parametro, únicamente retornara la dificultad elegida
//Funcion elegir palabra
char *seleccionarPalabra(Categoria categorias[], int categoria, int dificultad); //Retorna un puntero a Categorias en base a la categoria elegida y la dificultad
//Procedimiento para la logia del ahorcado
void jugar(Jugador *jugador, Categoria categorias[], int dificultad);
//Procedimiento del dibujo
void dibujarAhorcado(int errores); //Se pasa por copia
//Procedimiento del ranking
void mostrarRanking(Jugador *listaJugadores); //Se pasa por copia la cabeza de la lista
//Procedimiento para mostrar jugadores
void mostrarJugadores(Jugador *listaJugadores); //Se pasa por copia la cabeza de la lista
//Procedimiento para liberar memoria
void liberarLista(Jugador **listaJugadores); //Se pasa por referencia la cabeza de la lista
//Procedimiento para mantener limpio el buffer
void limpiarBuffer(); //No se pasa parametro, únicamente limpia el buffer con un getchar
//Procedimiento para inicializar las categorias
void inicializarCategorias(Categoria categorias[]); //Se pasa por referencia el arreglo
//Procedimiento para dibujar el ahorcado
void mostrarAhorcado(int errores);

//Cuerpo del programa
int main(){
    Jugador *listaJugadores = NULL; //Creamos el puntero "listaJugadores" que apunta al primer nodo de la lista enlazada, primero en Null sin jugadores
    Jugador *jugadorActual; //Puntero que guardara el jugador seleccionado
    //Categorias
    Categoria categorias[CANT_CATEGORIAS];
    //Variables flag
    int opcion;//Para el Menu
    int dificultad;//Para Dificultad
    char seguir;//Para que ingrese S/N si desea seguir jugando
    int victoriasNivel; //Para llevar un registro de victorias consecutivas por Nivel
    //Inicializo 3 categorias y sus cantidades de palabras
    inicializarCategorias(categorias);
    //Inicializacion de numeros aleatorios
    srand(time(NULL)); //Usa la hora actual como semilla para generar secuencias de numeros aleatorios cada vez que se ejecuta el programa entonces no repite secuencias, el rand() genera secuencias random en base a la misma semilla entonces cada ejecucion creara la misma secuencia random
    //Carga de los archivos
    cargarJugadores(&listaJugadores);
    cargarPalabras(categorias);
    do{
        printf("\n");
        printf("===== JUEGO DEL AHORCADO =====\n");
        printf("1. Jugar\n");
        printf("2. Agregar palabra\n");
        printf("3. Mostrar ranking\n");
        printf("4. Mostrar jugadores\n");
        printf("5. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d",&opcion);
        limpiarBuffer();
        switch(opcion){
        case 1:
            jugadorActual = elegirJugador(&listaJugadores); //El puntero jugadorActual toma la posicion en memoria del jugador que retorna la funcion elegirJugador
            dificultad = elegirDificultad(); //Elegimos dificultad
            seguir = 'S'; //Empezamos el ciclo al menos una vez
            victoriasNivel = 0; //Empezamos sin victorias consecutivas
            while(seguir == 'S' || seguir == 's'){
                jugar(jugadorActual, categorias, dificultad);
                if(jugadorActual->rachaActual > 0){
                    victoriasNivel++;
                }else{
                    victoriasNivel = 0;
                }
                if(victoriasNivel >= 2 && dificultad < 3){
                    dificultad++;
                    victoriasNivel = 0;
                    printf("\nSubiste al nivel ");
                    printf("%d", dificultad);
                    printf("!\n");
                }
                printf("\nDesea jugar otra partida? (S/N): ");
                scanf(" %c",&seguir);
                limpiarBuffer();
            }
            break;
        case 2:
            agregarPalabra(categorias);
            break;
        case 3:
            mostrarRanking(listaJugadores);
            break;
        case 4:
            mostrarJugadores(listaJugadores);
            break;
        case 5:
            printf("Gracias por jugar.\n");
            break;
        default:
            printf("Opcion incorrecta.\n");
        }
    }while(opcion != 5);
    guardarJugadores(listaJugadores);
    liberarLista(&listaJugadores);
    return 0;
}

void limpiarBuffer(){
    while(getchar() != '\n');
}

void inicializarCategorias(Categoria categorias[]){
    char nombres[CANT_CATEGORIAS][20] ={
        "Animales",
        "Paises",
        "Tecnologia"
    };
    int i;
    for(i = 0; i < CANT_CATEGORIAS; i++){
        strcpy(categorias[i].nombre,nombres[i]);
        categorias[i].cantidad = 0;
    }
}

void agregarPalabra(Categoria categorias[]){
    int categoria;
    int i;
    char palabra[MAX_PALABRA];
    FILE *archivo;
    categoria = elegirCategoria(categorias);
    //Verificacion que la categoria no haya alcanzado la cantidad maxima de palabras
    if(categorias[categoria].cantidad >= MAX_PALABRAS){
        printf("La categoria esta completa.\n");
        return;
    }
    printf("Ingrese la nueva palabra: ");
    fgets(palabra, MAX_PALABRA, stdin);
    palabra[strcspn(palabra,"\n")] = '\0'; //El usuario ingresa una palabra por ejemplo "gato" y el fgets lee "gato\n\0" con strcspn queda "gato\0"
    //Validacion que la palabra no este repetida
    for(i = 0; i < categorias[categoria].cantidad; i++){
        if(strcmp(categorias[categoria].palabras[i], palabra) == 0){
            printf("Esa palabra ya existe.\n");
            return;
        }
    }
    strcpy(categorias[categoria].palabras[categorias[categoria].cantidad],palabra); //Copia en el arreglo y al final de las palabras la "palabra"
    categorias[categoria].cantidad++; //Incrementa la cantidad de palabras
    switch(categoria){
        case 0:
            archivo = fopen("animales.txt","a"); //El parametro "a" significa "append" para agregar al final, sin borrar todo su contenido, diferente al "w"
            break;
        case 1:
            archivo = fopen("paises.txt","a");
            break;
        case 2:
            archivo = fopen("tecnologia.txt","a");
            break;
    }
    //Verificacion que el archivo se haya encontrado
    if(archivo == NULL){
        printf("Error al abrir el archivo.\n");
        return;
    }
    fprintf(archivo,"%s\n",palabra); //Escribe en el archivo abierto y al final la palabra
    fclose(archivo); //Cierra el archivo
    printf("Palabra agregada correctamente.\n");
}

void mostrarRanking(Jugador *lista){
    Jugador *aux; //Puntero auxiliar para recoorrer la lista
    Jugador ranking[100]; //Este arreglo contendra copias
    Jugador temp;
    int cantidad = 0; //Variable flas para llevar la cuenta de cuantos copiamos
    aux = lista;
    int i; 
    int j;
    while(aux != NULL){
        ranking[cantidad] = *aux; //Copiamos en ranking toda la estructura de una vez
        cantidad++;
        aux = aux->sig;
        //Algoritmo burbuja
        for(i = 0; i < cantidad - 1; i++){
            for(j = 0; j < cantidad - i - 1; j++){
                if(ranking[j].rachaMaxima < ranking[j+1].rachaMaxima){
                    temp = ranking[j];
                    ranking[j] = ranking[j+1];
                    ranking[j+1] = temp;
                }
            }
        }
    }
    printf("\n===== RANKING =====\n");
    printf("%-20s %-10s\n","Jugador","Racha");
    for(i=0;i<cantidad;i++){
        printf("%-20s %d\n",ranking[i].nombre,ranking[i].rachaMaxima);
    }
}

void mostrarJugadores(Jugador *lista){
    Jugador *aux;
    aux = lista;
    if(aux == NULL){
        printf("No hay jugadores registrados.\n");
        return;
    }
    printf("\n===== JUGADORES REGISTRADOS =====\n");
    while(aux != NULL){
        printf("Nombre: %s\n", aux->nombre);
        printf("Jugadas: %d\n", aux->jugadas);
        printf("Ganadas: %d\n", aux->ganadas);
        printf("Perdidas: %d\n", aux->perdidas);
        printf("Racha actual: %d\n", aux->rachaActual);
        printf("Racha maxima: %d\n", aux->rachaMaxima);
        printf("------------------------------\n");
        aux = aux->sig;
    }
}

void liberarLista(Jugador **listaJugadores){
    Jugador *aux;
    while(*listaJugadores != NULL){
        aux = (*listaJugadores)->sig;
        free(*listaJugadores);
        *listaJugadores = aux;
    }
}

void mostrarAhorcado(int errores){
    switch(errores){
        case 0:
            printf(" +---+\n");
            printf(" |   |\n");
            printf(" |\n");
            printf(" |\n");
            printf(" |\n");
            printf(" |\n");
            printf("=========\n");
            break;
        case 1:
            printf(" +---+\n");
            printf(" |   |\n");
            printf(" |   O\n");
            printf(" |\n");
            printf(" |\n");
            printf(" |\n");
            printf("=========\n");
            break;
        case 2:
            printf(" +---+\n");
            printf(" |   |\n");
            printf(" |   O\n");
            printf(" |   |\n");
            printf(" |\n");
            printf(" |\n");
            printf("=========\n");
            break;
        case 3:
            printf(" +---+\n");
            printf(" |   |\n");
            printf(" |   O\n");
            printf(" |  /|\n");
            printf(" |\n");
            printf(" |\n");
            printf("=========\n");
            break;
        case 4:
            printf(" +---+\n");
            printf(" |   |\n");
            printf(" |   O\n");
            printf(" |  /|\\\n");
            printf(" |\n");
            printf(" |\n");
            printf("=========\n");
            break;
        case 5:
            printf(" +---+\n");
            printf(" |   |\n");
            printf(" |   O\n");
            printf(" |  /|\\\n");
            printf(" |  /\n");
            printf(" |\n");
            printf("=========\n");
            break;
        case 6:
            printf(" +---+\n");
            printf(" |   |\n");
            printf(" |   O\n");
            printf(" |  /|\\\n");
            printf(" |  / \\\n");
            printf(" |\n");
            printf("=========\n");
            break;
        case 7:
            printf(" +---+\n");
            printf(" |   |\n");
            printf(" |  \\O\n");
            printf(" |  /|\\\n");
            printf(" |  / \\\n");
            printf(" |\n");
            printf("=========\n");
            break;        
        case 8:
            printf(" +---+\n");
            printf(" |   |\n");
            printf(" |  \\O/\n");
            printf(" |  /|\\\n");
            printf(" |  / \\\n");
            printf(" |\n");
            printf("=========\n");
            break;
        }
}

void cargarPalabras(Categoria categorias[]){
    FILE *archivo; //FILE es una funcio de stdio.h para un archivo abierto
    int i; //Declaracion de variable para recorrer las categorias
    char archivos[CANT_CATEGORIAS][20] = 
    {"animales.txt",
    "paises.txt",
    "tecnologia.txt"};

    for(i = 0; i < CANT_CATEGORIAS; i++){
        archivo = fopen(archivos[i],"r"); //fopen, es FILE OPEN y recibe 2 parametros, primero el nombre y despues "r" que significa LECTURA
        if(archivo == NULL){ //fopen devuelve NULL si no encontro el archivo o no lo recibio
            printf("No se pudo abrir %s\n", archivos[i]);
            continue; //El "continue" es para que continue a la siguiente iteracion del for, ya que no me interesa que continue en esta iteracion 
        }
        while(categorias[i].cantidad < MAX_PALABRAS && fscanf(archivo,"%29s",categorias[i].palabras[categorias[i].cantidad]) == 1){ //fscanf lee del archivo con un maximo de 29 posiciones de string porque la ultima(pos 30) es "/0", el fscanf retorna 1 si encuentra algo, sino retorna 0 y deja de añadir cantida de palabras a cantidad de esa categoria o si alcanzo las 100 palabras esa categoria(MAX_PALABRAS)
            categorias[i].cantidad++;
        }
        fclose(archivo); //fclose cierra el archivo que reciba como parametro
    }
}

void cargarJugadores(Jugador **listaJugadores){
    FILE *archivo;
    Jugador *nuevo;
    archivo = fopen("jugadores.txt", "r");
    if (archivo == NULL){
        return;
    }
    char linea[MAX_LINEA];
    while (fgets(linea, sizeof(linea), archivo) != NULL){ //fgets a diferencia de fscanf lee la linea completa y retorna un puntero, sino encuentra nada retorna NULL
        nuevo = (Jugador *)malloc(sizeof(Jugador)); //Reservamos en memoria una cantidad de espacio suficiente para que entre Jugador
        sscanf(linea,"%29s[^;];%d;%d;%d;%d;%d", nuevo->nombre, &nuevo->jugadas, &nuevo->ganadas, &nuevo->perdidas, &nuevo->rachaActual, &nuevo->rachaMaxima);//De la cadena lee en el formato de maximo 29 caracteres mientras no haya ";", luego espera un entero y otro y otro sucesivamente. Le asigna al puntero (porque es un arreglo de caracteres) nombre el nombre y los demás se pasa con "&" para su direccion donde guardarlo
        nuevo->sig = NULL; //El puntero de nuevo apunta a NULL
        if (sscanf(linea, "%29[^;];%d;%d;%d;%d;%d", nuevo->nombre, &nuevo->jugadas, &nuevo->ganadas, &nuevo->perdidas, &nuevo->rachaActual, &nuevo->rachaMaxima) == 6){ //Verificacion que el sscanf retorna 6 campos, ni mas ni menos, evitando asi datos corruptos
            nuevo->sig = *listaJugadores; //El puntero de nuevo apunta a la cabeza de la lista
            *listaJugadores = nuevo; //La cabeza de la lista es igual a nuevo, ahora inserte el nodo Nuevo como cabeza de la lista
        }else{
            free(nuevo); //Libera el espacio en memoria reservado con malloc
        }
            if (nuevo == NULL){ //comprobacion que el malloc no falle
            fclose(archivo);
            return;
        }
    }
    fclose(archivo);
}

void guardarJugadores(Jugador *listaJugadores){
    FILE *archivo;
    Jugador *actual; //actual es un puntero auxiliar para recorrer la lista enlazada y asi no perder la cabeza de la lista
    archivo = fopen("jugadores.txt","w"); //fopen, es FILE OPEN y recibe 2 parametros, primero el nombre y despues "w" que significa ESCRITURA, si el archivo tenia contenido lo borra y reescribe
    if(archivo == NULL){ //Si no pudo abrir el archivo retorna null y retorna
        return;
    }
    actual = *listaJugadores; //Actual es igual a la cabeza de la lista
    while(actual != NULL){
        fprintf(archivo, "%s;%d;%d;%d;%d;%d\n", actual->nombre, actual->jugadas, actual->ganadas, actual->perdidas, actual->rachaActual, actual->rachaMaxima); //La funcion fprinf de stdio.h es el equivalente al printf pero escribiendo en un archivo
        actual = actual->sig; //Avanza al nodo siguiente
    }
    fclose(archivo);
}

Jugador *elegirJugador(Jugador **listaJugadores){ //Esto es una funcion porque retorna un puntero a Jugador
    char nombre[MAX_NOMBRE];
    Jugador *actual; //Puntero auxiliar
    Jugador *nuevo; //Puntero para añadir un jugador en caso que no exista
    printf("Ingrese su nombre: ");
    fgets(nombre, MAX_NOMBRE, stdin); //Se usa fgets en lugar del scanf proque el scanf corta la lectura en el espacio, si el jugador se llama "Juan Pablo" no lo leera
    nombre[strcspn(nombre,"\n")] = '\0'; //El fgets guarda el espacio final de salto de linea "\n", entonces con strcspn cuenta la cantidad de caracteres en el primer parametro hasta que haya un caracter igual al segundo parametro "\n" y corta ahi, entonces tras encontrar el salto de linea corta el nombre y queda nombre[9]='\0' tras 9 caracteres termina los caracteres
    actual = *listaJugadores; //Actual es igual a la cabeza de la lista
    while(actual != NULL){
        if(strcmp(actual->nombre,nombre)==0){ //el srtcmp retorna 0 si son iguales u otro numero si NO son iguales
            return actual; //Si lo encuentra retorna el jugador
        }
        actual = actual->sig; //Avanza al nodo siguiente si aun no lo encontro
    }
    nuevo = (Jugador *)malloc(sizeof(Jugador)); //Reservamos espacio en memoria para este jugador
    if(nuevo == NULL){ //Verificamos que haya espacio para este jugador
        return NULL;
    }
    strcpy(nuevo->nombre,nombre); //Usamos strcpy para poder recorrer y asignar el nombre ya que no podemos hacer simplemente "nuevo->nombre = nombre;"
    nuevo->jugadas = 0;
    nuevo->ganadas = 0;
    nuevo->perdidas = 0;
    nuevo->rachaActual = 0;
    nuevo->rachaMaxima = 0;
    nuevo->sig = *listaJugadores; //Insertamos al principio de la lista
    *listaJugadores = nuevo; //La cabeza de la lista ahora es igual a "nuevo"
    return nuevo;
}

int elegirCategoria(Categoria categorias[]){
    int opcion;
    int i;
    printf("\n===== CATEGORIAS =====\n");
    for(i = 0; i < CANT_CATEGORIAS; i++){
        printf("%d. %s\n", i + 1, categorias[i].nombre);
    }
    do{
        printf("Seleccione una categoria: ");
        scanf("%d", &opcion);
        limpiarBuffer();
        if(opcion < 1 || opcion > 3){
            printf("Categoria invalida.\n");
        }
    }while(opcion < 1 || opcion > 3);
    return opcion - 1;
}

int elegirDificultad(){
    int opcion;
    printf("\n===== DIFICULTAD =====\n");
    printf("1. Facil\n");
    printf("2. Media\n");
    printf("3. Dificil\n");
    do{
        printf("Seleccione una dificultad: ");
        scanf("%d",&opcion);
        limpiarBuffer();
        if(opcion < 1 || opcion > 3){
            printf("Dificultad invalida.\n");
        }
    }while(opcion < 1 || opcion > 3);
    return opcion;
}

char *seleccionarPalabra(Categoria categorias[], int categoria, int dificultad){
    int longitudMinima;
    int i;
    int posiciones[MAX_PALABRAS];
    int cantidad = 0;
    int elegida;
    switch(dificultad){
        case 1:
            longitudMinima = 4;
            break;
        case 2:
            longitudMinima = 6;
            break;
        case 3:
            longitudMinima = 8;
            break;
    }
    for(i = 0; i < categorias[categoria].cantidad; i++){
        if(strlen(categorias[categoria].palabras[i]) >= longitudMinima){ //srtlen obtiene la cantidad de caracteres y compara la palabra en la posocon i con nombre categoria del arreglo de categorias con la longitudMinina
            posiciones[cantidad] = i; //Posiciones agrega la posicion == i porque la palabra cumple el minimo de la dificultad
            cantidad++; //Incrementamos cantidad porque encontramos una palabra
        }
    }
    if(cantidad == 0){ //Verificacion en caso que no encontremos ninguna palabra que cumpla la cantidad
        return NULL;
    }
    elegida = rand() % cantidad; //Elige un numero random de 0 a cantidad - 1 (esto pasa por el %), por ejemplo si cantidad es 4 elige de manera random de 0 a 3
    return categorias[categoria].palabras[posiciones[elegida]]; //Retorna la palabra de "categoria" segun la lista "posiciones" en la posicion "elegida"
}

void jugar(Jugador *jugador, Categoria categorias[], int dificultad){
    char *palabra; //Aca guardamos la palabra, pero no la copia, solo apunta a ella
    char oculta[MAX_PALABRA]; //Esta variable es la que ve el usuario
    char letra; //La letra que ingresara el usuario
    char letrasUsadas[MAX_LETRAS]; //Aca guardamos las letras usadas
    int cantidadLetras = 0; //Cuenta la cantidad de letras que fue ingresando el usuario, siempre inicia en 0
    int errores = 0; //Siempre se inicia los errores en 0 para ir aumentando con un tope segun la dificultad
    int erroresMaximos; //El valor dependera de la dificultad
    int categoria; //dentro del juego preguntamos la categoria
    int i; //Variable para recorrer los arreglos
    int longitud; //Variable en la que guardaremos la cantidad de caracteres de la palabra
    int acierto; //Variable flag, si es parte de la palabra =1 si no =0
    int repetida; //Variable flag, si encuentra la letra =1 si no la encuentra =0

    categoria = elegirCategoria(categorias); //Ahora sabremos donde buscar la palabra
    palabra = seleccionarPalabra(categorias, categoria, dificultad); //Ahora elegimos la palabra
    if(palabra == NULL){ //Verificacion que la palabra exista 
        printf("No hay palabras para esa dificultad.\n");
        return;
    }
    longitud = strlen(palabra);
    switch(dificultad){
        case 1:
            erroresMaximos = 8;
            break;
        case 2:
            erroresMaximos = 6;
            break;
        case 3:
            erroresMaximos = 4;
            break;
    }
    for(i = 0; i < longitud; i++){
        oculta[i] = '_'; //Guardara en la variable _ por cada caracter que tenga la palabra
    }
    oculta[longitud] = '\0'; //Cerramos la cadena al final para evitar que despues el printf("%s") no se quede leyendo infinitamente porque no encuentra el final de string (que es el \0)

    while(errores < erroresMaximos && strcmp(palabra, oculta) != 0){
        printf("\n");
        mostrarAhorcado(errores);
        printf("Errores: %d/%d\n", errores, erroresMaximos);
        printf("Palabra: ");
        for(i = 0; i < longitud; i++){ //Este FOR es para mostrar "_ _ _ _" en lugar de "____"
            printf("%c ", oculta[i]); //Imprimimos el caracter en la posicion i y hacemos un espacio
        }
        printf("\n");
        
        printf("Letras usadas: ");
        for(i = 0; i < cantidadLetras; i++){
            printf("%c ", letrasUsadas[i]);
        }
        printf("\n");
        
        printf("Ingrese una letra: ");
        scanf(" %c",&letra); //El espacio delante del %c es para que scanf ignore cualquier espacio que haya quedado en el buffer antes de leer el caracter
        limpiarBuffer();
        
        //Seccion para controlar letras ya ingresadas y avisar al jugador si repite una letra
        repetida = 0; //Se pone en 0 porque por cada letra ingresada suponemos que no esta repetida
        for(i = 0; i < cantidadLetras; i++){
            if(letrasUsadas[i] == letra){
                repetida = 1;
                break;//Este break corta las iteracones del for
            }
        }
        if(repetida){
            printf("Esa letra ya fue ingresada.\n");
            continue; //Este continue es para regresar al principio del while ignorando todo lo que siga
        }

        letrasUsadas[cantidadLetras] = letra;
        cantidadLetras++;

        //Seccion para ver si encontramos un acierto
        acierto = 0;
        for(i = 0; i < longitud; i++){
            if(palabra[i] == letra){
                oculta[i] = letra;
                acierto = 1;
            }
        }
        if(acierto == 0){
            errores++;
            printf("La letra no esta en la palabra.\n");
        }else{
            printf("Correcto!\n");
        }
    }
    //Mostramos como va el dibujo
    mostrarAhorcado(errores);
    //Caso de fin del while por victoria
    if(strcmp(palabra, oculta) == 0){
        printf("\n¡Felicitaciones!\n");
        printf("Adivinaste la palabra: %s\n", palabra);
        jugador->jugadas++;
        jugador->ganadas++;
        jugador->rachaActual++;
        if(jugador->rachaActual > jugador->rachaMaxima){
            jugador->rachaMaxima = jugador->rachaActual;
        }
    }else{
        printf("\nPerdiste.\n");
        printf("La palabra era: %s\n", palabra);
        jugador->jugadas++;
        jugador->perdidas++;
        jugador->rachaActual = 0;
    }
}