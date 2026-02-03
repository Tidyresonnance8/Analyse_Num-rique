#include <stdio.h>
#include <stdlib.h>
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
    matrix *A = allocate_matrix(3,2);
    matrix *B = allocate_matrix(2,3);
    matrix *C = allocate_matrix(2,2);

    for(int i=0; i<6; i++)  A->data[i] = i+1;
    for(int i=0; i<6; i++)  B->data[i] = 1.0;

    if (mult_matrix(A, B, C) == 0) {
        printf("Résultat C[0][0] : %f (attendu: 6.0)\n", C->a[0][0]);
        printf("Résultat C[1][0] : %f (attendu: 15.0)\n", C->a[1][0]);
    } else {
        printf("Erreur de dimensions !\n");
    }
    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
    return 0;
}