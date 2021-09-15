#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MaxBombas 1//el tope de las bombas en la dificultad maximo
#define Maximo 10//el tope de la matriz en la dificultad maximo
#define Medio 8//el tope de la matriz en la dificultad facil
#define MedioBombas 1//el tope de las bombas en la dificultad medio
#define Minimo 6//el tope de la matriz y las bombas en la dificultad facil

#define cantJugadores 20//el límite de la cantidad de jugadores
#define cantNombre 20//el límite de la cantidad de caracteres para el nombre
using namespace std;
//VARIABLES GLOBALES
int Dificultad;
int cantbomb;
int partidasTotales;

//ESTRUCTURAS:
struct partida{
    int intentos;
    int puntaje;
    char Dific;
};
struct partidaTop10{
    int intentos;
    int puntaje;
    char Dific;
    char nickname[cantNombre];
};
struct jugador{
    char nombre[cantNombre+1];
    char nickname[cantNombre];
    int edad;
    int CI;
    int cantintentos;
    int partidasjugadas;
    partida ultimas5[5];
};
struct listaJugadores{
    jugador lista[cantJugadores];
    bool ocupado[cantJugadores];
    int cantusuarios;

};

//FUNCIONES DE LA INTERACCION CON EL USUARIO:
void difJuego();
void registrarse(listaJugadores &x,bool salir);
void logIn(listaJugadores x,int &contexploradas,int &lugaruser,partidaTop10 losMejores[]);
void leerNombre(char pal[],int largo);
bool compararArreglos (char a[],char b[],int largoactual);
jugador crearJugador(listaJugadores x);
void menuUsuario(listaJugadores &x,int &contexploradas,int &lugaruser,partidaTop10 losMejores[]);//menú que se muestra cuando el usuario inicia sesion
void mainTrucho(int &contexploradas,int &lugaruser,partidaTop10 losMejores[]);
void funcionTop10(listaJugadores x,partidaTop10 losMejores[],int lugaruser, int contexploradas);
void ordenarArregloTop10(partidaTop10 losMejores[], int tope);
void igualarStructTop10(listaJugadores x,partidaTop10 losMejores[], int contexploradas, int posicion, int lugaruser);
void leerLineaRec (char arr[], int largo, int lugarActual);


//FUNCIONES DEL BUSCAMINAS:
void imprimirMatriz (char matriz[][Maximo]);//Primera matriz
void randomizarBombas(char matriz[][Maximo], int fil, int col);//Busca por primera vez
int numerosporbomba (char matriz[][Maximo],int xfila,int ycolumna);//Randomiza las bombas
void fExplorar1vez (char matriz[][Maximo],char matrizbombas[][Maximo], int filusuario, int colusuario);
void explorar(char matriz[][Maximo],char matrizbombas[][Maximo], int filusuario, int colusuario);
void fMarcar (char matriz [][Maximo],int fil,int col);
void inicializarMatriz(char matriz[][Maximo]);
void explorar(char matriz[][Maximo],char matrizbombas[][Maximo], int filusuario, int colusuario);
void fBuscar(char matriz[][Maximo],char matrizbombas[][Maximo], int filusuario, int colusuario,bool &perdiste);
void imprimirSiperdiste(char matriz[][Maximo], char matrizbombas[][Maximo]);
bool fparaganar(char matriz[][Maximo],int &contexploradas);
void jugar(listaJugadores &x,int &partidasjugadas,int &contexploradas,int &lugaruser);
void compararUltimas5(jugador &x, partida y);
void ultimas5(listaJugadores &x,int &contexploradas,int &lugaruser);
void imprimirUltimas5(listaJugadores &x,int &lugaruser);

int main(){
	srand(time(NULL));
	int contexploradas;
	int lugaruser;
    listaJugadores x;
    partidasTotales=0;
    partidaTop10 losMejores[10];
	mainTrucho(contexploradas,lugaruser,losMejores);
	return 0;
}

void mainTrucho(int &contexploradas,int &lugaruser,partidaTop10 losMejores[]){
    bool salir=false;
	char opcion;
	listaJugadores x;
	x.cantusuarios=0;
	for(int i=0;i<cantJugadores;i++)
        x.ocupado[i]=false;
    do{
    	printf("\t\tMENU INICIO\n\n");
    	printf("1- Iniciar Sesion.\n");
    	printf("2- Registrarse.\n");
    	printf("3- Salir\n");
    	opcion=getchar();
    	getchar();
    	switch(opcion){
    		case '1':
                logIn(x,contexploradas,lugaruser,losMejores);
                menuUsuario(x,contexploradas,lugaruser,losMejores);
    		break;
    		case '2':
    			registrarse(x,salir);
    			//getchar();
            break;
    		case '3':
    			salir=true;
    		break;
    	}
    }while (!salir);
}

void jugar(listaJugadores &x,int &contexploradas,int &lugaruser,partidaTop10 losMejores[]){
    printf("\t\t¡¡¡BUSCAMINAS!!!\n\n");
    int fil,col;
    int i;
    bool ganaste=false,esPrimeraVez=true,perdiste=false;
    char matriz[Maximo][Maximo],matrizbombas[Maximo][Maximo],cfil,ccol;
    char jugada;
    printf("- Presione la tecla E seguido de la coordenada para explorar la casilla deseada.\n");
    printf("- Presione la tecla M seguido de la coordenada para marcar donde usted cree que hay una bomba.\n");
    printf("- Presione la tecla B seguido de la coordenada para buscar donde usted ya marcó donde\n   están las banderas en dicho lugar.\n\n");
    printf("Nota: \nUsted solo podrá explorar una casilla que no esté con bandera.\n");
    printf("      \nUsted solo podrá buscar en una casilla cuando el número de banderas\n         alrededor de la casilla sea equivalente al número de casillas.\n\n");
    printf("Ganaste si descubriste todas las casillas vacías del \ntablero sin tocar las de las bombas.\n\n ¡¡¡SUERTE!!!, bah la suerte es para los débiles\n\n\n");
    difJuego();
	inicializarMatriz(matriz);
    imprimirMatriz(matriz);
    do{
        jugada=getchar();
        getchar();
        cfil=getchar();
        ccol=getchar();
        fil=cfil-'A';
        col=ccol-'A';
        switch (jugada){
            case 'E': case 'e':
                if(esPrimeraVez and matriz[fil][col]!='X'){
                	fExplorar1vez (matriz,matrizbombas,fil,col);//CARGA LA MATRIZ
                	esPrimeraVez=false;
				}
				explorar(matriz,matrizbombas,fil,col);
                if(matrizbombas[fil][col]=='*'){
                    perdiste=true;
                }
            break;
            case 'M': case 'm':
            	fMarcar(matriz,fil,col);
            break;
            case 'B': case 'b':
            	if (!esPrimeraVez){
                    if(matriz[fil][col]!='?' or matriz[fil][col]!='M'){
                        fBuscar(matriz,matrizbombas,fil,col,perdiste);
                    }
            	}
            break;
            default:
            	printf("Jugada Invalida\n");
            break;

        }
		if (!ganaste){
            imprimirMatriz(matriz);
		}
        getchar();
        ganaste=fparaganar(matriz,contexploradas);
	}while (!ganaste and !perdiste);
	if (perdiste==true){
        printf("\n¡¡¡PERDISTE!!!\n");
        printf("\nAHI HABIA UNA BOMBA\n\n");
        imprimirSiperdiste(matriz,matrizbombas);
        ultimas5(x,contexploradas,lugaruser);
        partidasTotales++;
	}
	if (ganaste==true){
        printf("¡¡¡GANASTE!!!\n\n");
        ultimas5(x,contexploradas,lugaruser);
        partidasTotales++;
    }
    funcionTop10(x,losMejores,lugaruser,contexploradas);
}

void imprimirMatriz (char matriz[][Maximo]){//Primera Matriz
    int fila,cola,filh;
    printf (" ");
    for (filh=0;filh<Dificultad;filh++){
        printf (" %c",filh+'A');
    }
    printf("\n");

    for (fila=0;fila<Dificultad;fila++){
        printf("%c ",fila+'A');
        for (cola=0;cola<Dificultad;cola++){
            printf("%c ",matriz[fila][cola]);
        }
        printf("\n");
    }
}

void inicializarMatriz(char matriz[][Maximo]){
	int fil,col;
	for (fil=0;fil<Dificultad;fil++){
		for(col=0;col<Dificultad;col++){
			matriz[fil][col]='?';
		}
	}
}

void fExplorar1vez (char matriz[][Maximo],char matrizbombas[][Maximo], int filusuario, int colusuario){//Explora por primera vez
        inicializarMatriz(matrizbombas);
        randomizarBombas(matrizbombas,filusuario,colusuario);
}

void explorar(char matriz[][Maximo],char matrizbombas[][Maximo], int filusuario, int colusuario){
//ESTO EXPLORA NORMAL
	int fil,col;
	int resultado,resultadoady;
	if (matriz[filusuario][colusuario]!='X'){
        resultado=numerosporbomba(matrizbombas,filusuario,colusuario);
        matriz[filusuario][colusuario]=resultado+'0';
        if(resultado==0){
            for(fil=filusuario-1;fil<=filusuario+1;fil++){
                for(col=colusuario-1;col<=colusuario+1;col++){
                    if (col>=0 and fil>=0 and col<Dificultad and fil<Dificultad){
                        resultadoady=numerosporbomba (matrizbombas,fil,col);
                        if (resultadoady==0 and matriz[fil][col]=='?'){
                                explorar(matriz,matrizbombas,fil,col);
                        }
                        matriz[fil][col]=resultadoady+'0';
                    }

                }
            }
        }

    }
    else
        printf("No se puede explorar en una casilla marcada...\n\n");
}

void randomizarBombas(char matrizbombas[][Maximo], int fil, int col){//Randomiza las bombas...
    int cantbombas=0, xfila, ycolumna;
    while(cantbombas<cantbomb){
        xfila=rand()%Dificultad;
        ycolumna=rand()%Dificultad;
        if ((matrizbombas[xfila][ycolumna]!='*')and(xfila!=fil or ycolumna!=col)){
            matrizbombas[xfila][ycolumna]='*';
            cantbombas++;
        }
    }
}

//Me devuelve la cantidad de bombas adyacentes a un lugar
int numerosporbomba (char matrizbombas[][Maximo],int  xfila, int ycol){//Le adjudica el número correspondiente a cada casilla
    int contbombas=0,fil,col;
    for(fil=xfila-1;fil<=xfila+1;fil++){
        for (col=ycol-1;col<=ycol+1;col++){
            if (col>=0 and fil>=0 and col<Dificultad and fil<Dificultad){
                if (matrizbombas[fil][col]=='*'){
                    contbombas++;
                }
            }
        }

    }
    return contbombas;
}


void fMarcar (char matriz [][Maximo],int filusuario,int colusuario){//Marca la matriz en la posicion deseada
    int contmarcar=0;
    if(matriz[filusuario][colusuario]=='?'){
        matriz[filusuario][colusuario]='X';
        contmarcar++;
    }
    else if (matriz[filusuario][colusuario]=='X'){
        matriz[filusuario][colusuario]='?';
        contmarcar--;
    }

    if(contmarcar==16){
		printf("¡¡¡GANASTE!!!\n\n");
	}

}


void fBuscar(char matriz[][Maximo],char matrizbombas[][Maximo], int filusuario, int colusuario,bool &perdiste){
    int resultado,contmarcadas=0,fil,col;
    for (fil=filusuario-1;fil<=filusuario+1;fil++){
        for (col=colusuario-1;col<=colusuario+1;col++){
            if (col>=0 and fil>=0 and col<Dificultad and fil<Dificultad){
                if (matriz[fil][col]=='X'){
                    contmarcadas++;
                }
            }
        }
    }
    resultado=numerosporbomba(matrizbombas,filusuario,colusuario);
    if (resultado==contmarcadas){
        for (fil=filusuario-1;fil<=filusuario+1;fil++){
            for (col=colusuario-1;col<=colusuario+1;col++){
                if (col>=0 and fil>=0 and col<Dificultad and fil<Dificultad){
                    if (matriz[fil][col]!='X'){
                        explorar(matriz,matrizbombas,fil,col);
                    }
                    if(matrizbombas[fil][col]=='*'){
                        if (matriz[fil][col]!='X')
                            perdiste=true;
                    }
                }
            }
        }
    }
    else
        printf("No marcaste las casillas suficientes...\n\n");
}

void imprimirSiperdiste(char matriz[][Maximo], char matrizbombas[][Maximo]){
    int fil, col, filh;
    printf (" ");
    for (filh=0;filh<Dificultad;filh++){
        printf (" %c",filh+'A');
    }
    printf("\n");

    for (fil=0;fil<Dificultad;fil++){
        printf("%c ",fil+'A');
        for (col=0;col<Dificultad;col++){
            if (matrizbombas[fil][col]=='*')
                printf("%c ",matrizbombas[fil][col]);
            else
                printf("%c ",matriz[fil][col]);
        }
        printf("\n");
    }
}

bool fparaganar(char matriz[][Maximo],int &contexploradas){
    bool ganaste;
    int fil, col;
    contexploradas=0;
    for(fil=0;fil<Dificultad;fil++){
        for(col=0;col<Dificultad;col++){
            if (matriz[fil][col]!='X'){
                if (matriz[fil][col]!='?')
                    contexploradas++;
            }
        }
    }
    if (contexploradas==(Dificultad*Dificultad)-cantbomb)
        ganaste=true;
    printf("Puntos :%d\n",contexploradas);//Cuenta los puntos... Poner el puntaje en la cant. de puntajes del jugador
    return ganaste;
}

void difJuego(){
    printf("(F)acil: tablero de 6x6 con 6 minas\n");
    printf("(I)ntermedio: tablero de 8x8 con 10 minas\n");
    printf("(A)vanzado: tablero de 10x10 con 20 minas\n");
    char opcion=getchar();
    switch(opcion){
        case 'F':case 'f':
            Dificultad=Minimo;
            cantbomb=Minimo;
        break;
        case 'I': case 'i':
            Dificultad=Medio;
            cantbomb=MedioBombas;
        break;
        case 'A':case 'a':
            Dificultad=Maximo;
            cantbomb=MaxBombas;
        break;
        default:
            printf("Comando Erroneo (¡¡¡Banana!!!)");
        break;

    }
    getchar();

}

jugador cargarJugador(listaJugadores x){
    jugador ret;
    bool ciIguales,nickIguales=false,salir;
    int i;
    do{
        ciIguales=false;
        printf("\nIngrese la cedula: ");
        scanf("%d",&ret.CI);
        getchar();
        for (i=0;i<x.cantusuarios;i++){
            if(x.lista[i].CI==ret.CI){
                printf("Alguno de estos datos ya están usados, pancho!!!\n");
                printf("Ingrese cedula nuevamente\n");
                ciIguales=true;
            }
        }
    }while(ciIguales);
    do{
        i=0;
        salir=false;
        printf("\nIngrese nickname: ");
        scanf("%s",&ret.nickname);
        getchar();
        while(i<cantJugadores && !salir){
            nickIguales=compararArreglos(ret.nickname,x.lista[i].nickname,0);
            if(nickIguales){
                salir=true;
                printf("Ingrese otro nickname ese ya esta en uso.\n");
            }
            i++;
        }
    }while(nickIguales);
    printf("\nEdad: ");
    scanf("%d",&ret.edad);
    getchar();
    printf("\nIngrese el nombre: ");
    leerLineaRec (ret.nombre,cantNombre,0);
    return ret;
}

void registrarse(listaJugadores &x,bool salir){
    int i=0;
    bool encontre=false;
    if(x.cantusuarios<cantJugadores){
        while(i<cantJugadores and !encontre){
            if(x.ocupado[i]==false){
                x.lista[i]=cargarJugador(x);
                x.ocupado[i]=true;
                x.lista[i].cantintentos=1;
    			x.lista[i].partidasjugadas=0;
                x.cantusuarios++;
                encontre=true;
                salir=true;
            }
            else{
                i++;
            }
        }
    }
    else{
        printf("Esta todo lleno.\n");
    }
}

void menuUsuario(listaJugadores &x,int &contexploradas,int &lugaruser,partidaTop10 losMejores[]){//menú que se muestra cuando el usuario inicia sesion
    char opcion;
    int i;
    bool cerrarSesion=false;
    do{
        printf("A- Jugar.\n");
        printf("B- Ver Perfil.\n");
        printf("C- Ver ultimas 5 personales.\n");
        printf("D- Ver TOP 10\n");
        printf("E- Cerrar Sesion.\n");
        opcion=getchar();
        getchar();
        switch(opcion){
            case 'A': case 'a'://juega al buscaminas desde el perfil
                jugar(x,contexploradas,lugaruser,losMejores);
            break;
            case 'B': case 'b':
                printf("Nombre: %s\n",x.lista[lugaruser].nombre);
                printf("Nickname: %s\n",x.lista[lugaruser].nickname);
                printf("Cedula: %d\n",x.lista[lugaruser].CI);
                printf("Edad: %d\n\n",x.lista[lugaruser].edad);
            break;
            case 'C': case 'c':
                imprimirUltimas5(x,lugaruser);
            break;
            case 'D': case 'd':
                i=0;
                if (partidasTotales==0){
                    printf("No se han jugado partidas...\n");
                }
                else{
                    while(i<partidasTotales and i<10){
                        printf("Puntos: %d Intentos: %d Dif: %c Nickname: %s\n",losMejores[i].puntaje,losMejores[i].intentos,losMejores[i].Dific,losMejores[i].nickname);
                        i++;
                    }
                }
            break;
            case 'E': case 'e':
                cerrarSesion=true;
            break;
            default:
                printf("Comando Erroneo\n:");
            break;
        }

    }while (!cerrarSesion);


}

void logIn(listaJugadores x,int &contexploradas,int &lugaruser,partidaTop10 losMejores[]){
    char arr[cantNombre];
    bool sonIguales=false;
    lugaruser=0;
    printf("Ingrese su nickname: ");
    scanf("%s",arr);
    getchar();
    while (!sonIguales and lugaruser<cantJugadores){
        sonIguales=compararArreglos(arr,x.lista[lugaruser].nickname,0);
        lugaruser++;
    }
    lugaruser--;
    if (sonIguales){
        printf("\n\tBienvenido %s\n\n",x.lista[lugaruser].nombre);
    }
    else{
        printf("Este nickname no está registrado.\n");
        mainTrucho(contexploradas,lugaruser,losMejores);
    }
}

bool compararArreglos (char arrScan[],char arrLista[],int largoactual){
    bool nickIguales=false;
    if((arrScan[largoactual]!='\0') and (arrScan[largoactual]==arrLista[largoactual])){
        nickIguales=compararArreglos(arrScan,arrLista,largoactual+1);
    }
    else if(arrScan[largoactual]=='\0' and arrLista[largoactual]=='\0')
        nickIguales=true;
    return nickIguales;
}


void ultimas5(listaJugadores &x,int &contexploradas,int &lugaruser){
	int lugar;
	lugar=x.lista[lugaruser].partidasjugadas%5;
	x.lista[lugaruser].ultimas5[lugar].puntaje=contexploradas;
    if(Dificultad==Minimo){
        x.lista[lugaruser].ultimas5[lugar].Dific='F';
    }else if(Dificultad==Medio){
        x.lista[lugaruser].ultimas5[lugar].Dific='I';
    }else{
        x.lista[lugaruser].ultimas5[lugar].Dific='A';
    }
    x.lista[lugaruser].ultimas5[lugar].intentos=x.lista[lugaruser].cantintentos;
    x.lista[lugaruser].cantintentos++;
    x.lista[lugaruser].partidasjugadas++;
}

void imprimirUltimas5(listaJugadores &x,int &lugaruser){

    if(x.lista[lugaruser].partidasjugadas==0)
        printf("No hay partidas jugadas\n");
    if(x.lista[lugaruser].partidasjugadas<5){
        for (int i=0;i<x.lista[lugaruser].partidasjugadas;i++){
            printf("Intentos: %d  Puntaje: %d Dif: %c\n",x.lista[lugaruser].ultimas5[i].intentos,x.lista[lugaruser].ultimas5[i].puntaje,x.lista[lugaruser].ultimas5[i].Dific);
        }
    }else if(x.lista[lugaruser].partidasjugadas>=5){
        for (int i=0;i<5;i++){
            printf("Intentos: %d  Puntaje: %d Dif: %c\n",x.lista[lugaruser].ultimas5[i].intentos,x.lista[lugaruser].ultimas5[i].puntaje,x.lista[lugaruser].ultimas5[i].Dific);
        }

    }
    printf("\n");

}

void funcionTop10(listaJugadores x,partidaTop10 losMejores[],int lugaruser, int contexploradas){//LE TENEMOS QUE PASAR LA LISTA DE LOS JUGADORES CON EL LUGAR, EL ARREGLO DE TOP 10 Y LA PARTIDA CON LOS PUNTAJES
    //CASO DE QUE SEA LA PRIMERA PARTIDA
    int aux=0;
    if (partidasTotales==1){
        igualarStructTop10(x,losMejores,contexploradas,0,lugaruser);
    }

    //CASO DE QUE SE HALLAN JUGADO MENOS DE 10 PARTIDAS (POR LA VARIABLE GLOBAL PARTIDASTOTALES)
    else if (partidasTotales>=0 and partidasTotales<=9){
        igualarStructTop10(x,losMejores,contexploradas,partidasTotales-1,lugaruser);
        ordenarArregloTop10(losMejores,partidasTotales-1);
    }
    //SE HAN JUGADO MAS DE 10 PARTIDAS, ENTONCES SOLO SE AGREGAN SI EL PUNTAJE ES MEJOR QUE EL MENOR DEL TOP10
    else if (contexploradas>losMejores[0].puntaje){
        igualarStructTop10(x,losMejores,contexploradas,0,lugaruser);
        ordenarArregloTop10(losMejores,10);
    }
}

void ordenarArregloTop10(partidaTop10 losMejores[], int tope){
    int x, aux=0;
    partidaTop10 y;
    bool cambie=false;
    for (x=0;x<tope;x++){
        if (losMejores[x].puntaje>losMejores[x+1].puntaje){//si el arr en la posicion x es mas grande q en la posicion x+1 cambialos
            //iguala en y los valores de x
            y=losMejores[x];
            //iguala n x los valoxrees d+1e
            losMejores[x]=losMejores[x+1];
            //iguala en x+1 los valores de x (de y en este caso)
            losMejores[x+1]=y;
            cambie=true;
        }
    }
    if(tope>=0 and cambie)
        ordenarArregloTop10(losMejores,tope-1);
}
void igualarStructTop10(listaJugadores x,partidaTop10 losMejores[], int contexploradas, int posicion, int lugaruser){
    int aux=0;
    losMejores[posicion].puntaje=contexploradas;
    losMejores[posicion].intentos=x.lista[lugaruser].cantintentos-1;
    if (Dificultad==Minimo)
        losMejores[posicion].Dific='F';
    else if (Dificultad==Medio)
        losMejores[posicion].Dific='I';
    else if (Dificultad==Maximo)
        losMejores[posicion].Dific='A';
    /*while (aux<cantNombre and x.lista[lugaruser].nickname[aux]!='\0'){
        losMejores[posicion].nickname[aux]=x.lista[lugaruser].nickname[aux];
        aux++;
    }*/
    strcpy(losMejores[posicion].nickname,x.lista[lugaruser].nickname);
}

void leerLineaRec (char arr[], int largo, int lugarActual){
    char leer=getchar();
    if(lugarActual<largo-1){
        if(leer!='\n'){
            arr[lugarActual]=leer;
            leerLineaRec(arr,largo,lugarActual+1);
        }
        else
            arr[lugarActual]='\0';
    }
    else{
        if(lugarActual==largo-1)
            arr[lugarActual]='\0';
        if(leer!='\n')//No encontre el enter independientemente de que era el final del arreglo
            leerLineaRec(arr,largo,lugarActual+1);

    }
}
