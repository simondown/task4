#include"head.h"
#define MPI_ID 100

int main (int argc, char **argv){

//     MPI_Status status;

    int size;
    int res;
    int formula;
    double N = 0;
    double P = 0;
    double* M = 0;
    double* M0 = 0;
    double* V = 0;
    double* X = 0;
    double* buf = 0;
    int* PP = 0;
    int* puc = 0;
    int* D = 0;
    char file[50];
    double time;
    int num, rank;
    int fr, lr, quant;


    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &num);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank==0)
        printf("Total processes count = %d\n", num);

    if(argc < 4 || argc > 5){

        if (rank ==0){

            printf("\nWrong parameters\n\n");

            MPI_Abort(MPI_COMM_WORLD,1);
        }


    }

    sscanf(argv[1], "%d", &size);
    if (rank ==0)
        printf("\nThe size of the matrix: %d\n\n", size);


    if(num > size-1){

            if(rank == 0){

                printf("Too many slots");

                MPI_Abort(MPI_COMM_WORLD,2);
            }

    }


    sscanf(argv[2], "%d", &res);

    if((res > size)&&(rank==0)){

        if (rank ==0){

            printf("Wrong parameters\n");

            MPI_Abort(MPI_COMM_WORLD,3);
        }


    }

    fr = (size*rank)/num;
    lr = (size*(1+rank))/num;

    quant = lr - fr;

   // printf("My rank is %d, quant is %d\n", rank, quant);
//     printf("My rank is %d, quant is %d, fr is %d, lr is %d\n", rank, quant, fr, lr);

    MPI_Barrier(MPI_COMM_WORLD);

    try {

        M = new double[size*quant];
        M0 = new double[size*size];

    } catch (std::bad_alloc &e) {

         delete[] M0;
         delete[] M;

        if(rank==0){

            printf("Can't allocate memory");
            MPI_Abort(MPI_COMM_WORLD,4);
        }

    }

    sscanf(argv[3], "%d", &formula);

    if(formula < 0 || formula > 4){

         delete[] M;
         delete[] M0;

        if(rank==0){

            printf("Wrong formula\n\n");
            MPI_Abort(MPI_COMM_WORLD,5);
        }

    }


    if(formula != 0){

        if(rank == 0)
            printf("You chose formula №%d\n\n", formula);

        if(rank==0)
            input(size, formula, fr, lr, 1, M0);

        input(size, formula, fr, lr, 2, M);

        MPI_Barrier(MPI_COMM_WORLD);
    }


    else{

        if(argc < 5) {

            if(rank==0)
                printf("Please, enter the file\n");

             delete[] M;
             delete[] M0;

            if(rank==0)
                MPI_Abort(MPI_COMM_WORLD,6);

        }

        sscanf(argv[4], "%s", file);

        if(rank == 0){
            printf("From the file: %s\n\n", file);
        }

            if (text(size, file, M0) != 0) {

                if(rank==0)
                    printf("Wrong file\n");

             delete[] M0;
             delete[] M;

            if(rank==0)
                MPI_Abort(MPI_COMM_WORLD,7);

            }




    }

    MPI_Barrier(MPI_COMM_WORLD);

    if(formula==0){

        for(int i=fr; i<lr; i++){

            for(int j=0; j<size; j++){

                M[(i-fr)*size + j]=M0[i*size + j];
            }
        }

    }

    if(rank==0){

        printf("The matrix A is:\n");
        print(res, size, fr, lr, 4, M0);
    }

//     printf("My rank is %d, my part of the matrix A is:\n", rank);

//     print(res, size, fr, lr, 1, M);

    MPI_Barrier(MPI_COMM_WORLD);

    try {

        V = new double[size];

    } catch (std::bad_alloc &e) {

        delete[] M0;
        delete[] M;
        delete[] V;

        if(rank==0){
            printf("Can't allocate memory");
        }

        if(rank==0)
            MPI_Abort(MPI_COMM_WORLD,8);

    }



    vector(size, M0, V);

    if(rank==0){

        printf("The vector b is:\n");

        print(res, size, rank, quant, 3, V);

    }

    MPI_Barrier(MPI_COMM_WORLD);

//     printf("Cheeeeck\n\n\n");


     try {

        X = new double[size];

        D = new int[size];

        buf = new double[size];

        puc = new int[num];

        PP = new int[num];

//         printf("Cheegfhgfhgeeck\n\n\n");

     } catch (std::bad_alloc &e) {

        delete[] M0;
        delete[] puc;
        delete[] PP;
        delete[] M;
        delete[] V;
        delete[] X;
        delete[] D;
        delete[] buf;

        if(rank==0){

        printf("Can't allocate memory");

        MPI_Abort(MPI_COMM_WORLD,9);

        }
    }

  //  printf("Check1 from rank %d\n", rank);

    time = MPI_Wtime();

    int uup = solution(size, num, rank, quant, puc, PP, M, M0, buf, V, X, D, fr, lr);

    MPI_Barrier(MPI_COMM_WORLD);

    time = MPI_Wtime() - time;



    if(uup != 0){

        delete[] D;
        delete[] M;
        delete[] M0;
        delete[] V;
        delete[] buf;
        delete[] X;
        delete[] puc;
        delete[] PP;

        MPI_Barrier(MPI_COMM_WORLD);

        if(rank==0){

            printf("Non-zero minors of the matrix are required\n");


            MPI_Abort(MPI_COMM_WORLD,10);
        }

    }

    MPI_Barrier(MPI_COMM_WORLD);


    if(rank==0){

//         printf("The  1 solution is:\n");

//         print(res, size, rank, quant, 3, X);

        if(formula == 0)
            text(size, file, M0);

        if(formula != 0)
            input(size, formula, fr, lr,  1, M0);


        vector(size, M0, V);


        for(int i=0; i<size; i++)
            buf[i] = 0;

        norma(size, M0, V, X, buf, &N);

        for(int i=0; i<size; i++)
            buf[i] = 0;

        accuracy(size, X, buf, &P);

    }

    MPI_Bcast(&P, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if(P > 1){

         delete[] D;
         delete[] M;
         delete[] M0;
         delete[] V;
         delete[] buf;
         delete[] X;
         delete[] puc;
         delete[] PP;

         MPI_Barrier(MPI_COMM_WORLD);

         if(rank==0){

                printf("Non-zero minors of the matrix are required");

                MPI_Abort(MPI_COMM_WORLD,11);
         }

    }

    MPI_Barrier(MPI_COMM_WORLD);

    if(rank==0){

        printf("The solution is:\n");

        print(res, size, rank, quant, 3, X);

        printf("Time required: %10.3e\n\n", time);

        printf("%s: residual = %e, elapsed = %10.3e, k = %d, n = %d, m = %d, p = %d\n\n", argv[0], N, time, formula, size, res, num);

        printf("Accuracy: %10.3e\n\n\n\n", P);
       }

    delete[] D;
    delete[] M;
    delete[] M0;
    delete[] V;
    delete[] X;
    delete[] buf;
    delete[] puc;
    delete[] PP;

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Finalize();

    return 0;
}
