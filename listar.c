#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "listar.h"
int contarCoincidencias(char * palabra1[], char * palabra2[], int cantP1, int cantP2){
  int i,j;
  int seguir=1;
  for(i=0;seguir && i<cantP1 && i<cantP2;i++){
    if(strcmp(palabra1[i],palabra2[i])!=0)
      seguir=0;
  }
  return i-1;
}
// Buscar el proximo indice en base a la cantidad de coincidencias.
int buscarSiguiente(int indiceActual, char ** tabla[],int impresos[],int palabras[],int rutas){
  int i,actual,indiceMayor,mayor;
  mayor=0;    // Almacena la mayor cantidad de coincidencias.
  indiceMayor=rutas;      // Almacena el indice con la mayor cantidad de coincidencias (si retorna rutas sera que no obtiene ninguna coincidencia).
  for(i=0; i<rutas;i++){
    if(impresos[i]==0 && i!=indiceActual){
      actual = contarCoincidencias(tabla[indiceActual],tabla[i],palabras[indiceActual],palabras[i]);
      if(actual>mayor){
        mayor=actual;
        indiceMayor = i;
      }
    }
  }
  return indiceMayor;
}
int * ordenar(char ** tabla[], int cantPalabras[], int cantRutas,int orden[]){
  int j,i;
  int prox=0;
  int ordenar=0;
  int ordenados[cantRutas];
  for(i=0;i<cantRutas;i++){
    ordenados[i]=0;
  }
  for(j=0;j<cantRutas;j++){
    orden[j]=ordenar;
    ordenados[ordenar] = 1;
    prox = buscarSiguiente(ordenar,tabla,ordenados,cantPalabras,cantRutas);
    if (prox==cantRutas){
      prox=0;
      for(i=0; i<cantRutas && prox==0;i++){
        if (ordenados[i]==0){
          prox=i;
        }
      }
    }
    ordenar=prox;
  }
}

char * listarArbol(char * rutas[], int cantRutas ){

    char ** tabla[cantRutas];                       // Almacena las Tabla[ruta] almacenara la ruta fragmentada por palabra.
    int cantPalabras[cantRutas];                    // Almacena la cantidad de palabras que tendra cada ruta.
    int i,j;                                        // Variables de control de bucles.
    // Completar tabla y cantPalabras.
    for(j=0;j<cantRutas;j++) {
      tabla[j]=malloc(sizeof(int*));
      char *token = strtok(rutas[j], "/");
      cantPalabras[j]=0;
      for (i=0;token != NULL;i++){
        tabla[j][i] = malloc(strlen(token+1));
        strcpy(tabla[j][i],token);
        token = strtok(NULL, "/");
        cantPalabras[j]++;
      }
    }
    // Encontrar el orden en que se imprimiran las rutas. Se imprime primero tabla[0] y de ahi en mas la que mayor cantidad de palabras coincidentes tenga.
    int orden[cantRutas];
    ordenar(tabla,cantPalabras,cantRutas,orden);

    int rutaMasLarga=0;                             //Variable usada para iniciar un arreglo luego.
    for (i=0;i<cantRutas;i++){
      if(cantPalabras[i]>rutaMasLarga)
        rutaMasLarga=cantPalabras[i];
    }

    int coincidencias,k;
    int * arrCoincidencias[cantRutas];               // arrCoincidencias[i][j] Almacena la cantidad de veces que coincide la palabra j de la ruta i en las rutas siguientes a i.
    for (i=0;i<cantRutas;i++)
      arrCoincidencias[i] = malloc(sizeof(int*));
    for (i=0;i<cantRutas;i++){
      for(j=0;j<rutaMasLarga;j++){
        arrCoincidencias[i][j]=0;
      }
      for(j=i+1;j<cantRutas;j++){
        coincidencias=contarCoincidencias(tabla[orden[i]],tabla[orden[j]],cantPalabras[orden[i]],cantPalabras[orden[j]]);
        for(k=0;k<coincidencias;k++){
          arrCoincidencias[i][k]++;
        }
      }
    }
    // Arranca el imprimir por pantalla:
    char * retorno =malloc(10000);
    strcpy(retorno,".\n");
    for(i=0;i<cantRutas;i++){
      if(i==0){
        // printf(".\n");
        for (j=0;j<cantPalabras[i];j++){
          for(k=0;k<j;k++){
            if (arrCoincidencias[i][k]>0 ) //Falta tomar en cuenta al proximo (cuando se cierran 2 carpetas).
              strcat(retorno,"│  ");
              //
              // printf("│  ");
            else
              strcat(retorno,"   ");
              // printf("   ");
          }
          if(j!=0){   //Caso especial cuando j-1 se cae del arreglo.
            if(arrCoincidencias[orden[i]][j-1]!=0){
              strcat(retorno,"├──");
              strcat(retorno,tabla[orden[i]][j]);
            }
              // printf("├──%s",tabla[orden[i]][j] );
            else{
              strcat(retorno,"└──");
              strcat(retorno,tabla[orden[i]][j]);
            }
              // printf("└──%s",tabla[orden[i]][j] );
          }
          else {
            if(i==cantRutas-1){
              strcat(retorno,"└──");
              strcat(retorno,tabla[orden[i]][j]);
            }
              // printf("└──%s",tabla[orden[i]][j] );
            else{
              strcat(retorno,"├──");
              strcat(retorno,tabla[orden[i]][j]);
            }
              // printf("├──%s",tabla[orden[i]][j] );
          }
          strcat(retorno,"\n");
          // printf("\n");
        }
      }
      else {
        for (j=0;j<cantPalabras[orden[i]];j++){
          if(arrCoincidencias[i-1][j]==0) {
            for(k=0;k<j;k++){
              if (k==0){
                if(i==cantRutas-1 || arrCoincidencias[i][0]==cantRutas-1-i) {
                  strcat(retorno,"   ");
                  // printf("   ");
                }
                else {
                  strcat(retorno,"│  ");
                  // printf("│  ");
                }
              }
              else{
                if (arrCoincidencias[i][k-1]>0 )   // Si no es el ultimo en la misma carpeta
                  strcat(retorno,"│  ");
                  // printf("│  ");
                else
                  strcat(retorno,"   ");
                  // printf("   ");
              }
            }
            if (j==0){
              if (i==cantRutas-1 || arrCoincidencias[i][0]==cantRutas-1-i) {
                strcat(retorno,"└──");
                strcat(retorno,tabla[orden[i]][j]);
                strcat(retorno,"\n");
              }
                // printf("└──%s\n",tabla[orden[i]][j] );
              else {
                strcat(retorno,"├──");
                strcat(retorno,tabla[orden[i]][j]);
                strcat(retorno,"\n");
              }
                // printf("├──%s\n",tabla[orden[i]][j] );
            }
            else {
              if (arrCoincidencias[i][j-1]!=0){
                strcat(retorno,"├──");
                strcat(retorno,tabla[orden[i]][j]);
                strcat(retorno,"\n");
              }
                // printf("├──%s\n",tabla[orden[i]][j] );
              else {
                strcat(retorno,"└──");
                strcat(retorno,tabla[orden[i]][j]);
                strcat(retorno,"\n");
              }
                // printf("└──%s\n",tabla[orden[i]][j] );
            }
          }
        }
      }
    }
  return retorno;
}
  /*char ** tabla[cantRutas];                       // Almacena las Tabla[ruta] almacenara la ruta fragmentada por palabra.
  int cantPalabras[cantRutas];                    // Almacena la cantidad de palabras que tendra cada ruta.
  int i,j;                                        // Variables de control de bucles.
  // Completar tabla y cantPalabras.
  for(j=0;j<cantRutas;j++) {
    tabla[j]=malloc(sizeof(int*));
    char *token = strtok(rutas[j], "/");
    cantPalabras[j]=0;
    for (i=0;token != NULL;i++){
      tabla[j][i] = malloc(strlen(token+1));
      strcpy(tabla[j][i],token);
      token = strtok(NULL, "/");
      cantPalabras[j]++;
    }
  }
  // Encontrar el orden en que se imprimiran las rutas. Se imprime primero tabla[0] y de ahi en mas la que mayor cantidad de palabras coincidentes tenga.
  int orden[cantRutas];
  ordenar(tabla,cantPalabras,cantRutas,orden);

  int rutaMasLarga=0;                             //Variable usada para iniciar un arreglo luego.
  for (i=0;i<cantRutas;i++){
    if(cantPalabras[i]>rutaMasLarga)
      rutaMasLarga=cantPalabras[i];
  }

  int coincidencias,k;
  int * arrCoincidencias[cantRutas];               // arrCoincidencias[i][j] Almacena la cantidad de veces que coincide la palabra j de la ruta i en las rutas siguientes a i.
  for (i=0;i<cantRutas;i++)
    arrCoincidencias[i] = malloc(sizeof(int*));
  for (i=0;i<cantRutas;i++){
    for(j=0;j<rutaMasLarga;j++){
      arrCoincidencias[i][j]=0;
    }
    for(j=i+1;j<cantRutas;j++){
      coincidencias=contarCoincidencias(tabla[orden[i]],tabla[orden[j]],cantPalabras[orden[i]],cantPalabras[orden[j]]);
      for(k=0;k<coincidencias;k++){
        arrCoincidencias[i][k]++;
      }
    }
  }
  // Arranca el imprimir por pantalla:
  for(i=0;i<cantRutas;i++){
    if(i==0){
      printf(".\n");
      for (j=0;j<cantPalabras[i];j++){
        for(k=0;k<j;k++){
          if (arrCoincidencias[i][k]>0 ) //Falta tomar en cuenta al proximo (cuando se cierran 2 carpetas).
            printf("│  ");
          else
            printf("   ");
        }
        if(j!=0){   //Caso especial cuando j-1 se cae del arreglo.
          if(arrCoincidencias[orden[i]][j-1]!=0)
            printf("├──%s",tabla[orden[i]][j] );
          else
            printf("└──%s",tabla[orden[i]][j] );
        }
        else {
          if(i==cantRutas-1)
            printf("└──%s",tabla[orden[i]][j] );
          else
            printf("├──%s",tabla[orden[i]][j] );
        }
        printf("\n" );
      }
    }
    else {
      for (j=0;j<cantPalabras[orden[i]];j++){
        if(arrCoincidencias[i-1][j]==0) {
          for(k=0;k<j;k++){
            if (k==0){
              if(i==cantRutas-1 || arrCoincidencias[i][0]==cantRutas-1-i) {
                printf("   ");
              }
              else {
                printf("│  ");
              }
            }
            else{
              if (arrCoincidencias[i][k-1]>0 )   // Si no es el ultimo en la misma carpeta
                printf("│  ");
              else
                printf("   ");
            }
          }
          if (j==0){
            if (i==cantRutas-1 || arrCoincidencias[i][0]==cantRutas-1-i)
              printf("└──%s\n",tabla[orden[i]][j] );
            else
              printf("├──%s\n",tabla[orden[i]][j] );
          }
          else {
            if (arrCoincidencias[i][j-1]!=0)
              printf("├──%s\n",tabla[orden[i]][j] );
            else
              printf("└──%s\n",tabla[orden[i]][j] );
          }
        }
      }
    }
  }
}*/
