#ifndef BOIDS_SIMULATION_H
#define BOIDS_SIMULATION_H

#include "../../vector.h"
#include <stdint.h>


typedef struct
{
    vec2 pos;
    vec2 velocity;
    vec2 prev_pos;
    vec2* trail;
    vec2 prev_velocity;
} Boid;

typedef struct
{
    Boid *boids;
    int num_boids;
    float height;
    float width;
    float max_speed;
    float max_force;
    float perception_squared;
    float separation_weight;
    float alignment_weight;
    float cohesion_weight;
    int trail_length;
} BoidsSimulation;

BoidsSimulation *boids_new(
    int num_boids,
    float height,
    float width,
    float max_speed,
    float max_force,
    float perception,
    float separation_weight,
    float alignment_weight,
    float cohesion_weight,
    int trail_length,
    uint32_t *seed
);
void boids_free(BoidsSimulation *sim);
void boids_step(BoidsSimulation *sim);


#endif // #ifndef BOIDS_SIMULATION_H
