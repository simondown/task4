#include"head.h"

void print(int res, int size, int fr, int lr, int p, double* M){

    if(p == 1){

        for (int i=fr; i<fmin(res, lr); i++){

            for(int j=0; j<res; j++){

                printf(" %10.3e", M[(i-fr)*size + j]);

            }

            printf("\n");
        }

    }

   /* if(p==2){

        for(int i=rank*quant; i<(rank+1)*quant; i++)
            printf(" %10.3e", M[i-rank*quant]);

    }
    printf("\n");*/

    if(p==3){

        for(int i=0; i<res; i++)
            printf(" %10.3e", M[i]);

    }

    if(p == 4){

        for (int i=0; i<res; i++){

            for(int j=0; j<res; j++){

                printf(" %10.3e", M[i*size + j]);

            }

            printf("\n");
        }

    }
    printf("\n\n");

}
