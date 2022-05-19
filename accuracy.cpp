#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void accuracy(int size, double* X, double* Xn, double* P){


    if(size%2 == 0){

        for (int i=0; i<size/2; i++){

            Xn[2*i] = 1;
            Xn[2*i+1] = 0;
        }
    }

    if(size%2 == 1){

        for (int i=0; i<(size/2); i++ ){

            Xn[2*i] = 1;
            Xn[2*i+1] = 0;
        }

      Xn[size-1] = 1;
    }

    for(int i=0; i<size; i++){

        Xn[i] -= X[i];

    }

    for(int i=0; i<size; i++){

        *P += Xn[i]*Xn[i];
    }

    *P = sqrtf(*P);

}

