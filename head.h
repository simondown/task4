#include<new>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include<mpi.h>
#define eps 1e-15


 double function(int, int, int, int );

 void input(int, int, int, int, int, double*);

 int text(int, char* , double* );

 int check_text(int, char* );

 void print(int, int, int, int, int, double*);

 void vector(int, double* , double* );

 int solution(int, int, int, int, int* , int* , double* , double* , double*, double* , double* , int* , int, int );

 int sign (double);

 void accuracy( int, double* , double* , double* );

 void norma(int, double* , double* , double* , double* , double*);

