#include"head.h"

double function(int formula, int size, int i, int j) {

                switch (formula){


                    case 1:

                        return (size - std::max(i+1,j+1) + 1);


                    case 2:

                        return std::max(i+1,j+1);


                    case 3:

                        return fabs(i-j);


                    case 4:

                        return (1/((double)i + j + 1));

                }

    return 0;

}

void input (int size, int formula, int fr, int lr, int flag, double* M){

    if(flag==1){

        for(int i=0; i<size; i++){

            for(int j=0; j< size; j++){

                M[i*size + j] = function(formula, size, i, j);
            }
        }

    }

    else if(flag==2){

         for(int i=fr; i<lr; i++){

            for(int j=0; j<size; j++){

                M[(i - fr)*size + j] = function(formula, size, i, j);
            }
        }

    }
}
