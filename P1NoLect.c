/*
Programa que encuentra los caminos a la meta de un laberinto usando una búsqueda por árbol DFS.
Hecho por Castillo Montes Alan y Vargas Castro Daniel 
para la clase de Algoritmos y Estructuras de Datos de la FI de la UNAM en marzo de 2016.
*/
#include <stdio.h>
#include <stdlib.h>
//DECLARACIÓN DE VARIABLES GLOBALES
typedef struct puntosVisitados 
{ //Estructura que contendrá las coordenadas de los puntos visitados
	int coordX;
	int coordY;
} PuntoVisitado;
int indiceVisitado = 0; //Variable que recorrerá los índices del arreglo de puntos visitados

typedef struct puntosCamino 
{ //Estructura que contendrá las coordenadas del camino a la meta
	int coordX;
	int coordY;
} PuntoActual;
int indicePActual = 0; 	//Variable que recorrerá los índices del arreglo con el camino a la meta

FILE *archivoLaberinto = NULL;
FILE *archivoCaminos = NULL;
PuntoActual *camino = NULL;		//Apuntador a estructura con coordenadas del camino
PuntoVisitado *visitados = NULL;//Apuntador a estructura con coordenadas de puntos visitados
char **laberintoEnMatriz = NULL;//Apuntador de apuntadores donde se almacenará el laberinto leído del archivo
int renglones = 0; 				//Para manejar la matriz con el laberinto
int columnas = 0; 				//Para manejar la matriz con el laberinto
char posicionEnArchivo = 0; 	//Variable que recorrá caracter por caracter el archivo durante su lectura
int i = 0, j = 0; 				//Contadores para usus múltiples
int todoRevisado = 0;			//Variable que indicará si ya se tomaron todos los caminos posibles
int totalDePuntos = 0;			//Variable que contará la cantidad de puntos que no son pared del laberinto
int NORTEVisitado = 0;
int ESTEVisitado = 0;
int SURVisitado = 0;
int OESTEVisitado = 0;

//PROTOTIPOS DE FUNCIONES
void caminoNORTE();
void caminoESTE();
void caminoSUR();
void caminoOESTE();
void agregarVisitado();

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
       	printf ("Por favor, ingrese el nombre del archivo.");
       	exit (0);
    }
    else if (argc < 4)
    {
  	 	printf ("Ingrese las coordenadas correctamente.");
      	exit (0);
    }
    if (argc > 5)
    {
        printf("Se pasaron demasiados argumentos.");
        exit (0);
    }
 
//SE CARGA EL ARCHIVO CON EL LABERINTO AL PROGRAMA

	archivoLaberinto = fopen(argv[1], "r"); //El nombre del archivo se almacena como el argumento 1 pasado por consola
	if (archivoLaberinto == NULL)
    {
		printf("\n\tNo se pudo cargar el archivo. El programa se cerrara\n");
		exit(0);
	}

//CUENTA RENGLONES Y COLUMNAS DEL ARCHIVO CON EL LABERINTO PARA DESPUÉS ASIGNAR UNA MATRIZ DINÁMICA DE ESE TAMAÑO
	posicionEnArchivo = fgetc(archivoLaberinto);
	while (feof(archivoLaberinto) == 0)
	{
		if (posicionEnArchivo == '0' || posicionEnArchivo == '1')
			columnas ++;
		else if (posicionEnArchivo == '\n')
		{
			renglones ++;
			columnas = 0;
		}
		posicionEnArchivo = fgetc(archivoLaberinto);
	}
	renglones ++; //Para compensar por el renglón que no lee por el salto de línea
	rewind(archivoLaberinto);

//CREACIÓN DE MATRIZ DINÁMICA CON LAS DIMENSIONES OBTENIDAS ANTERIORMENTE
	laberintoEnMatriz = (char**)malloc(renglones*sizeof(char*));
	if(laberintoEnMatriz == NULL)
    { 
    	printf("\n\tNo se pudo reservar la memoria. El programa se cerrara\n");
    	exit(0);
    }
	for (i = 0; i < renglones; i++)
	{
    	laberintoEnMatriz[i] = (char*)malloc(columnas*sizeof(char));
    	if (laberintoEnMatriz[i] == NULL)
		{
			printf("\n\tNo se pudo reservar la memoria. El programa se cerrara\n");
	  		exit(0);
  		}
	}
	renglones = 0;
	columnas = 0;

//COPIA DE LABERINTO A MATRIZ
    while (feof(archivoLaberinto) == 0)
	{
		posicionEnArchivo = fgetc(archivoLaberinto);
		if (posicionEnArchivo == '0' || posicionEnArchivo == '1')
		{
			laberintoEnMatriz[renglones][columnas] = posicionEnArchivo;
			columnas ++;
			if (posicionEnArchivo == '0')
				totalDePuntos ++;
		}
		else if (posicionEnArchivo == '\n')
		{
			renglones ++;
			columnas = 0;
		}
		else if (posicionEnArchivo == -1)
			renglones ++;
	} 
	fclose(archivoLaberinto);

//CREACIÓN DE ARREGLOS QUE CONTENDRÁN EL CAMINO Y LOS PUNTOS VISITADOS
	visitados = (PuntoVisitado*) malloc((indiceVisitado+1)*sizeof(PuntoVisitado));
	if (visitados == NULL)
    {
		printf("\n\tNo se pudo reservar la memoria. El programa se cerrara\n");
		exit(0);
	}
	camino = (PuntoActual*) malloc((indicePActual+1)*sizeof(PuntoActual));
	if (visitados == NULL)
    {
		printf("\n\tNo se pudo reservar la memoria. El programa se cerrara\n");
		exit(0);
	}

//VERIFICACIÓN DE QUE EXISTA EL PUNTO INICIAL INGRESADO
	camino[0].coordX = atoi(argv[2]); //La coordenada X del punto inicial se toma del segundo argumento del programa convertido a int
	camino[0].coordY = atoi(argv[3]); //La coordenada Y del punto inicial se toma del tercer argumento del programa convertido a int
	visitados[0].coordX = camino[0].coordX; //Se marca como visitado el punto inicial
	visitados[0].coordY = camino[0].coordY;

	if (laberintoEnMatriz[camino[0].coordX][camino[0].coordY] != '0')
	{ //Si el punto ingresado es pared o se sale del arreglo, se cierra el programa
		printf("\n\tNo existe este punto en el laberinto. El programa se cerrara.\n");
		exit(0);
	}
	

//SE ABRE UN ARCHIVO PARA ALMACENAR LOS ARCHIVOS
	archivoCaminos = fopen("Caminos.txt", "w+");
	if (archivoCaminos == NULL)
	{
		printf("\n\n\t\t***No se pudo abrir un archivo. El programa se cerrara.***");
		exit(0);
	}

	close(archivoCaminos);
//BUSCADOR DE CAMINOS EN LABERINTO (ALGORITMO "DEPTH FIRST SEARCH")
	while (todoRevisado == 0) 	//Se repite todo hasta que se hayan revisado todos los caminos
	{
		archivoLaberinto = fopen(argv[1], "a+"); //El nombre del archivo se almacena como el argumento 1 pasado por consola
		if (archivoLaberinto == NULL)
	    {
			printf("\n\tNo se pudo cargar el archivo. El programa se cerrara\n");
			exit(0);
		}

		NORTEVisitado = 0;
		ESTEVisitado = 0;
		SURVisitado = 0;
		OESTEVisitado = 0;

	//Se revisa si ya se revisó todo el laberinto
		if (indiceVisitado+1 == totalDePuntos)
			todoRevisado = 1;

		for(i = 0; i <= indiceVisitado; i++)
		{
			if (camino[indicePActual].coordX-1 == visitados[i].coordX && camino[indicePActual].coordY == visitados[i].coordY)
				NORTEVisitado = 1;
			if (camino[indicePActual].coordX == visitados[i].coordX && camino[indicePActual].coordY+1 == visitados[i].coordY)
				ESTEVisitado = 1;
			if (camino[indicePActual].coordX+1 == visitados[i].coordX && camino[indicePActual].coordY == visitados[i].coordY)
				SURVisitado = 1;
			if (camino[indicePActual].coordX == visitados[i].coordX && camino[indicePActual].coordY-1 == visitados[i].coordY)
				OESTEVisitado = 1;
		}

		if (NORTEVisitado == 0)
		{
			if (camino[indicePActual].coordX-1 < 0)
				caminoNORTE();
			else if (camino[indicePActual].coordX-1 >= 0 && laberintoEnMatriz[camino[indicePActual].coordX-1][camino[indicePActual].coordY] == '0' && NORTEVisitado == 0)
				caminoNORTE();
			else if (camino[indicePActual].coordY+1 > (columnas-1))
				caminoESTE();
			else if (camino[indicePActual].coordY+1 <= (columnas-1) && laberintoEnMatriz[camino[indicePActual].coordX][camino[indicePActual].coordY+1] == '0' && ESTEVisitado == 0)
				caminoESTE();
			else if (camino[indicePActual].coordX+1 > (renglones-1))
				caminoSUR();
			else if (camino[indicePActual].coordX+1 <= (renglones-1) && laberintoEnMatriz[camino[indicePActual].coordX+1][camino[indicePActual].coordY] == '0' && SURVisitado == 0)
				caminoSUR();
			else if (camino[indicePActual].coordY-1 < 0)
				caminoOESTE();
			else if (camino[indicePActual].coordY-1 >= 0 && laberintoEnMatriz[camino[indicePActual].coordX][camino[indicePActual].coordY-1] == '0' && OESTEVisitado == 0)
				caminoOESTE();
		}

		else if (ESTEVisitado == 0)
		{
			if (camino[indicePActual].coordY+1 > (columnas-1))
				caminoESTE();
			else if (camino[indicePActual].coordY+1 <= (columnas-1) && laberintoEnMatriz[camino[indicePActual].coordX][camino[indicePActual].coordY+1] == '0' /*&& ESTEVisitado == 0*/)
				caminoESTE();
			else if (camino[indicePActual].coordX+1 > (renglones-1))
				caminoSUR();
			else if (camino[indicePActual].coordX+1 <= (renglones-2) && laberintoEnMatriz[camino[indicePActual].coordX+1][camino[indicePActual].coordY] == '0' /*&& SURVisitado == 0*/)
				caminoSUR(); //PROBLEMA DE QUE NO VAYA AL OESTE AQUÍ
			else if (camino[indicePActual].coordY-1 < 0)
				caminoOESTE();
			else if (camino[indicePActual].coordY-1 >= 0 && laberintoEnMatriz[camino[indicePActual].coordX][camino[indicePActual].coordY-1] == '0' /*&& OESTEVisitado == 0*/)
				caminoOESTE();
		}
		
		else if (SURVisitado == 0) 
		{
			if (camino[indicePActual].coordX+1 > (renglones-1))
				caminoSUR();
			else if (camino[indicePActual].coordX+1 <= (renglones-1) && laberintoEnMatriz[camino[indicePActual].coordX+1][camino[indicePActual].coordY] == '0' /*&& SURVisitado == 0*/)
				caminoSUR();
			else if (camino[indicePActual].coordY-1 < 0)
				caminoOESTE();
			else if (camino[indicePActual].coordY-1 >= 0 && laberintoEnMatriz[camino[indicePActual].coordX][camino[indicePActual].coordY-1] == '0' /*&& OESTEVisitado == 0*/)
				caminoOESTE();
		}

		else if (OESTEVisitado == 0)
		{
			if (camino[indicePActual].coordY-1 < 0) 
				caminoOESTE();
			else if (camino[indicePActual].coordY-1 >= 0 && laberintoEnMatriz[camino[indicePActual].coordX][camino[indicePActual].coordY-1] == '0' /*&& OESTEVisitado == 0*/)
				caminoOESTE();
		}

	//Se revisa si el arreglo con el camino está vacío y se rellena con el punto inicial si lo está
		if (indicePActual < 0) 	
		{
			printf("Se reasignó el valor inicial\n");
			indicePActual ++;
			camino = (PuntoActual*) realloc(camino, (indicePActual+1)*sizeof(PuntoActual));			
			camino[0].coordX = atoi(argv[2]);
			camino[0].coordY = atoi(argv[3]);
		}
	}
	printf("Se encontraron todos los caminos y se han almacenado en un archivo.\n");

//LIBERACIÓN DE MEMORIA Y CIERRE DE ARCHIVOS
	for (i = 0; i < renglones; i++)
    	free(laberintoEnMatriz[i]);
    free(laberintoEnMatriz);
    free(visitados);
    free(camino);
    //fclose(archivoCaminos);

    return 0;
}

//Función que verifica si al NORTE de un punto existe camino
void caminoNORTE()
{	//Se revisa si existe camino al norte del punto actual
	if (camino[indicePActual].coordX-1 < 0) //Si el punto al norte se sale de los índices del arreglo, se habría encontrado una meta
	{
		for (i = 0; i <= indicePActual; i++)
			fprintf(archivoCaminos, "(%d, %d) ", camino[i].coordX, camino[i].coordY); //Se vuelca la memoria
		fprintf(archivoCaminos, "\n"); //Se imprime salto de línea en archivo por si se almacenan más caminos
		fclose(archivoCaminos);
		exit(0);
		indicePActual --; //Se reduce el tamaño del arreglo que contiene el camino, borrando el último dato ingresado (bakctrack)
		camino = (PuntoActual*) realloc(camino, (indicePActual+1)*sizeof(PuntoActual));
	}
	else if (laberintoEnMatriz[camino[indicePActual].coordX-1][camino[indicePActual].coordY] == '0')
	{

		if (NORTEVisitado == 1) 					//Si el punto ya se visitó
		{
			indicePActual --;
			camino = (PuntoActual*) realloc(camino, (indicePActual+1)*sizeof(PuntoActual));
		}
		else if (NORTEVisitado == 0)
		{
			indicePActual ++;
			camino = (PuntoActual*) realloc(camino, (indicePActual+1)*sizeof(PuntoActual));
			camino[indicePActual].coordX = camino[indicePActual-1].coordX-1;
			camino[indicePActual].coordY = camino[indicePActual-1].coordY;
			agregarVisitado();
		}
	}
}

//Función que verifica si al ESTE de un punto existe camino
void caminoESTE()
{
	if (camino[indicePActual].coordY+1 > (columnas-1)) //Si el punto al norte se sale de los índices del arreglo, se habría encontrado una meta
	{ 
		for (i = 0; i <= indicePActual; i++)
			fprintf(archivoCaminos, "(%d, %d) ", camino[i].coordX, camino[i].coordY); //Se vuelca la memoria
		fprintf(archivoCaminos, "\n"); //Se imprime salto de línea en archivo por si se almacenan más caminos
		fclose(archivoCaminos);
		exit(0);
		indicePActual --; //Se reduce el tamaño del arreglo que contiene el camino, borrando el último dato ingresado (bakctrack)
		camino = (PuntoActual*) realloc(camino, (indicePActual+1)*sizeof(PuntoActual));
	}
	else if (laberintoEnMatriz[camino[indicePActual].coordX][camino[indicePActual].coordY+1] == '0')
	{
		if (ESTEVisitado == 1) 
		{
			indicePActual --; //Se reduce el tamaño del arreglo que contiene el camino, borrando el último dato ingresado
			camino = (PuntoActual*) realloc(camino, (indicePActual+1)*sizeof(PuntoActual));
		}
		else if (ESTEVisitado == 0)
		{
			indicePActual ++;
			camino = (PuntoActual*) realloc(camino, (indicePActual+1)*sizeof(PuntoActual));
			camino[indicePActual].coordX = camino[indicePActual-1].coordX;
			camino[indicePActual].coordY = camino[indicePActual-1].coordY+1;
			agregarVisitado();		
		}
	}
}

//Función que verifica si al SUR de un punto existe camino
void caminoSUR()
{
	if (camino[indicePActual].coordX+1 > (renglones-1)) //Si el punto al norte se sale de los índices del arreglo, se habría encontrado una meta
	{ 
		for (i = 0; i <= indicePActual; i++)
			fprintf(archivoCaminos, "(%d, %d) ", camino[i].coordX, camino[i].coordY); //Se vuelca la memoria
		fprintf(archivoCaminos, "\n"); //Se imprime salto de línea en archivo por si se almacenan más caminos
		fclose(archivoCaminos);
		exit(0);
		indicePActual --; //Se reduce el tamaño del arreglo que contiene el camino, borrando el último dato ingresado (bakctrack)
		camino = (PuntoActual*) realloc(camino, (indicePActual+1)*sizeof(PuntoActual));
	}
	else if (laberintoEnMatriz[camino[indicePActual].coordX+1][camino[indicePActual].coordY] == '0')
	{
		if (SURVisitado == 1)
		{
			indicePActual --; //Se reduce el tamaño del arreglo que contiene el camino, borrando el último dato ingresado
			camino = (PuntoActual*) realloc(camino, (indicePActual+1)*sizeof(PuntoActual));
		}
		else if (SURVisitado == 0)
		{
			indicePActual ++;
			camino = (PuntoActual*) realloc(camino, (indicePActual+1)*sizeof(PuntoActual));
			camino[indicePActual].coordX = camino[indicePActual-1].coordX+1;
			camino[indicePActual].coordY = camino[indicePActual-1].coordY;
			agregarVisitado();		
		}
	}
}

//Función que verifica si al OESTE de un punto existe camino
void caminoOESTE()
{
	if (camino[indicePActual].coordY-1 < 0) //Si el punto al norte se sale de los índices del arreglo, se habría encontrado una meta
	{ 
		for (i = 0; i <= indicePActual; i++)
			fprintf(archivoCaminos, "(%d, %d) ", camino[i].coordX, camino[i].coordY); //Se vuelca la memoria
		fprintf(archivoCaminos, "\n"); //Se imprime salto de línea en archivo por si se almacenan más caminos
		fclose(archivoCaminos);
		exit(0);
		indicePActual --; //Se reduce el tamaño del arreglo que contiene el camino, borrando el último dato ingresado (bakctrack)
		camino = (PuntoActual*) realloc(camino, (indicePActual+1)*sizeof(PuntoActual));
	}
	else if (laberintoEnMatriz[camino[indicePActual].coordX][camino[indicePActual].coordY-1] == '0')
	{
		if (OESTEVisitado == 1) 
		{
			indicePActual --;
			camino = (PuntoActual*) realloc(camino, (indicePActual+1)*sizeof(PuntoActual));
		}
		else if (OESTEVisitado == 0)
		{
			indicePActual ++;
			camino = (PuntoActual*) realloc(camino, (indicePActual+1)*sizeof(PuntoActual));
			camino[indicePActual].coordX = camino[indicePActual-1].coordX;
			camino[indicePActual].coordY = camino[indicePActual-1].coordY-1;
			agregarVisitado();		
		}
	}
}
//FUNCIÓN QUE AGREGA ELEMENTOS AL ARREGLO DE VISITADOS
void agregarVisitado()
{
	indiceVisitado ++;
	visitados = (PuntoVisitado*) realloc(visitados, (indiceVisitado+1)*sizeof(PuntoVisitado));
	visitados[indiceVisitado].coordX = camino[indicePActual].coordX;
	visitados[indiceVisitado].coordY = camino[indicePActual].coordY;
	if (visitados == NULL)
	{
		printf("No se pudo reservar memoria para almacenar puntos visitados. El programa se cerrará.");
		exit (0);
	}
}