#include <stdio.h>

int optimization(void* state, int i)
{
    edge_t* edge = (edge_t*)state;
    return edge->p_cost[i][edge->p_best[i]];
}

int cost(void* state, int i, int o)
{
    edge_t* edge = (edge_t*)state;
    if(edge->p_cost[i][o] == 0)
        return -1;
    return edge->p_cost[i][o] + optimization(edge->next, o);
}

int best(void* state, int i)
{
    edge_t* edge = (edge_t*)state;
    int init = 0;
    int best_o = 0;
    for(int o = 0; o < edge->out_cnt; o++)
    {
        int cost = cost(state, i, o);
        if(cost > 0 && init == 0)
        {
            best = cost;
            init = 1;
            continue;
        }
            
        if(best > cost)
        {
            best = cost;
            best_o = o;
        }
    }
    return best_o;
}

void optimize(void* state)
{
    edge_t* edge = (edge_t*)state;
    for(int i = 0; i < edge->in_cnt; i++)
    {
        int best_o = best(state, i);
        for(int o = 0; o < edge->out_cnt; o++)
        {
            if(best_o == o)
                continue;
            edge->p_cost[i][o] = 0;
        }
        edge->p_best[i] = best_o;
    }
}


typedef struct edge_t{
    int in_cnt;
    int out_cnt;
    void* p_cost;

    int* p_best;

    struct edge_t* prev;
    struct edge_t* next;
}edge_t;

typedef struct {
    edge_t* head;
    edge_t* tail;
}path_t;

int main()
{
    char* filename = "./input.txt";

    path_t* path = (path_t*)calloc(1, sizeof(path_t));

    edge_t* p = path->tail;
    while(p)
    {
        optimize(p);
        p = p->prev;
    }

    //print best strategy
    int toltal_cost = 0;
    int best_i;
    p = path->head;
    printf("best strategy:");
    while(p)
    {
        if(p == path->head)
            best_i = 0;
        toltal_cost += p->p_cost[best_i][p->p_best[best_i]];
        best_i = p->p_best[best_i];
        printf(" %d", best_i)
        p = p->next;;
    }
    printf("toltal_cost:%d\n", toltal_cost);
}