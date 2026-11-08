#include "vector.h"

inline float vec2_distance_squared(const vec2 a, const vec2 b)
{
    const float dx = a.x - b.x;
    const float dy = a.y - b.y;

    return dx * dx + dy * dy;
}

inline float vec3_distance_squared(const vec3 a, const vec3 b)
{
    const float dx = a.x - b.x;
    const float dy = a.y - b.y;
    const float dz = a.z - b.z;

    return dx * dx + dy * dy + dz * dz;
}
