#include <stdlib.h>
#include "nn.h"
#include "random.h"

MLP *mlp_new(int n_layers, uint8_t *layer_sizes)
{
    MLP *mlp = (MLP *)malloc(sizeof(MLP));
    if (!mlp)
        return NULL;
    mlp->n_layers = n_layers;
    mlp->layer_sizes = layer_sizes;

    int n_weights = 0;
    int n_biases = 0;

    for (int i = 1; i < n_layers; i++)
    {
        n_biases += layer_sizes[i];
        n_weights += layer_sizes[i - 1] * layer_sizes[i];
    }

    mlp->weights = (float *)malloc(sizeof(float) * n_weights);
    mlp->biases = (float *)malloc(sizeof(float) * n_biases);
    if (!mlp->weights || !mlp->biases)
    {
        free(mlp->weights);
        free(mlp->biases);
        free(mlp);
        return NULL;
    }
    return mlp;
};
void mlp_free(MLP *mlp)
{
    free(mlp->weights);
    free(mlp->biases);
    free(mlp);
};

void mlp_init_xavier_normal(MLP *mlp, RandomGenerator *rng)
{
    int offset = 0;
    for (int i = 1; i < mlp->n_layers; i++)
    {
        int fan_in = mlp->layer_sizes[i - 1];
        int fan_out = mlp->layer_sizes[i];
        xavier_normal_init(rng, mlp->weights + offset, fan_in, fan_out);
        offset += fan_in * fan_out;
    }
}

// TODO: optimize cached memory access (https://michalpitr.substack.com/p/optimizing-matrix-multiplication)
static void matmul(float *restrict a, float *restrict b, float *restrict c, int m, int k, int n)
{
    for (int m_ = 0; m_ < m; m_++)
    {
        for (int n_ = 0; n_ < n; n_++)
        {
            float sum = 0;
            for (int k_ = 0; k_ < k; k_++)
            {
                sum += a[m_ * k + k_] * b[k_ * n + n_];
            }
            c[m_ * n + n_] = sum;
        }
    }
}

void mlp_forward(MLP *mlp, float *in, float *out, float *buffer_a, float *buffer_b)
{
    int w_offset = 0;
    int b_offset = 0;
    float *layer_in = in;
    for (int i = 1; i < mlp->n_layers; i++)
    {
        float *layer_out = (i == mlp->n_layers - 1) ? out : (i % 2 == 0 ? buffer_a : buffer_b);
        matmul(layer_in, mlp->weights + w_offset, layer_out, 1, mlp->layer_sizes[i - 1], mlp->layer_sizes[i]);
        for (int j = 0; j < mlp->layer_sizes[i]; j++)
            layer_out[j] += mlp->biases[b_offset + j];

        // RELU activation
        if (i != mlp->n_layers - 1)
        {
            for (int j = 0; j < mlp->layer_sizes[i]; j++)
                layer_out[j] = layer_out[j] > 0 ? layer_out[j] : 0;
        }

        w_offset += mlp->layer_sizes[i - 1] * mlp->layer_sizes[i];
        b_offset += mlp->layer_sizes[i];
        layer_in = layer_out;
    }
}
