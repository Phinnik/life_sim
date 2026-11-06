#ifndef AGENT_H
#define AGENT_H

#define AGENT_SPAWN_ENERGY 100.0f
#define AGENT_MAX_ENERGY 100

#define AGENT_FIELDS(X) \
    X(float, x)         \
    X(float, y)         \
    X(int, energy)      \
    X(float, brain)

typedef struct AgentsSoA
{
#define declare_attribute(type, name) type *name;
    AGENT_FIELDS(declare_attribute)
#undef declare_attribute
    int count;
    int capacity;
} AgentsSoA;

AgentsSoA *agents_create(int count);
void agents_destroy(AgentsSoA *agents);
int spawn_new_agent(AgentsSoA *agents, float x, float y);
int swap_agents(AgentsSoA *agents, int a, int b);
int kill_agent(AgentsSoA *agents, int idx);
void copy_agent_brain(AgentsSoA *agents, int parent_idx, int child_idx);

#endif // #ifndef AGENT_H
