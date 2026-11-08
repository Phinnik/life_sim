#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
typedef struct
{
    float x, y;
} vec2;

typedef struct
{
    float x, y, z;
} vec3;

inline vec2 vec2_add(vec2 a, vec2 b);
inline vec2 vec2_sub(vec2 a, vec2 b);
inline vec2 vec2_scale(vec2 v, float s);
inline vec2 vec2_negate(vec2 v);
inline float vec2_distance_squared(const vec2 a, const vec2 b);
inline float vec2_distance(vec2 a, vec2 b);
inline float vec2_length_squared(vec2 v);
inline float vec2_length(vec2 v);
inline vec2 vec2_normalize(vec2 v);
inline vec2 vec2_limit(vec2 v, float max);
inline vec2 vec2_lerp(vec2 a, vec2 b, float t);
inline bool vec2_equal(vec2 a, vec2 b);
inline bool vec2_approx_equal(vec2 a, vec2 b, float epsilon);
inline float vec2_dot_product(vec2 a, vec2 b);
inline vec2 vec2_perpendicular(vec2 v);
inline float vec2_angle(vec2 a, vec2 b);
inline vec2 vec2_rotate(vec2 v, float radians);

#endif // #ifndef VECTOR_H
