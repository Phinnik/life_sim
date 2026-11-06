#include <stdio.h>
#include <stdint.h>

#define declare_field(type, name) type *name;
#define swap_field(type, name) type *name;

#define declare_SoA(name, fields) \
    typedef struct name           \
    {                             \
        fields(declare_field);    \
        uint32_t count;           \
        uint32_t capacity;        \
    } name;

#define AGENT_FIELDS(X) \
    X(float, x)         \
    X(float, y)         \
    X(int, energy)
declare_SoA(Agent, AGENT_FIELDS);

int main(int argc, char const *argv[])
{
    Agent a = (Agent){.x = {3.0f}, .y = {3.0f}, .energy = 1, .count1, .capacity = 10};

    return 0;
}
