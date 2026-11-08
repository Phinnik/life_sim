#include <stdlib.h>
#include <stddef.h>

#include "simulation.h"
#include "../../random.h"

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
    uint32_t *seed)
{
    BoidsSimulation *sim = malloc(sizeof(BoidsSimulation));
    if (!sim)
        return NULL;
    sim->boids = malloc(sizeof(Boid) * num_boids);
    if (!sim->boids)
    {
        free(sim);
        return NULL;
    }

    for (size_t i = 0; i < num_boids; i++)
    {
        sim->boids[i].pos.x = (float)width / 2 + (rand_float(seed) - 0.5f) * 100;
        sim->boids[i].pos.y = (float)height / 2 + (rand_float(seed) - 0.5f) * 100;
        sim->boids[i].prev_pos.x = sim->boids[i].pos.x;
        sim->boids[i].prev_pos.y = sim->boids[i].pos.y;

        sim->boids[i].velocity.x = (rand_float(seed) - 0.5f) * max_speed * 2;
        sim->boids[i].velocity.y = (rand_float(seed) - 0.5f) * max_speed * 2;
        sim->boids[i].prev_velocity.x = sim->boids[i].velocity.x;
        sim->boids[i].prev_velocity.y = sim->boids[i].velocity.y;

        sim->boids[i].trail = malloc(sizeof(vec2) * trail_length);
        if (!sim->boids[i].trail)
            // TODO: deallocate all boids
            return NULL;
        for (size_t j = 0; j < trail_length; j++)
            sim->boids[i].trail[j] = sim->boids[i].prev_pos;
    }

    sim->num_boids = num_boids;
    sim->height = height;
    sim->width = width;
    sim->max_speed = max_speed;
    sim->max_force = max_force;
    sim->perception_squared = perception * perception;
    sim->separation_weight = separation_weight;
    sim->alignment_weight = alignment_weight;
    sim->cohesion_weight = cohesion_weight;
    sim->trail_length = trail_length;
    return sim;
}

void boids_free(BoidsSimulation *sim)
{
    free(sim->boids);
    free(sim);
}

// The force needed to change current velocity to desired velocity
static inline vec2 steer(vec2 current_velocity, vec2 desired_velocity, float max_force)
{
    return vec2_limit(vec2_sub(desired_velocity, current_velocity), max_force);
}

static void boid_update(BoidsSimulation *sim, int idx)
{
    Boid self = sim->boids[idx];
    vec2 separation_equilibrium = {0.0f, 0.0f};
    vec2 avg_alignment = {0.0f, 0.0f};
    vec2 center_mass = {0.0f, 0.0f};
    int neighbours_cnt = 0;

    for (size_t i = 0; i < sim->num_boids; i++)
    {
        if (i == idx)
            continue;

        float distance_squared = vec2_distance_squared(self.prev_pos, sim->boids[i].prev_pos);
        if (distance_squared <= sim->perception_squared)
        {
            if (distance_squared > 0.0001f)
            {
                separation_equilibrium = vec2_add(separation_equilibrium,
                                                  vec2_scale(
                                                      vec2_sub(self.prev_pos, sim->boids[i].prev_pos),
                                                      1 / distance_squared));
            }
            avg_alignment = vec2_add(avg_alignment, sim->boids[i].prev_velocity);
            center_mass = vec2_add(center_mass, sim->boids[i].prev_pos);
            neighbours_cnt++;
        }
    }
    vec2 acceleration = {0.0f, 0.0f};
    if (neighbours_cnt > 0)
    {
        center_mass = vec2_scale(center_mass, 1.0f / neighbours_cnt);

        vec2 desired_velocity;

        desired_velocity = vec2_scale(vec2_normalize(separation_equilibrium), sim->max_speed);
        vec2 separation_acceleration = steer(self.prev_velocity, desired_velocity, sim->max_force);

        desired_velocity = vec2_scale(vec2_normalize(avg_alignment), sim->max_speed);
        vec2 alignment_acceleration = steer(self.prev_velocity, desired_velocity, sim->max_force);

        desired_velocity = vec2_scale(vec2_normalize(vec2_sub(center_mass, self.prev_pos)), sim->max_speed);
        vec2 cohesion_acceleration = steer(self.prev_velocity, desired_velocity, sim->max_force);

        acceleration = vec2_add(acceleration, vec2_scale(separation_acceleration, sim->separation_weight));
        acceleration = vec2_add(acceleration, vec2_scale(alignment_acceleration, sim->alignment_weight));
        acceleration = vec2_add(acceleration, vec2_scale(cohesion_acceleration, sim->cohesion_weight));
    }
    acceleration = vec2_limit(acceleration, sim->max_force);

    self.velocity = vec2_limit(vec2_add(self.prev_velocity, acceleration), sim->max_speed);

    sim->boids[idx].velocity = self.velocity;
    sim->boids[idx].pos = vec2_add(self.prev_pos, self.velocity);

    // Rotate trail
    for (size_t i = 1; i < sim->trail_length; i++)
        sim->boids[idx].trail[i-1] = sim->boids[idx].trail[i];
    sim->boids[idx].trail[sim->trail_length-1] = self.prev_pos;

    if (sim->boids[idx].pos.x < 0)
        sim->boids[idx].pos.x += sim->width;
    else if (sim->boids[idx].pos.x >= sim->width)
        sim->boids[idx].pos.x -= sim->width;

    if (sim->boids[idx].pos.y < 0)
        sim->boids[idx].pos.y += sim->height;
    else if (sim->boids[idx].pos.y >= sim->height)
        sim->boids[idx].pos.y -= sim->height;
}

void boids_step(BoidsSimulation *sim)
{
    for (size_t i = 0; i < sim->num_boids; i++)
    {
        sim->boids[i].prev_pos = sim->boids[i].pos;
        sim->boids[i].prev_velocity = sim->boids[i].velocity;
    }
    for (size_t i = 0; i < sim->num_boids; i++)
    {
        boid_update(sim, i);
    }
}
