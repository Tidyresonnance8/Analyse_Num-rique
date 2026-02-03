#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cblas.h>


typedef struct {
    double **a;
    double *data;
    int m,n;
} matrix;

matrix* allocate_matrix (int n, int m){
    matrix *mat = (matrix*) malloc(sizeof(matrix));
    mat->m = m;
    mat->n = n;

    mat->a = (double**)malloc(m*sizeof(double));
    if (mat->a == NULL)return NULL;

    mat->data = (double*)malloc(m*n*sizeof(double));
    if (mat->data == NULL)return NULL;

    for (int i=0;i<m;i++) mat->a[i]=mat->data+i*n;
    return mat;
}
void free_matrix (matrix *mat){
    if (mat == NULL)return;
    free (mat->a);
    free (mat->data);
    free(mat);
}

int mult_matrix (matrix *A, matrix *B, matrix *C){
    if (A->n != B->m) return -1;
    if (A->m != C->m) return -1;
    if (B->n != C->n) return -1;
    for(int i=0; i < C->m; i ++){
        for(int j=0; j < C->n; j++){
            C->a[i][j] = 0.0;
        }
    }
    for(int i=0; i < A->m; i++){
        for(int k=0; k< A->n; k++){
            for (int j = 0; j< B->n; j++){
                C-> a[i][j] += A -> a[i][k] * B -> a[k][j];
            }
        }
    }    
    return 0;
    

}

int mult_matrix_blas( matrix *A, matrix *B, matrix *C){
    if (A->n != B->m) return -1;
    if (A->m != C->m) return -1;
    if (B->n != C->n) return -1;
    
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                A->m, B->n, A->n,
                1.0, A->data, A->n,
                B->data, B->n,
                0.0, C->data, C->n);
    return 0;
}

int main(void){
    //matrix *mat = allocate_matrix(100,200);
    //if (mat == NULL) return -1;

    //double **a = mat->a;
    //for (int i=0;i<100;i++){
        //for (int j=0;j<200;j++){
            //a[i][j] = drand48();
       // }
    //}

    //printf("A[12][116] = %12.5E\n", a[12][116]);
    //free_matrix(mat);
    //mat = NULL;
    //return 0;
    int m = 1000, p = 2000, n = 3000;
    matrix *A = allocate_matrix(p, m); // n, m dans ta fonction
    matrix *B = allocate_matrix(n, p);
    matrix *C = allocate_matrix(n, m);
    
    if (!A || !B || !C) return -1;

    for(int i=0; i<m*p; i++)  A->data[i] = drand48();
    for(int i=0; i<p*n; i++)  B->data[i] = drand48();
    
    struct timespec start, end;
    //double total_flops = 2.0 * m * n * p;

    // --- TEST VERSION NAÏVE ---
    clock_gettime(CLOCK_MONOTONIC, &start);
    mult_matrix(A, B, C);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_naive = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    // --- TEST VERSION BLAS ---
    clock_gettime(CLOCK_MONOTONIC, &start);
    mult_matrix_blas(A, B, C);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_blas = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    double total_flops = 2.0 * m * n * p; //
    printf("Naïf : %f s (%f GFLOPS)\n", time_naive, (total_flops/time_naive)/1e9);
    printf("BLAS : %f s (%f GFLOPS)\n", time_blas, (total_flops/time_blas)/1e9);
    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
    return 0;
}