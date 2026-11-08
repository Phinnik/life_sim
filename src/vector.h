#ifndef VECTOR_H
#define VECTOR_H

typedef struct
{
    float x, y;
} vec2;

typedef struct
{
    float x, y, z;
} vec3;

inline float vec2_distance_squared(const vec2 a, const vec2 b);
inline float vec3_distance_squared(const vec3 a, const vec3 b);

#endif // #ifndef VECTOR_H
