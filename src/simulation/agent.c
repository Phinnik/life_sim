#include <stdlib.h>
#include "agent.h"

AgentsSoA *agents_create(int count)
{
    if (count < 0)
        return NULL;
    AgentsSoA *agents = calloc(1, sizeof(*agents));
    if (!agents)
        return NULL;

#define alloc_field(type, name)                  \
    agents->name = malloc(sizeof(type) * count); \
    if (!agents->name)                           \
        goto fail;
    AGENT_FIELDS(alloc_field)
#undef alloc_field

    agents->count = 0;
    agents->capacity = count;
    return agents;

fail:
#define free_field(type, name) free(agents->name);
    AGENT_FIELDS(free_field)
#undef free_field
    return NULL;
}

void agents_destroy(AgentsSoA *agents)
{
#define free_field(type, name) free(agents->name);
    AGENT_FIELDS(free_field)
#undef free_field
    free(agents);
}

int spawn_new_agent(AgentsSoA *agents, float x, float y)
{

#ifdef FOOLPROOFING
    if (agents->count + 1 > agents->capacity)
        return -1;
#endif

    int idx = agents->count++;
    agents->x[idx] = x;
    agents->y[idx] = y;
    agents->energy[idx] = AGENT_SPAWN_ENERGY;
    // TODO: implement brain init
    // agents->brain = ;
    return idx;
};

int swap_agents(AgentsSoA *agents, int a, int b)
{
#ifdef FOOLPROOFING
    if (a >= agents->count || b >= agents->count || a < 0 || b < 0 || a == b)
        return 0;
#endif

#define swap_fields(type, name)        \
    type tmp_##name = agents->name[a]; \
    agents->name[a] = agents->name[b]; \
    agents->name[b] = tmp_##name;

    AGENT_FIELDS(swap_fields)

#undef swap_fields

    return 1;
};

int kill_agent(AgentsSoA *agents, int idx)
{
#ifdef FOOLPROOFING
    if (idx >= agents->count || idx < 0)
        return 0;
#endif

    swap_agents(agents, idx, agents->count - 1);
    agents->count--;
    return 1;
};

void copy_agent_brain(AgentsSoA *agents, int parent_idx, int child_idx) {
    // TODO: implement
};
