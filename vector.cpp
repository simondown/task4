#include<stdio.h>
#include<stdlib.h>

void vector(int size, double* M, double* V){

    for(int i=0; i<size; i++){

        V[i] = 0;

    }


    for(int i=1; i<size+1; i++){

         for(int j=0; j<((size+1)/2); j++){

            V[i-1] += M[(i-1)*size + 2*j];
         }

     }

}

