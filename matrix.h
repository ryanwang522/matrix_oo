#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    int row, col;
    void *priv;
} Matrix;

typedef struct {
    void (*assign)(Matrix *thiz, int, int, float **);
    bool (*equal)(const Matrix *l, const Matrix *r);
    bool (*mul)(Matrix *dst, const Matrix *l, const Matrix *r);
} MatrixAlgo;

/* Available matrix providers */
extern MatrixAlgo NaiveMatrixProvider;

#endif /* MATRIX_H_ */
