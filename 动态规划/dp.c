#include <stdio.h>

int optimization(void* state)
{

}

int cost(void* state)
{

}

int best(void* results)
{

}

int optimize(void* state, void* opt_result, void* opt_strategy)
{
    void* sub_state;
    void* last_state;

    best(cost(sub_state) + optimization(last_state));
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
        optimize(p,  p, p);
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