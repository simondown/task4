#include"head.h"
#define MPI_ID 200

int solution(int size, int num, int rank, int quant, int* puc, int* P, double* M, double* M0, double* buf, double* V, double* X, int* D, int fr, int lr){

    int i = 0;
    int j = 0;
    int k = 0;
    double temp1, temp2, temp3;
    int a;
    int flag = 0;
//     int res = 4;

//     printf("Check2 from rank %d\n", rank);

    if(rank==0){

        D[0] = sign(M[0]);

        if(D[0]==0)
            flag = -1;

        M[0] = sqrtf(std::fabs(M[0]));

        temp3=D[0]*M[0];

    }

    MPI_Bcast(&flag, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(flag != 0){

//         printf("i am rank %d, flag is %d\n", rank, flag);
        return 1;
    }

//     printf("Check3 from rank %d\n", rank);

    MPI_Bcast(&D[0], 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&temp3, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for(i=0; i<quant; i++){

        if(i==0 && rank==0)
            continue;

        M[i*size] /= temp3;

    }


    MPI_Barrier(MPI_COMM_WORLD);

//     printf("Check4 from rank %d\n", rank);

//     printf("My rank %d, D0 = %d, buf = %10.3e\n", rank, D[0], buf);


    for(i=1; i<size; i++){

//         printf("rank %d here, i = %d\n", rank, i);

        a = i*num/size;

        if ((a+1)*size/num <= i)
            a++;

        if(rank==a){

            temp1 = 0;

            temp1 = M[(i-fr)*size + i];

           /* if(std::fabs(temp1) < eps*size){

                printf("err1\n");

                flag = -1;;

            }*/

            for(j = 0; j < i; j++){

                temp1 -= M[(i-fr)*size + j]*M[(i-fr)*size + j]*D[j];

            }

            if(std::fabs(temp1/M[(i-fr)*size + i]) < eps*size){

//                 printf("err2\n");

                flag = -1;;

            }

            D[i] = sign(temp1);

            M[(i-fr)*size + i] = sqrtf(fabs(temp1));

            temp2 = M[(i-fr)*size + i]*D[i];

            for(j=0; j<i; j++)
                buf[j] = M[(i-fr)*size + j];

        }

        MPI_Barrier(MPI_COMM_WORLD);

        MPI_Bcast(&flag, 1, MPI_INT, a, MPI_COMM_WORLD);

        if(flag != 0)
            return 1;

        MPI_Bcast(&D[i], 1, MPI_INT, a, MPI_COMM_WORLD);

        MPI_Bcast(&temp2, 1, MPI_DOUBLE, a, MPI_COMM_WORLD);

        MPI_Bcast(buf, i, MPI_DOUBLE, a, MPI_COMM_WORLD);

        temp1 = 0;

        a = (i+1)*num/size;

        if ((a+1)*size/num <= i+1)
            a++;

        if (rank==a){

            for (j=i+1-fr; j<quant; j++)
                M[j*size + i] /= temp2;
        }

        if (rank>a){

            for (j=0; j<quant; j++)
                M[j*size + i ]/= temp2;
        }

        MPI_Barrier(MPI_COMM_WORLD);

        if(rank == a){

            for (j=i+1-fr; j<quant; j++){

                for (k=0; k<i; k++){

                    temp1 += buf[k]*M[j*size + k]*D[k];
                }

            M[j*size + i] -= temp1/temp2;
            temp1 = 0;

            }
        }

        if(rank>a){

            for(j=0; j<std::min(size, quant); j++){

                for(k=0; k<i; k++){

                    temp1 += buf[k]*M[j*size + k]*D[k];
                }

            M[j*size + i] -= temp1/temp2;

            temp1 = 0;

            }

        }

        temp1 = 0;

        MPI_Barrier(MPI_COMM_WORLD);
    }



    for(i=fr; i<lr; i++){

        for (j=i+1; j<size; j++){

            M[(i-fr)*size + j] = 0;

        }
    }



//     printf("My rank is %d, my part of the L-matrix is:\n", rank);

//     print(res, size, fr, lr, 1, M);

    for(i=0; i<size; i++){

//         printf("D[%d] = %d", i, D[i]);

        buf[i] = D[i];

//         printf("B[%d] = %10.3e", i, buf2[i]);
//

    }

    MPI_Barrier(MPI_COMM_WORLD);

    D[rank] = quant;

//     printf("My rank is %d, my quant is:%d\n", rank, quant);
//     printf("My rank is %d, my buff2 is:%d\n", rank, D[rank]);

    /*if(rank!=0)
        MPI_Send(&D[rank], 1, MPI_INT, 0, MPI_ID, MPI_COMM_WORLD);

    if(rank==0){

        for(int g = 1; g<num; g++){

            MPI_Recv(&buf, 1, MPI_INT, g, MPI_ID, MPI_COMM_WORLD, &status);

            D[g] = buf;


        }

    }*/


    MPI_Gather(&D[rank], 1, MPI_INT, puc, 1, MPI_INT, 0, MPI_COMM_WORLD);


    MPI_Barrier(MPI_COMM_WORLD);


  /*  if(rank==0){

        for(int o = 0; o< num; o++){

            printf("D is %d;", puc[o]);

        }

        printf("\n  ");

    }

//     MPI_Barrier(MPI_COMM_WORLD);*/

    if(rank == 0){

//     printf("Puc is:\n");

        for(int l = 0; l < num; l++){

            puc[l] = puc[l]*size;

//             printf(" %d ", puc[l]);

        }

    }

    if(rank==0){

    P[0]=0;

    for(i=1; i<num; i++)
        P[i] = P[i-1] + puc[i-1];

    }

 //   printf("P is:\n");

 //   for(int l = 0; l < num; l++){

   //     printf(" %d ", P[l]);

  //  }

 //   printf("\n  ");

    MPI_Gatherv(M, quant*size, MPI_DOUBLE, M0, puc, P, MPI_DOUBLE, 0, MPI_COMM_WORLD);


    for(i=0; i<size; i++){

//          printf("B[%d] = %10.3e  ", i, buf2[i]);

    D[i] = buf[i];

//          printf("D[%d] = %d  ", i, D[i]);
//

    }



//     MPI_Barrier(MPI_COMM_WORLD);

    if (rank==0){

//         printf("My rank is %d, the D is:\n", rank);

//         print(num, size, fr, lr, 3, D);

//         printf("\n\n\n\n\n");


//         printf("My rank is %d, the matrix L is:\n", rank);

//         print(res, size, fr, lr, 4, M0);

       for(i=0; i<size; i++)
            buf[i] = 0;

        for(i=0; i < size; i++){

            for (j = 0; j < i; j++){

                V[i] -= M0[i*size + j]*buf[j];

            }

            buf[i] = V[i]/M0[i*size + i];
        }

//         printf("Vector Xn is:\n");

//         for(i=0; i<size; i++)
//             printf(" buf2[%d]=%10.3e  ", i, buf2[i]);


        for(i=0; i<size; i++){

            for ( j=0; j<size; j++){

                M0[i*size + j] = M0[j*size + i];
            }
        }

        for(i=0; i<size; i++){

            for (j=0; j<i; j++){

                M0[i*size + j] = 0;

            }
        }

//         printf("\n\n");

//          printf("My rank is %d, the matrix A is:\n", rank);

//          print(res, size, fr, lr, 4, M0);


        for(i=0; i<size; i++)
            buf[i] *= D[i];

//         printf("Vector D is:\n");

//         for(i=0; i<size; i++)
//             printf("%10.3e  ", buf2[i]);

//         printf("\n\n");

        for (i = size-1; i>=0; i--){

            for( j = size-1; j>i; j--){

                buf[i] -= M0[i*size + j]*X[j];

            }
            X[i] = buf[i]/M0[i*size + i];
        }

//         printf("Vector X is:\n");

//         for(i=0; i<size; i++)
//             printf("%10.3e  ", X[i]);

//         printf("\n\n");


   }

    MPI_Bcast(X, size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    return 0;
}

int sign(double x){

    if (x > 0.0) return 1;
    if (x < 0.0) return -1;

    return 0;
}



