#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../src/nn.c"

#define EPS 1e-9

bool array_equal(float *a, float *b, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (fabs(a[i] - b[i]) >= EPS)
            return false;
    }
    return true;
}

void test_matmul()
{
#define m 3
#define k 4
#define n 2

    float a[m * k] = {
        0.0f, 1.0f, 2.0f, 3.0f,
        4.0f, 5.0f, 6.0f, 7.0f,
        8.0f, 9.0f, 10.0f, 11.0f};
    float b[k * n] = {12.0f, 13.0f,
                      14.0f, 15.0f,
                      16.0f, 17.0f,
                      18.0f, 19.0f};
    float c[m * n];
    float expected[m * n] = {100.0f, 106.0f,
                             340.0f, 362.0f,
                             580.0f, 618.0f};

    matmul(a, b, c, m, k, n);
    if (array_equal(c, expected, m * n))
    {
        printf("Ok\n");
    }
    else
    {
        printf("result: ");
        for (int i = 0; i < m * n; i++)
            printf("%f ", c[i]);
        printf("\nexpected: ");
        for (int i = 0; i < m * n; i++)
            printf("%f ", expected[i]);
        printf("\n");
    }

#undef m
#undef n
#undef k
}

void test_mlp_forward()
{
    uint8_t layer_sizes[] = {2, 5, 2};
    MLP *mlp = mlp_new(3, &layer_sizes);

    float weights[2 * 5 + 5 * 2] = {
        0.0f, 1.0f, 2.0f, 3.0f, 4.0f,
        -5.0f, 6.0f, -7.0f, 8.0f, -9.0f,

        10.0f, 11.0f,
        12.0f, 13.0f,
        14.0f, 15.0f,
        16.0f, 17.0f,
        18.0f, 19.0f};
    float biases[5 + 2] = {
        0.0f, 1.0f, 2.0f, 3.0f, 4.0f,

        5.0f, 6.0f};

    for (int i = 0; i < 2 * 5 + 5 * 2; i++)
        mlp->weights[i] = weights[i];
    for (int i = 0; i < 5 + 2; i++)
        mlp->biases[i] = biases[i];

    float *buffer_a = malloc(sizeof(float) * 5);
    float *buffer_b = malloc(sizeof(float) * 5);
    float in[] = {2.0f, 3.0f};
    float out[] = {};
    float expected[] = {785.0f, 840.0f};
    mlp_forward(mlp, in, out, buffer_a, buffer_b);

    if (array_equal(out, expected, 2))
        printf("Ok\n");
    else{
        printf("%f %f\n", out[0], out[1]);
    }
}

int main(int argc, char const *argv[])
{
    test_matmul();
    test_mlp_forward();
    return 0;
}
