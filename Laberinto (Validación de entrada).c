#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
    /*Validacion de entrada de argumentos*/
    while(argc<=1||argc>2)
    {
        if (argc<=1){
            printf ("Por favor, ingrese el nombre del archivo.");
        }
        else{
            if (argc>2){
                printf("Se pasaron demasiados argumentos.");
            }else{
                #define FILENAME argv[1]    /*En caso de optimizaci�n de velocidad, este ser�a la unica linea que quedar�a intacta.*/
            }

        }
    }
    /*Comprobando el archivo*/
    FILE *f;
    f=fopen(FILENAME, "r");
    if (f==0){
        printf("Archivo %s no se pudo abrir\n", FILENAME);
    }

    /*Para las coordenadas de entrada*/
    printf("Ingrese las coordenadas de inicio (fila,columna)");
    scanf("%d,%d",int fila,int columna);
    /*Obteniendo las dimensiones del archivo*/

    /*Pasando el archivo a una matriz*/

    /*Se establece el inicio de busqueda*/

    /*
    Algoritmo de busqueda
    +Buscar en celdas aleda�as (+1 columna, - 1 columna,+1 renglon,-1reglon)
    +Definir putnos cr�ticos (Lugares donde existen mas de 1 posible camino)
    +Llevar registro de pasos (Si uno de las casillas se repite, regresar al �ltimo punto cr�tico y descartar desicion previa.)
    +Detenerse hasta que una de las coordenadas sea un borde del mismo archivo.
    +Imprimir conjunto de coordenadas y retomar camino desde el primer punto cr�tico. Repetir busqueda y volver a encontrar salida.
    */
}

