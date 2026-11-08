#ifndef NN_H
#define NN_H

#include <stdint.h>
#include "random.h"

typedef struct
{
    uint8_t n_layers;
    uint8_t *layer_sizes;
    float *weights;
    float *biases;
} MLP;

MLP *mlp_new(int n_layers, uint8_t *layer_sizes);
void mlp_free(MLP *mlp);
void mlp_init_xavier_normal(MLP *mlp, RandomGenerator *rng);
void mlp_forward(MLP *mlp, float *in, float *out, float *buffer_a, float *buffer_b);

#endif // #ifndef NN_H