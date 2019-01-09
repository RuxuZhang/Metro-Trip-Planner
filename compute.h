//
// Created by Yingqiao Zheng on 11/12/18.
//

#ifndef CSE_6010_PROJ_COMPUTE_H
#define CSE_6010_PROJ_COMPUTE_H

#include <stdbool.h>

/////////////////////////////////////////////////////////////////
// Unweighted adjacency list
// Used for calculating minimum number of transfer needed route
////////////////////////////////////////////////////////////////

typedef struct node{
    int edge;
    struct node *nextEdge;
}nWEdgeNode;

typedef struct{
    int vertex;
    nWEdgeNode *firstEdge;
}nWVertexNode;

nWVertexNode** malloc_unweighted_vertex_node(int size);
void print_unweighted_adjList(nWVertexNode **adjList, int vertexNum);
void free_unweighted_adjList(nWVertexNode **adjList, int vertexNum);
void add_unweighted_edge(nWVertexNode *vertexEdges, int nodeID);
void add_unweighted_undirected_edge(nWVertexNode **adjlist, int u, int v);

/////////////////////////////////////////////////////////////////
// Weighted adjacency list
// Used for calculating minimum number of stations passed route
// Used for calculating minimum number of time needed route
/////////////////////////////////////////////////////////////////
typedef struct Wnode{
    int edge;
    int weight;
    struct Wnode *nextEdge;
}WeightedEdgeNode;

typedef struct{
    int vertex;
    WeightedEdgeNode *firstEdge;
}WeightedVertexNode;

WeightedVertexNode** malloc_weighted_vertex_node(int size);
void print_weighted_adjList(WeightedVertexNode **adjList, int vertexNum);
void free_weighted_adjList(WeightedVertexNode **adjList, int vertexNum);
void add_weighted_edge(WeightedVertexNode *vertexEdges, int nodeID, int weight);
void add_weighted_undirected_edge(WeightedVertexNode **adjlist, int u, int v, int weight);

//////////////////////////////////////////////////////////////
// FIFO queue
// Used for BFS
///////////////////////////////////////////////////////////////
typedef struct qnode {
    int data;
    struct qnode *next;
}qNode;

typedef struct {
    qNode *front;
    qNode *rear;
}LinkQueue;

bool InitLinkQueue(LinkQueue *pLQ);
bool IsQueueEmpty(LinkQueue *pLQ);
bool EnQueue(LinkQueue *pLQ, int data);
int  DeQueue(LinkQueue *pLQ);
int  getQueueSize(LinkQueue *pLQ);
void printQueue(LinkQueue *pLQ);
void clearQueue(LinkQueue *pLQ);

/////////////////////////////////////////////////
// Priority Queue Used in Dijkstra
// key should be unique
/////////////////////////////////////////////////
typedef struct pQnode{
    int key;
    int priority;
    struct pQnode *next;
}pQNode;

typedef struct {
    pQNode *front;
    pQNode *rear;
}PrioQueue;

pQNode* newPQNode(int d, int p);
bool InitPQ(PrioQueue *pQ);
bool IsPQEmpty(PrioQueue *pQ);
int  maxPrioData(PrioQueue *pQ);
int  pQpop(PrioQueue *pQ);
int  getPrioQueueSize(PrioQueue *pQ);
int  findPrio(PrioQueue *pQ, int d);
void pQpush(PrioQueue *pQ, int d, int p);
void printPrioQueue(PrioQueue *pQ);
void clearPrioQueue(PrioQueue *pQ);
void changePrio(PrioQueue *pQ, int key, int newP);

/////////////////////////////////////////////////
// Data structure to store info from BFS
////////////////////////////////////////////////
typedef struct {
    bool visited;
    int distance;
    int pre;
}BFSinfo;

LinkQueue* BFS_return_path(nWVertexNode **adjList, int src, int dest, int ListSize);

//////////////////////////////////////////////////
// Data structure to store info from Dijkstra
//////////////////////////////////////////////////
typedef struct {
    int distance;
    int pre;
}Dinfo;

LinkQueue* Dijkstra_return_path_weight(WeightedVertexNode **Wadjlist, int src, int dest, int size);

////////////////////////////////////////////////
// build test data
////////////////////////////////////////////////
nWVertexNode** build_gz_line_adjlist(int *size);
WeightedVertexNode** build_gz_transfer_stationNum(int *size);

#endif //CSE_6010_PROJ_COMPUTE_H
