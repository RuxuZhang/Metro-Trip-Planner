//
// Created by Yingqiao Zheng on 11/12/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>
#include "compute.h"

////////////////////////////////////////////////
// Unweighted adjList function
// 1. print adjlist
// 2. free memory allocated for adjlist
// 3. add unweighted edge to vertex edge
////////////////////////////////////////////////

nWVertexNode** malloc_unweighted_vertex_node(int size) {
    if (size == 0) { perror("No need to malloc with size 0!\n"); return NULL; }
    nWVertexNode **adjList;
    adjList = (nWVertexNode**)malloc((size) * sizeof(nWVertexNode*));
    if (adjList == NULL) { perror("Memory allocation for vertex edges failed! Abort.\n"); exit(1); }
    for (int num = 0; num < (size); ++num) {
        adjList[num] = (nWVertexNode*)malloc(sizeof(nWVertexNode));
        adjList[num] -> vertex = num;
        adjList[num] -> firstEdge = NULL;
    }
    return adjList;
}

void print_unweighted_adjList(nWVertexNode **adjList, int vertexNum) {
    if (adjList == NULL) { printf("\nEmpty Input Unweighted Adjacency List!\n"); return; }
    for (int i = 0; i < vertexNum; ++i) {
        printf("%d | ", adjList[i] -> vertex);
        if (adjList[i] -> firstEdge != NULL) {
            nWEdgeNode *tmpPtr = adjList[i] -> firstEdge;
            while (tmpPtr != NULL) {
                printf("%d ", tmpPtr -> edge);
                tmpPtr = tmpPtr -> nextEdge;
            }
        }
        printf("\n");
    }
}

void free_unweighted_adjList(nWVertexNode **adjList, int vertexNum) {
    if (adjList == NULL) { printf("\nEmpty Input Adjacency List!\n"); return; }
    for (int i = 0; i < vertexNum; ++i) {
        if (adjList[i] -> firstEdge != NULL) {
            nWEdgeNode *tmpPtr = adjList[i] -> firstEdge;
            free(adjList[i]);
            while (tmpPtr != NULL) {
                tmpPtr = tmpPtr -> nextEdge;
            }
        }
        else free(adjList[i]);
    }
    free(adjList);
}

/* add an unweighted DIRECTED edge */
void add_unweighted_edge(nWVertexNode *vertexEdges, int nodeID) {
    if (vertexEdges == NULL) { perror("Add edge failed: empty vertex!\n"); return; }
    if (nodeID < 0) { perror("Unexpected node ID! (Expected positive integer.)\n"); return; }
    /* Allocate and set nextEdge */
    nWEdgeNode *tmpEdge = malloc(sizeof(nWEdgeNode));
    if (tmpEdge == NULL) { perror("\nMemory allocation for edge node failed. Abort.\n"); exit(1); }
    tmpEdge -> edge = nodeID;
    tmpEdge -> nextEdge = NULL;

    /* Add edge behind the first vertex */
    if (vertexEdges -> firstEdge == NULL) {
        vertexEdges -> firstEdge = tmpEdge;
    }
        /* Add edge behind other edges */
    else {
        nWEdgeNode *tmpPtr = vertexEdges -> firstEdge;
        while (tmpPtr -> nextEdge != NULL) {
            tmpPtr = tmpPtr -> nextEdge;
        }
        tmpPtr -> nextEdge = tmpEdge;
    }
}

void add_unweighted_undirected_edge(nWVertexNode **adjlist, int u, int v) {
    if (u == v) return;
    add_unweighted_edge(adjlist[u], v);
    add_unweighted_edge(adjlist[v], u);
}

//////////////////////////////////////////////////////
// Weighted adjacency list
/////////////////////////////////////////////////////

WeightedVertexNode** malloc_weighted_vertex_node(int size) {
    if (size == 0) { perror("No need to malloc with size 0!\n"); return NULL; }
    WeightedVertexNode **WadjList;
    WadjList = (WeightedVertexNode**)malloc((size) * sizeof(WeightedVertexNode*));
    if (WadjList == NULL) { perror("Memory allocation for vertex edges failed! Abort.\n");exit(1); }
    for (int num = 0; num < (size); ++num) {
        WadjList[num] = (WeightedVertexNode*)malloc(sizeof(WeightedVertexNode));
        WadjList[num] -> vertex = num;
        WadjList[num] -> firstEdge = NULL;
    }
    return WadjList;
}

void print_weighted_adjList(WeightedVertexNode **WadjList, int vertexNum) {
    if (WadjList == NULL) { printf("\nEmpty Input Unweighted Adjacency List!\n"); return; }
    for (int i = 0; i < vertexNum; ++i) {
        printf("%d | ", WadjList[i] -> vertex);
        if (WadjList[i] -> firstEdge != NULL) {
            WeightedEdgeNode *tmpPtr = WadjList[i] -> firstEdge;
            while (tmpPtr != NULL) {
                printf("%d W:%d, ", tmpPtr -> edge, tmpPtr->weight);
                tmpPtr = tmpPtr -> nextEdge;
            }
        }
        printf("\n");
    }
}

void free_weighted_adjList(WeightedVertexNode **WadjList, int vertexNum) {
    if (WadjList == NULL) { printf("\nEmpty Input Adjacency List!\n"); return; }
    for (int i = 0; i < vertexNum; ++i) {
        if (WadjList[i] -> firstEdge != NULL) {
            WeightedEdgeNode *tmpPtr = WadjList[i] -> firstEdge;
            free(WadjList[i]);
            while (tmpPtr != NULL) {
                tmpPtr = tmpPtr -> nextEdge;
            }
        }
        else free(WadjList[i]);
    }
    free(WadjList);
}

/* add a weighted DIRECTED edge */
void add_weighted_edge(WeightedVertexNode *vertexEdges, int nodeID, int weight) {
    if (vertexEdges == NULL) { perror("Add edge failed: empty vertex!\n"); return; }
    if (nodeID < 0) { perror("Unexpected node ID! (Expected positive integer.)\n"); return; }
    if (weight < 0) { perror("Unexpected weight! (Expected positive integer.)\n"); return; }
    /* Allocate and set nextEdge */
    WeightedEdgeNode *tmpEdge = malloc(sizeof(WeightedEdgeNode));
    if (tmpEdge == NULL) { perror("\nMemory allocation for edge node failed. Abort.\n"); exit(1); }
    tmpEdge->edge = nodeID;
    tmpEdge->weight = weight;
    tmpEdge->nextEdge = NULL;

    /* Add edge behind the first vertex */
    if (vertexEdges -> firstEdge == NULL) {
        vertexEdges -> firstEdge = tmpEdge;
    }
        /* Add edge behind other edges */
    else {
        WeightedEdgeNode *tmpPtr = vertexEdges -> firstEdge;
        while (tmpPtr -> nextEdge != NULL) {
            tmpPtr = tmpPtr -> nextEdge;
        }
        tmpPtr -> nextEdge = tmpEdge;
    }
}

void add_weighted_undirected_edge(WeightedVertexNode **Wadjlist, int u, int v, int weight) {
    if (u == v) return;
    add_weighted_edge(Wadjlist[u], v, weight);
    add_weighted_edge(Wadjlist[v], u, weight);
}

////////////////////////////////////////////////
// Queue operate function
// 1. initialize queue
// 2. check if queue is empty
// 3. push data into queue
// 4. pop data at top of the queue
// 5. print element in queue
// 6. clear all elements in queue
///////////////////////////////////////////////

bool InitLinkQueue(LinkQueue *pLQ) {
    qNode *p;
    p = (qNode*)malloc(sizeof(qNode));
    if (p == NULL) return false;
    else {
        p->next = NULL;
        pLQ->front = p;
        pLQ->rear = p;
        return true;
    }
}

bool IsQueueEmpty(LinkQueue *pLQ) { return pLQ->front == pLQ->rear; }

bool EnQueue(LinkQueue *pLQ, int data) {
    qNode *p;
    p = (qNode*)malloc(sizeof(qNode));
    if (p == NULL) return false;

    p->data = data;
    p->next = NULL;
    pLQ->rear->next = p;
    pLQ->rear = p;
    return true;
}

// return the deleted data in link queue
int DeQueue(LinkQueue *pLQ) {
    qNode *p;
    if (IsQueueEmpty(pLQ)) { perror("Error DeQueue: Empty Queue\n"); exit(1); }
    int data;
    p = pLQ->front->next;
    data = p->data;
    pLQ->front->next = p->next;
    if(pLQ->front->next == NULL) pLQ->rear = pLQ->front;
    free(p);
    return data;
}

void printQueue(LinkQueue *pLQ) {
    if (IsQueueEmpty(pLQ)) { printf("Empty queue!\n"); return; }
    qNode *p;
    p = pLQ->front->next;
    while (p != NULL) {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}

// clear all elements in queue
void clearQueue(LinkQueue *pLQ) {
    if (IsQueueEmpty(pLQ)) return;
    while (!IsQueueEmpty(pLQ)) DeQueue(pLQ);
}

// return the number of elements in link queue
int getQueueSize(LinkQueue *pLQ) {
    if (pLQ->front == pLQ->rear) return 0;
    int count = 0;
    qNode *tmp = pLQ->front;
    while (tmp != pLQ->rear) {
        count++;
        tmp = tmp->next;
    }
    return count;
}

///////////////////////////////////////////////////////
// Priority Queue
///////////////////////////////////////////////////////

pQNode* newPQNode(int d, int p) {
    pQNode *temp = (pQNode*)malloc(sizeof(pQNode));
    if (temp == NULL) { perror("Memory allocation for priority queue node failed.\n"); exit(1); }
    temp->key = d;
    temp->priority = p;
    temp->next = NULL;
    return temp;
}

// initialize priority queue:
//
bool InitPQ(PrioQueue *pQ) {
    pQNode *temp;
    temp = (pQNode*)malloc(sizeof(pQNode));
    if (temp == NULL) return false;
    else {
        temp->key = -1;
        temp->priority = -INT_MAX;
        temp->next = NULL;
        pQ->front = temp;
        pQ->rear = temp;
        return true;
    }
}

int maxPrioData(PrioQueue *pQ) { return pQ->front->next->key; }

bool IsPQEmpty(PrioQueue *pQ) { return pQ->front == pQ->rear; }

int pQpop(PrioQueue *pQ) {
    if (IsPQEmpty(pQ)) { perror("Cannot Pop due to empty priority queue.\n"); exit(1); }
    pQNode *temp = pQ->front->next;
    int data = temp->key;
    pQ->front->next = temp->next;
    if (pQ->front->next == NULL) pQ->rear = pQ->front;
    free(temp);
    pQ->rear = pQ->front;
    while (pQ->rear->next != NULL) pQ->rear = pQ->rear->next;

    return data;
}

// lower p means higher priority, ordered in descending order of priority
void pQpush(PrioQueue *pQ, int d, int p) {
    if (p < 0) { perror("pQ push Error: Priority Need to be Possitive!"); return; }
    pQNode *temp = newPQNode(d, p);
    if (IsPQEmpty(pQ)) {
        pQ->front->next = temp;
        pQ->rear = temp;
    } else {
        pQNode *find = pQ->front;
        while (find != pQ->rear && temp->priority > find->next->priority) {
            find = find->next;
            if (find->key == d) { perror("Key already exists!"); free(temp); return; }
        }
        // insert temp to the next element right after find
        if (find == pQ->rear) {
            find->next = temp;
            temp->next = NULL;
            pQ->rear = temp;
        } else {
            temp->next = find->next;
            find->next = temp;
        }
    }
}

void printPrioQueue(PrioQueue *pQ) {
    if (IsPQEmpty(pQ)) { printf("Empty Priority Queue!\n"); return; }
    pQNode *temp;
    temp = pQ->front->next;
    while (temp != NULL) {
        printf("%d p:%d, ", temp->key, temp->priority);
        temp = temp->next;
    }
    printf("\n");
}

void clearPrioQueue(PrioQueue *pQ) {
    if (IsPQEmpty(pQ)) return;
    while (!IsPQEmpty(pQ)) pQpop(pQ);
}

int getPrioQueueSize(PrioQueue *pQ) {
    if (IsPQEmpty(pQ)) return 0;
    int count = 0;
    pQNode *temp = pQ->front;
    while (temp != pQ->rear) {
        count++;
        temp = temp->next;
    }
    return count;
}

int findPrio(PrioQueue *pQ, int key) {
    if (IsPQEmpty(pQ)) { perror("Cannot Find Priority due to Empty Priority Queue!\n"); exit(1); }
    pQNode *temp = pQ->front->next;
    while (temp != pQ->rear->next) {
        if (temp->key == key) { return temp->priority;}
        else { temp = temp->next; }
    }
    printf("Could not find in Priority Queue!\n");
    return -1;
}

void changePrio(PrioQueue *pQ, int key, int newP) {
    if (IsPQEmpty(pQ)) {
        perror("Cannot Find Priority due to Empty Priority Queue!\n");
        exit(1);
    }
    pQNode *temp = pQ->front->next;
    pQNode *prev = pQ->front;
    while (temp != pQ->rear->next && temp->key != key) {
        temp = temp->next;
        prev = prev->next;
        assert(prev->next == temp);
    }
    if (temp == pQ->rear->next) { perror("change priority error: could not find key!\n"); return; }
    else {
        if (temp->priority == newP) {
            printf("changePrio warning: same priority!\n");
            return;
        }
        prev->next = temp->next;
        free(temp);
        pQ->rear = pQ->front;
        while (pQ->rear->next != NULL) pQ->rear = pQ->rear->next;
        pQpush(pQ, key, newP);
    }
}

///////////////////////////////////////////////////////////////////////
// Calculate shortest path of two vertices in unweighted adjacency list
// Using BFS
// Return the path
////////////////////////////////////////////////////////////////////////

LinkQueue* BFS_return_path(nWVertexNode **adjList, int src, int dest, int ListSize) {
    int curr;
    LinkQueue *pLQ;
    pLQ = (LinkQueue*)malloc(sizeof(LinkQueue));
    InitLinkQueue(pLQ);
    if (src == dest) { EnQueue(pLQ, src); return pLQ; } // no need to transfer from one line to another

    BFSinfo *pBFS = (BFSinfo *)malloc(ListSize * sizeof(BFSinfo));
    if (pBFS == NULL) {
        perror("Memory allocated for BFS Info failed. Abort.\n");
        exit(1);
    }
    if (src >= ListSize || dest >= ListSize) {
        perror("source and destination node out of range!\n");
        exit(1);
    }

    // swap source and destination in order to do BFS from destination to source
    // after BFS backtrack and find the path from source to destination
    int temp;
    temp = src;
    src = dest;
    dest = temp;
    if (!IsQueueEmpty(pLQ)) clearQueue(pLQ);
    for (int i = 0; i < ListSize; ++i) {
        pBFS[i].distance = INT_MAX;
        pBFS[i].pre = -1;
        pBFS[i].visited = false;
    }
    pBFS[src].distance = 0;
    EnQueue(pLQ, src);
    //while (!IsQueueEmpty(pLQ)) {
    while (!pBFS[dest].visited) {
        // printQueue(pLQ);
        curr = DeQueue(pLQ);
        // printf("BFS %d ... \n", curr);
        pBFS[curr].visited = true;
        if (adjList[curr]->firstEdge != NULL) {
            nWEdgeNode *tmpPtr = adjList[curr]->firstEdge;
            while (tmpPtr != NULL) {
                if (!pBFS[tmpPtr->edge].visited) {
                    pBFS[tmpPtr->edge].distance = pBFS[curr].distance + 1;
                    pBFS[tmpPtr->edge].pre = curr;
                    pBFS[tmpPtr->edge].visited = true;
                    EnQueue(pLQ, tmpPtr->edge);
                }
                tmpPtr = tmpPtr->nextEdge;
            }
        }
    }
    clearQueue(pLQ);
    // use LinkQueue to store the path
    // printQueue(pLQ);
    curr = dest;
    EnQueue(pLQ, curr);
    while (pBFS[curr].pre != src) {
        EnQueue(pLQ, pBFS[curr].pre);
        curr = pBFS[curr].pre;
    }
    EnQueue(pLQ, src);
    // printQueue(pLQ);
    return pLQ;
}

/////////////////////////////////////////////////////////////////////////
// Dijkstra to calculate shortest path between source and destination
/////////////////////////////////////////////////////////////////////////

LinkQueue* Dijkstra_return_path_weight(WeightedVertexNode **Wadjlist, int src, int dest, int size) {
    if (size < 0) {perror("Dijkstra error: Unexpected negative size.\n"); exit(1); }
    if (src >= size) { perror("Dijkstra error: source out of range.\n"); exit(1);}
    if (dest >= size) { perror("Dijkstra error: destination out of range.\n"); exit(1);}
    // LinkQueue to store path from src to dest
    LinkQueue *path;
    path = (LinkQueue*)malloc(sizeof(LinkQueue));
    InitLinkQueue(path);
    // when source is the same as destination, return source itself
    if (src == dest) { EnQueue(path, src); return path;}

    // swap src and dest
    int t;
    t = dest;
    dest = src;
    src = t;
    // to store dijkstra shortest path and predecessor info of each node
    Dinfo *pD = (Dinfo*)malloc(size * sizeof(Dinfo));
    if (pD == NULL) { perror("Memory allocation for Dijkstra info failed. Abort.\n"); exit(1); }
    // initialize: distance to be infinity and pre to be undefined
    for (int i = 0; i < size; ++i) {
        pD[i].distance = INT_MAX;
        pD[i].pre = -1;
    }
    pD[src].distance = 0;

    PrioQueue *pQ;
    pQ = (PrioQueue*)malloc(sizeof(PrioQueue));
    if (pQ == NULL) { perror("Memory allocation for Priority Queue failed. Abort.\n"); exit(1); }
    InitPQ(pQ);
    for (int i = 0; i < size; ++i) {
        if (i != src)
            pQpush(pQ, i, pD[i].distance);
        else
            pQpush(pQ, src, pD[src].distance);
    }

    int curr = -1;
    int tmpDis = 0;
    while (!IsPQEmpty(pQ)) {
        // printPrioQueue(pQ);
        curr = pQpop(pQ);
        if (Wadjlist[curr]->firstEdge != NULL) {
            WeightedEdgeNode *tmp = Wadjlist[curr]->firstEdge;
            while (tmp != NULL) {
                tmpDis = pD[curr].distance + tmp->weight;
                assert(tmpDis >= 0);
                if (tmpDis < pD[tmp->edge].distance) {
                    pD[tmp->edge].distance = tmpDis;
                    changePrio(pQ, tmp->edge, tmpDis);
                    pD[tmp->edge].pre = curr;
                }
                tmp = tmp->nextEdge;
            }
        }
    }
    assert(IsQueueEmpty(path));
    curr = dest;
    EnQueue(path, curr);
    while (pD[curr].pre != src) {
        curr = pD[curr].pre;
        EnQueue(path, curr);
    }
    EnQueue(path, src);
    // also return the total distance as the last element of the queue
    EnQueue(path, pD[dest].distance);
    // printQueue(path);
    return path;
}

/////////////////////////////////////////////////////////////////////
// Build Guangzhou City Metro Map
////////////////////////////////////////////////////////////////////

nWVertexNode** build_gz_line_adjlist(int *size) {

    // line 1 - 0
    // line 2 - 1
    // line 3 - 2
    // line 4 - 3
    // line 5 - 4
    // line 8 - 5
    *size = 6;
    nWVertexNode **adjList = malloc_unweighted_vertex_node(*size);

    add_unweighted_undirected_edge(adjList, 0, 1);
    add_unweighted_undirected_edge(adjList, 1, 2);
    add_unweighted_undirected_edge(adjList, 1, 4);
    add_unweighted_undirected_edge(adjList, 1, 5);
    add_unweighted_undirected_edge(adjList, 0, 4);
    add_unweighted_undirected_edge(adjList, 5, 2);
    add_unweighted_undirected_edge(adjList, 0, 2);
    add_unweighted_undirected_edge(adjList, 4, 2);
    add_unweighted_undirected_edge(adjList, 3, 4);
    add_unweighted_undirected_edge(adjList, 3, 5);

    return adjList;
}

WeightedVertexNode** build_gz_transfer_stationNum(int *size) {

    // JiaHeWangGang 0
    // Guangzhou Railway Station 1
    // TiYuXiLu 2
    // GongYuanQian 3
    // YangJi 4
    // ZhuJiang New Town 5
    // ChePi South 6
    // ChangGang 7
    // KeCun 8
    // WanShengWei 9
    *size = 10;
    *size += 2; // add two node for source and destination
    //*size = 5;
    WeightedVertexNode **Wadjlist = malloc_weighted_vertex_node(*size);

    add_weighted_undirected_edge(Wadjlist, 0, 1, 8);
    add_weighted_undirected_edge(Wadjlist, 0, 2, 9);
    add_weighted_undirected_edge(Wadjlist, 1, 3, 3);
    add_weighted_undirected_edge(Wadjlist, 3, 7, 4);
    add_weighted_undirected_edge(Wadjlist, 3, 4, 4);
    add_weighted_undirected_edge(Wadjlist, 7, 8, 4);
    add_weighted_undirected_edge(Wadjlist, 2, 4, 1);
    add_weighted_undirected_edge(Wadjlist, 2, 5, 1);
    add_weighted_undirected_edge(Wadjlist, 4, 5, 2);
    add_weighted_undirected_edge(Wadjlist, 5, 8, 2);
    add_weighted_undirected_edge(Wadjlist, 5, 6, 5);
    add_weighted_undirected_edge(Wadjlist, 6, 9, 1);
    add_weighted_undirected_edge(Wadjlist, 8, 9, 5);
    add_weighted_undirected_edge(Wadjlist, 1, 4, 5);

    return Wadjlist;
}
