#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint32_t state;
    bool has_spare;
    float spare;
} RandomGenerator;

uint32_t rand_uint32(uint32_t *seed);
float rand_float(uint32_t *seed);
float rand_normal(RandomGenerator *rng);
void xavier_normal_init(RandomGenerator *rng, float *w, int fan_in, int fan_out);

#endif // #ifndef RANDOM_H
