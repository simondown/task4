#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void norma(int size, double* M, double* V, double* X, double* Xn, double* N){


    double N1 = 0;
    double N2 = 0;


    for(int i=0; i<size; i++){

        for(int j=0; j<size; j++){

            Xn[i] += M[i*size + j]*X[j];

        }
    }

    for (int i=0; i<size; i++){

        Xn[i] -= V[i];

    }

    for(int i=0; i<size; i++){

    N1 += Xn[i]*Xn[i];

    }

    for(int i=0; i<size; i++){

    N2 += V[i]*V[i];

    }


    N1 = sqrtf(N1);
    N2 = sqrtf(N2);

    *N = N1/N2;

}
