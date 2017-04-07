#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TEST1_ROW 8
#define TEST1_COL 8
#define TEST2_ROW 8
#define TEST2_COL 8

MatrixAlgo *matrix_providers[] = {
    &NaiveMatrixProvider,
};

float **alloc(int row, int col)
{
    float **result;
    if (!(result = (float **) malloc(row * sizeof(float *))))
        return NULL;

    for (int i = 0; i < col; i++)
        if (!(result[i] = (float *) malloc(col * sizeof(float)))) {
            free(result);
            return NULL;
        }
    return result;
}

void free_space(float ***target, int col)
{
    for (int i = 0; i < col; i++)
        free((*target)[i]);
    free(**target);
    **target = NULL;
}

float randFloat(float range)
{
    return ((float)rand() / (float) (RAND_MAX)) * range;
}

int main()
{
    float **mat1, **mat2, **ans;

    /* Dynamic allocation */
    mat1 = alloc(TEST1_ROW, TEST1_COL);
    mat2 = alloc(TEST2_ROW, TEST2_COL);
    ans = alloc(TEST1_ROW, TEST2_COL);
    if (!(mat1) || !(mat2) || !(ans)) {
        printf("allocation error.\n");
        return -1;
    }

    /* Initialize */
    srand((unsigned int) time(NULL));
    float cst = 10.0;   // const for random float
    for (int i = 0; i < TEST1_ROW; i++)
        for (int j = 0; j < TEST1_COL; j++)
            mat1[i][j] = randFloat(cst);

    for (int i = 0; i < TEST2_ROW; i++)
        for (int j = 0; j < TEST2_COL; j++)
            mat2[i][j] = randFloat(cst);

    for (int i = 0; i < TEST1_ROW; i++)
        for (int j = 0; j < TEST2_COL; j++)
            ans[i][j] = 0.0;

    if (TEST1_COL != TEST2_ROW) {
        printf("INVALID : mat1's cols NOT equal mat2's rows.\n");
        free_space(&mat1, TEST1_COL);
        free_space(&mat2, TEST2_COL);
        free_space(&ans, TEST2_COL);
        return -1;
    }

    /* Calculate the answer */
    for (int i = 0; i < TEST1_ROW; i++)
        for (int j = 0; j < TEST2_COL; j++)
            for (int k = 0; k < TEST1_COL; k++)
                ans[i][j] += mat1[i][k] * mat2[k][j];

    /* Start testing */
    MatrixAlgo *algo = matrix_providers[0];
    Matrix dst, m, n, fixed;

    algo->assign(&m, TEST1_ROW, TEST1_COL, mat1);
    algo->assign(&n, TEST1_ROW, TEST2_COL, mat2);

    algo->mul(&dst, &m, &n);

    algo->assign(&fixed, 4, 4, ans);

    free_space(&mat1, TEST1_COL);
    free_space(&mat2, TEST2_COL);
    free_space(&ans, TEST2_COL);

    if (algo->equal(&dst, &fixed))
        return 0;
    return -1;
}
