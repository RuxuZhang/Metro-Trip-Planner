//
// Created by Yingqiao Zheng on 11/28/18.
//

#ifndef CSE_6010_PROJ_OUTPUT_H
#define CSE_6010_PROJ_OUTPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "compute.h"
#include "readFile.h"
#define MAX 200


void printPlanWithLQ_Line (LinkQueue *path, int *start, int *dest);
void addStation_Trans (WeightedVertexNode ** graph, int a, int *addIx, bool is_start);
void printPlan_Trans (LinkQueue *path, int station_1, int station_2);

#endif //CSE_6010_PROJ_OUTPUT_H
