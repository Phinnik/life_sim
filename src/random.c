#include <math.h>
#include "random.h"

// === Random number generators ===============================================

/*
Xorshift32
*/
uint32_t rand_uint32(uint32_t *seed)
{
    uint32_t x = *seed;

    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;

    *seed = x;
    return x;
};

/*
shifting right by 8 bits produces integers from [0, 2^(32-8)] -> [0, 16777216]
dividing it by `16777216` normalizes result from [0, 1]
*/
float rand_float(uint32_t *seed)
{
    return (rand_uint32(seed) >> 8) / 16777216.0f;
};

/*
Box–Muller Transform
*/
float rand_normal(RandomGenerator *rng)
{
    if (rng->has_spare)
    {
        rng->has_spare = false;
        return rng->spare;
    }

    float u1 = rand_float(&rng->state);
    float u2 = rand_float(&rng->state);

    float r = sqrtf(-2.0f * logf(u1));
    float theta = 2.0f * M_PI * u2;

    double z0 = r * cosf(theta);
    double z1 = r * sinf(theta);

    rng->spare = z1;
    rng->has_spare = true;

    return z0;
}

// === Weight initialization ==================================================

void xavier_normal_init(RandomGenerator *rng, float *w, int fan_in, int fan_out)
{
    float stddev = sqrtf(2.0f / (fan_in + fan_out));
    for (int i = 0; i < fan_in * fan_out; i++)
        w[i] = rand_normal(rng) * stddev;
}

void he_init(RandomGenerator *rng, float *w, int fan_in, int fan_out)
{
    float stddev = sqrtf(2.0f / fan_in);
    for (int i = 0; i < fan_in * fan_out; i++)
        w[i] = rand_normal(rng) * stddev;
}
