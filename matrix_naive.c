#include "matrix.h"
#include <stdlib.h>

struct naive_priv {
    float **values;
};

#define PRIV(x) \
    ((struct naive_priv *) ((x)->priv))

static void assign(Matrix *thiz, int rows, int cols, float **data)
{
    thiz->row = rows;
    thiz->col = cols;

    thiz->priv = malloc(cols * rows * sizeof(float));

    PRIV(thiz)->values = (float **) malloc(rows * sizeof(float *));
    for (int i = 0; i < cols; i++)
        PRIV(thiz)->values[i] = (float *) malloc(cols * sizeof(float));

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            PRIV(thiz)->values[i][j] = data[i][j];
}

static const float epsilon = 1 / 10000.0;

static bool equal(const Matrix *l, const Matrix *r)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (PRIV(l)->values[i][j] + epsilon < PRIV(r)->values[i][j] ||
                    PRIV(r)->values[i][j] + epsilon < PRIV(l)->values[i][j])
                return false;
    return true;
}

bool mul(Matrix *dst, const Matrix *l, const Matrix *r)
{
    if (l->col != r->row) {
        printf("Invalid matrix multiplication with :");
        printf("%dx%d matrix <-> %dx%d matrix", l->row, l->col, r->row, r->col);
        return false;
    }

    dst->row = l->row;
    dst->col = r->col;

    dst->priv = malloc(dst->row * dst->col * sizeof(float));

    PRIV(dst)->values = (float **) malloc(dst->row * sizeof(float *));
    for (int i = 0; i < dst->col; i++)
        PRIV(dst)->values[i] = (float *) malloc(dst->col * sizeof(float));

    for (int i = 0; i < l->row; i++)
        for (int j = 0; j < r->col; j++)
            for (int k = 0; k < l->col; k++)
                PRIV(dst)->values[i][j] += PRIV(l)->values[i][k] *
                                           PRIV(r)->values[k][j];
    return true;
}

MatrixAlgo NaiveMatrixProvider = {
    .assign = assign,
    .equal = equal,
    .mul = mul,
};
