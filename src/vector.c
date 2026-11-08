#include "vector.h"
#include <math.h>

vec2 vec2_add(vec2 a, vec2 b)
{
    return (vec2){.x = a.x + b.x, .y = a.y + b.y};
}
vec2 vec2_sub(vec2 a, vec2 b)
{
    return (vec2){.x = a.x - b.x, .y = a.y - b.y};
};
vec2 vec2_scale(vec2 v, float s)
{
    return (vec2){.x = v.x * s, .y = v.y * s};
}
vec2 vec2_negate(vec2 v)
{
    return (vec2){.x = -v.x, .y = -v.y};
};
float vec2_distance_squared(const vec2 a, const vec2 b)
{
    const float dx = a.x - b.x;
    const float dy = a.y - b.y;

    return dx * dx + dy * dy;
}
float vec2_distance(vec2 a, vec2 b)
{
    return sqrtf(vec2_distance_squared(a, b));
}
float vec2_length_squared(vec2 v)
{
    return v.x * v.x + v.y * v.y;
}
float vec2_length(vec2 v)
{
    return sqrtf(vec2_length_squared(v));
}
vec2 vec2_normalize(vec2 v)
{
    float len = vec2_length(v);

    return len > 0.0001f ? (vec2){.x = v.x / len, .y = v.y / len} : (vec2){.x = 0, .y = 0};
}
vec2 vec2_limit(vec2 v, float max)
{
    float len = vec2_length(v);
    return len > max ? vec2_scale(v, max / len) : v;
}
vec2 vec2_lerp(vec2 a, vec2 b, float t)
{
    return vec2_add(a, vec2_scale(vec2_sub(b, a), t));
}
bool vec2_equal(vec2 a, vec2 b)
{
    return (a.x == b.x) && (a.y == b.y);
}
bool vec2_approx_equal(vec2 a, vec2 b, float epsilon)
{
    return fabs(a.x - b.x) <= epsilon && fabs(a.y - b.y) <= epsilon;
}
float vec2_dot_product(vec2 a, vec2 b)
{
    return a.x * b.x + a.y * b.y;
}
vec2 vec2_perpendicular(vec2 v)
{
    return (vec2){.x = v.y, .y = -v.x};
}
float vec2_angle(vec2 a, vec2 b)
{
    return acosf(vec2_dot_product(a, b) / (vec2_length(a) * vec2_length(b)));
}
vec2 vec2_rotate(vec2 v, float radians)
{
    float cos = cosf(radians);
    float sin = sinf(radians);
    return (vec2){
        .x = v.x * cos - v.y * sin,
        .y = v.x * sin + v.y * cos,
    };
}
