#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

typedef struct edge_t{
    int in_cnt;
    int out_cnt;
    int* p_cost;

    int* p_best;

    struct edge_t* prev;
    struct edge_t* next;
}edge_t;

typedef struct {
    edge_t* head;
    edge_t* tail;
}path_t;

int optimization(void* state, int i)
{
    edge_t* edge = (edge_t*)state;
    if(edge)
    {
        printf("optimization:%d\n", edge->p_cost[i * edge->out_cnt + edge->p_best[i]]);
        return edge->p_cost[i * edge->out_cnt + edge->p_best[i]];
    }
    return 0;
}

int cost(void* state, int i, int o)
{
    edge_t* edge = (edge_t*)state;
    printf("i:%d, edge->in_cnt:%d, o:%d\n", i, edge->out_cnt, o);
    printf("const[%d]:%d\n", i * edge->out_cnt + o, edge->p_cost[i * edge->out_cnt + o]);
    if(edge->p_cost[i * edge->out_cnt + o] == 0)
        return -1;
    return edge->p_cost[i * edge->out_cnt + o] + optimization(edge->next, o);
}

int best(void* state, int i, int* best_cost)
{
    edge_t* edge = (edge_t*)state;
    int init = 0;
    int best_o = 0;
    for(int o = 0; o < edge->out_cnt; o++)
    {
        int cst = cost(state, i, o);
        printf("const:%d\n", cst);
        if(cst > 0 && init == 0)
        {
            *best_cost = cst;
            best_o = o;
            init = 1;
            continue;
        }
            
        if(cst > 0 && best_i > cst)
        {
            *best_cost = cst;
            best_o = o;
        }
    }
    printf("best_o:%d\n", best_o);
    return best_o;
}

void optimize(void* state)
{
    edge_t* edge = (edge_t*)state;
    printf("edge->in_cnt:%d\n", edge->in_cnt);
    printf("edge->out_cnt:%d\n", edge->out_cnt);
    for(int i = 0; i < edge->in_cnt; i++)
    {
        printf("in:%d\n", i);
        int cost;
        int best_o = best(state, i, &cost);
        for(int o = 0; o < edge->out_cnt; o++)
        {
            if(best_o == o)
                edge->p_cost[i * edge->out_cnt + o] = cost;
            else
                edge->p_cost[i * edge->out_cnt + o] = 0;
        }
        
        edge->p_best[i] = best_o;
    }
}


//gcc 
int main()
{
    char* filename = "./input.txt";

    path_t* path = (path_t*)calloc(1, sizeof(path_t));

    int ret = access(filename, F_OK);
    if(ret < 0)
        return -1;
    FILE* fp = fopen(filename, "r");
    if(fp == NULL)
        return -1;

#define STR_BUF_LEN 1024
    char line[STR_BUF_LEN];
    memset(line, 0, sizeof(line));
    int row,col,index;
    edge_t* ptr = path->head;

    while(fgets(line, sizeof(line), fp))
    {
        char* p;
        char* pp;
        if((p = strchr(line, '\n')) == NULL)
            p = line + strlen(line);
        if(p > line && p[-1] == '\r')
            p--;
        *p = '\0';

        if((p = strchr(line, '>')) != NULL)
        {
            sscanf(p+1,"%dx%d", &row, &col);
            printf(">%dx%d:\n", row, col);
            edge_t* entry = (edge_t*) calloc(1, sizeof(edge_t));
            entry->p_cost = (int*)calloc(row*col, sizeof(int));
            entry->p_best = (int*)calloc(row, sizeof(int));
            entry->in_cnt = row;
            entry->out_cnt = col;
            index = 0;

            while(ptr && ptr->next != NULL)
                ptr = ptr->next;
            if(ptr == NULL)
            {
                path->head = entry;
                ptr = path->head;
            }
            else
            {
                ptr->next = entry;
                entry->prev = ptr;
                ptr = entry;
            }
            path->tail = ptr;
            continue;
        }
        
        char tmp[32];
        p = line;
        while ((pp = strchr(p, ' ')) != NULL)
        {
            if(pp > p)
            {
                memset(tmp, 0, sizeof(tmp));
                strncpy(tmp, p, pp - p);
                ptr->p_cost[index++] = atoi(tmp);
                printf("%s,", tmp);
                p = pp + 1;
            }
        }
        if(*p)
        {
            ptr->p_cost[index++] = atoi(p);
            printf("%s,", p);
        }
        printf("\n");
    }


    edge_t* p = path->tail;
    while(p)
    {
        static int i = 0;
        printf("\ni = %d\n", i++);
        optimize(p);
        p = p->prev;
    }

    //print best strategy
    //int toltal_cost = 0;
    int best_i;
    p = path->head;
    printf("\nbest strategy:");
    while(p)
    {
        if(p == path->head)
            best_i = 0;
        //toltal_cost += p->p_cost[best_i * p->out_cnt + p->p_best[best_i]];
        //printf("->%d", toltal_cost);
        best_i = p->p_best[best_i];
        printf("->%d", best_i);
        p = p->next;;
    }
    printf(", toltal_cost:%d\n", path->head->p_cost[path->head->p_best[0]]);
}
