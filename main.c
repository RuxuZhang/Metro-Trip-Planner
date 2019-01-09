//
//  main.c
//  CSE_6010_proj
//
//  Created by Yingqiao Zheng on 11/11/18.
//  Copyright © 2018 Yingqiao Zheng. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "compute.h"
#include "output.h"
#include "readFile.h"

#define MAX 200

/* simply by comment and uncomment these arguments to execute different test */

// #define test_BFS
// #define test_prioQ
// #define test_DIJK
// #define Print_Info_and_Graph
#define read_file
#define GZ_Line
#define GZ_Transfer

/*global variables*/
int num_of_lines;                       //the number of lines
int num_of_stations;                    //the number of stations, regardless of normal or transfer, without duplication
int num_of_normal_stations;             //the number of normal stations
int num_of_transfer_stations;           //the number of transfer stations

LINE lines[MAX/10];                     //max is 20, assume a line at least has 10 stations
STATION stations[MAX];                  //max is 200
STATION normalStations[MAX];            // max is 200
TSTATION transferStations[MAX/2];       //assume at most 1/2 stations are transfer station


int main(int argc, char *argv[]) {

#ifdef test_BFS
    // create Metro Line List
    int LineSize = 0;
    nWVertexNode **lineList = build_gz_line_adjlist(&LineSize);
    print_unweighted_adjList(lineList, LineSize);

    // calculate wiht BFS
    LinkQueue *transferPath;
    transferPath = BFS_return_path(lineList, 0, 5, 6);
    printQueue(transferPath);
    // printf("%d\n", getQueueSize(transferPath));
    clearQueue(transferPath);
    free(transferPath);
    free_unweighted_adjList(lineList, LineSize);
#endif

#ifdef test_prioQ
    PrioQueue *pQ;
    pQ = (PrioQueue*)malloc(sizeof(PrioQueue));
    InitPQ(pQ);
    printPrioQueue(pQ);
    pQpush(pQ, 1, 2);
    pQpush(pQ, 3, 2);
    // 3 p:2, 1 p:2,
    printPrioQueue(pQ);
    pQpush(pQ, 5, 6);
    //pQpush(pQ, 4, 4);
    pQpush(pQ, 4, 8);
    pQpush(pQ, 8, 1);
    pQpush(pQ, 10, 3);
    // 8 p:1, 3 p:2, 1 p:2, 10 p:3, 5 p:6, 4 p:8,
    printPrioQueue(pQ);

    // 6
    printf("pQ size: %d\n", getPrioQueueSize(pQ));

    // 8
    printf("%d\n", findPrio(pQ, 4));

    // 8
    printf("%d\n", maxPrioData(pQ));

    changePrio(pQ, 1, 3);

    // 8 p:1, 3 p:2, 1 p:3, 10 p:3, 5 p:6, 4 p:8,
    printPrioQueue(pQ);

    changePrio(pQ, 5, 6);
    printf("pop: %d\n", pQpop(pQ));
    // 3 p:2, 1 p:3, 10 p:3, 5 p:6, 4 p:8,
    printPrioQueue(pQ);

    changePrio(pQ, 9, 3);

    clearPrioQueue(pQ);
    // empty
    printPrioQueue(pQ);
    free(pQ);
#endif

#ifdef test_DIJK
    // create transfer station list
    int TransferSize = 0;
    WeightedVertexNode **transferList = build_gz_transfer_stationNum(&TransferSize);
    // add zhongshanba
    add_weighted_undirected_edge(transferList, 1, 10, 3);
    // add yuancun
    add_weighted_undirected_edge(transferList, 5, 11, 3);
    add_weighted_undirected_edge(transferList, 6, 11, 2);

    print_weighted_adjList(transferList, TransferSize);

    LinkQueue *transferPath_2;
    transferPath_2 = Dijkstra_return_path_weight(transferList, 10, 11, TransferSize);
    printQueue(transferPath_2);
    // printf("%d\n", getQueueSize(transferPath_2));
    clearQueue(transferPath_2);
    free(transferPath_2);

    free_weighted_adjList(transferList, TransferSize);
#endif

    /* ********************** Read Input File ************************* */

#ifdef read_file
    FILE *ifp;
    if ((ifp = fopen(argv[1], "r")) == NULL) printf("Error! opening input file\n");

    /*read the first line*/
    int num_t_lines;
    int num_t_stations;

#ifdef Print_Info_and_Graph
    printf("\n**********************************\n");
#endif
    if (fscanf(ifp, "%d", &num_t_lines) != 1) {
        printf("Error: read the file");
        exit(1);
    }
    num_of_lines = num_t_lines;

#ifdef Print_Info_and_Graph
    printf("There are %d lines,", num_of_lines);
#endif

    if (fscanf(ifp, "%d", &num_t_stations) != 1) {
        printf("Error: read the file");
        exit(1);
    }
    num_of_stations = num_t_stations;

#ifdef Print_Info_and_Graph
    printf("and %d stations.\n", num_of_stations);
#endif

    /*read file*/
    int stations_length = readFile(ifp, num_of_lines, num_of_stations);

    /*print lines and stations*/

#ifdef Print_Info_and_Graph
    printLines(num_of_lines);
    printStations(stations_length);
#endif

    /*find two types of stations and save separately*/

    findNormalStation(stations_length);
    findTransferStation(stations_length);

#ifdef Print_Info_and_Graph
    printTransferStations();
#endif

    if (num_of_normal_stations + num_of_transfer_stations != num_of_stations) {
        printf("The sum of normal stations and transfer stations is not equal to the total number of stations. Please inpute the right file.");
        exit(1);
    }

#ifdef Print_Info_and_Graph
    printf("\n**********************************\nnormal stations: %d\n", num_of_normal_stations);
    printf("transfer stations: %d\n\n", num_of_transfer_stations);
#endif

    /* Close file */
    fclose(ifp);
#endif


#ifdef GZ_Line
    /******************** Build Unweighted Line Graph ********************/
    // unweighted line graph
    nWVertexNode **GZLine = malloc_unweighted_vertex_node(num_of_lines);
    int ID1 = -1, ID2 = -1;
    // search every transfer station for intersect lines
    for (int i = 0; i < num_of_transfer_stations; ++i) {
        // use i and j to store every pair of intersect line
        for (int j = 0; transferStations[i].lines[j] != -1; ++j) {
            for (int k = j+1; transferStations[i].lines[k] != -1; ++k) {
                // find out index of line using line ID
                // add edge to graph with each vertex indicating a line with its index
                for (int l = 0; l < num_of_lines; ++l) {
                    if (transferStations[i].lines[j] == lines[l].line_ID) { ID1 = l; }
                    else if (transferStations[i].lines[k] == lines[l].line_ID) { ID2 = l; }
                }
                if (ID1 == -1 || ID2 == -1) {perror("No matching line!"); exit(1);}
                add_unweighted_undirected_edge(GZLine, ID1, ID2);
            }
        }
    }
#ifdef Print_Info_and_Graph
    printf("\n");
    print_unweighted_adjList(GZLine, num_of_lines);
    printf("\n");
#endif

#endif



#ifdef GZ_Transfer
/* ***************** Build Weighted Transfer Station Graph ******************* */
    WeightedVertexNode **GZTransfer = malloc_weighted_vertex_node(num_of_transfer_stations + 2);
    int prev_trans, curr_trans;       // store transfer station Index as the vertex number in graph
    int prev_stat = -1;                         // store previous transfer station's index in stations, to calculate weight
    // scan every lines's station to find transfer station
    for (int i = 0; i < num_of_lines; ++i) {
        prev_stat = -1;
        prev_trans = -1;
        for (int j = lines[i].start_index; j <= lines[i].end_index; ++j) {
            if (stations[j].if_transfer > 0) {
                curr_trans = findTransferInd_usingName(stations[j].name);
                if (prev_trans != -1 && prev_stat != -1) {
                    add_weighted_undirected_edge(GZTransfer, prev_trans, curr_trans,
                            abs(stations[prev_stat].ID_in_line - stations[j].ID_in_line)); }
                prev_trans = curr_trans;
                prev_stat = j;
            }
        }
    }
#ifdef Print_Info_and_Graph
    printf("\n");
    print_weighted_adjList(GZTransfer, num_of_transfer_stations + 2);
    printf("\n");
#endif

#endif

/* ***************************** Planning system starts here **************************** */
    printf("**************************************************************\n");
	printf("**************************************************************\n");
	printf("***************** Welcome to Guangzhou Metro *****************\n");
	printf("**************************************************************\n");
	printf("**************************************************************\n\n");

	/* ********************* The customer input the information ******************* */
	char startStation[MAX];
	char endStation[MAX];
	printf("Please input your start station and destination : (seperate with enter)\n");
	scanf("%s", startStation);
	// printf("Please input your destination : ");
	scanf("%s", endStation);

    // variables
    int line_1 = -1, line_2 = -1;               // to store line index of both station in line list
    int station_1 = -1, station_2 = -1;         // to store station index of both station in stations list
    int trans_station_1 = -1, trans_station_2 = -1;     // if there is transfer station, store transfer index in trans station list

    // /////////////////////////////////////////////////////////////////////////////////
    // expected value:
    // (s: start station; d: destination)
    // (T: is transfer station, F: not transfer station)
    // 1. s:F, d:F
    //      trans_station_1 == -1, trans_station_2 == -1;
    // 2. s:T, d:F
    //      trans_station_1 != -1, trans_station_2 == -1;
    // 3. s:F, d:T
    //      trans_station_1 == -1, trans_station_2 != -1;
    // 4. s:T, d:T
    //      trans_station_1 != -1, trans_station_2 != -1;
    //
    //
    // line_1 and line_2 are the vertex index that we need to use to perform BFS
    // if there is at least one transfer station, and s and d are not on the same line
    // then at least one value between line_1 and line_2 is set to be -1
    // ///////////////////////////////////////////////////////////////////////////////////


    /* ********************* check if input station valid ******************* */
    for (int i = 0; strlen(stations[i].name) != 0; ++i) {
        if (strcmp(startStation, stations[i].name) == 0) { station_1 = i; }
        if (strcmp(endStation, stations[i].name) == 0) { station_2 = i; }
        if (station_1 != -1 && station_2 != -1) { break; }
    }
    if (station_1 == -1 || station_2 == -1) { printf("Error! Station not found, plz try again.\n"); return 0; }

    // if input same station name, no need to find a route plan
    if (strcmp(startStation, endStation) == 0) { printf("Same start & destination station!\n"); return 0; }



    /******************************* Find out line index for start and destination station ************************************/

    // if both station are not transfer station
    // use line_1 and line_2 to do BFS
    if (stations[station_1].if_transfer == 0 && stations[station_2].if_transfer == 0) {
        line_1 = stations[station_1].line_location;
        line_2 = stations[station_2].line_location;
        assert(line_1 != -1 && line_2 != -1);
    }

    // if start station is transfer station and destination is not
    // use trans_station_1 and line_2
    else if (stations[station_1].if_transfer > 0 && stations[station_2].if_transfer == 0) {
        line_2 = stations[station_2].line_location;
        // if they happen to be on the same line
        if (stations[station_1].line_location == line_2) { line_1 = line_2; }
        else {
            for (int i = 0; i < stations[station_1].if_transfer; ++i) {
                if (stations[station_1].transfer_lines[i] == line_2) { line_1 = line_2; break; }
            }
            // if they are not on the same line,
            // set transfer station ID refer to the station in transfer station list
            if (line_1 == -1) { trans_station_1 = findTransferInd_usingName(startStation); trans_station_2 = -1; }
        }
    }

    // if destination is transfer station and destination is not
    // use trans_station_2 and line_1
    else if (stations[station_2].if_transfer > 0 && stations[station_1].if_transfer == 0) {
        line_1 = stations[station_1].line_location;
        // if they happen to be on the same line
        if (stations[station_2].line_location == line_1) { line_2 = line_1; }
        else {
            for (int i = 0; i < stations[station_2].if_transfer; ++i) {
                if (stations[station_2].transfer_lines[i] == line_1) { line_2 = line_1; break; }
            }
            // if they are not on the same line
            // set transfer station ID refer to the station in transfer station list
            if (line_2 == -1) { trans_station_2 = findTransferInd_usingName(endStation); trans_station_1 = -1; }
        }
    }

    // if both stations are transfer station
    // use trans_station_1 and trans_station_2
    else {
        assert(line_1 == -1 && line_2 == -1);
        // if they happen to be on same route
        trans_station_1 = findTransferInd_usingName(startStation);
        trans_station_2 = findTransferInd_usingName(endStation);
        for (int i = 0; transferStations[trans_station_1].lines[i] != -1; ++i) {
            for (int j = 0; transferStations[trans_station_2].lines[j] != -1; ++j) {
                if (transferStations[trans_station_1].lines[i] == transferStations[trans_station_2].lines[j]) {
                    line_1 = transferStations[trans_station_1].lines[i];
                    line_2 = transferStations[trans_station_2].lines[j];
                    break;
                }
            }
            // break if find that they are on same line
            if (line_1 != -1 && line_2 != -1 && line_1 == line_2) break;
        }
    }
    // printf("Line name: %d %d\n", line_1, line_2);
    // printf("Station name: s:%s d:%s\ns:%d d:%d\n", transferStations[trans_station_1].name,
           // transferStations[trans_station_2].name, trans_station_1, trans_station_2);




    // if both station is located on the same line
    // line_1 and line_2 should be the same
    if (line_1 == line_2 && line_1 != -1 && line_2 != -1) {
        // printf("The plan(s) for trip from %s to %s is/are: \n", startStation, endStation);
        printf("No need to transfer!\n");
        printf("Starting from %s in line %d, go through %d stations to get to %s.\nTotal trip time: %d min.\n",
                startStation, line_1, abs(stations[station_1].ID_in_line - stations[station_2].ID_in_line),
                endStation, abs(stations[station_1].time - stations[station_2].time));
        return 0;
    }

#ifdef GZ_Line
    /* *************************** Find and Print the route using BFS *************************** */
    // if not on same line, and no transfer station
    else if (line_1 != line_2 && line_1 != -1 && line_2 != -1) {
        //printf("Line name: %d %d\n", line_1, line_2);
        findTwoLineInd_usingLineName(&line_1, &line_2);
        //printf("Line ID: %d %d\n", line_1, line_2);

        LinkQueue *transferPath;
        transferPath = BFS_return_path(GZLine, line_1, line_2, num_of_lines);
        // printQueue(transferPath);
        printPlanWithLQ_Line(transferPath, &station_1, &station_2);
    }
    // if not on same line, start station is a transfer station
    else if (line_1 == -1 && line_2 != -1) {
        assert(trans_station_1 != -1);
        line_2 = findLineInd_usingLineID(line_2);
        LinkQueue *transferPath = NULL;
        LinkQueue *temp;
        int lineInd_temp;
        for (int i = 0; transferStations[trans_station_1].lines[i] != -1 ; ++i) {
            lineInd_temp = findLineInd_usingLineID(transferStations[trans_station_1].lines[i]);
            temp = BFS_return_path(GZLine, lineInd_temp, line_2, num_of_lines);
            // printQueue(temp);
            if (i == 0) { transferPath = temp; }
            else if (getQueueSize(transferPath) > getQueueSize(temp)) { transferPath = temp; }
            // printQueue(transferPath);
        }
        printPlanWithLQ_Line(transferPath, &station_1, &station_2);
    }
    // if not on same line, destination is a transfer station
    else if (line_1 != -1 && line_2 == -1) {
        assert(trans_station_2 != -1);
        line_1 = findLineInd_usingLineID(line_1);
        LinkQueue *transferPath = NULL;
        LinkQueue *temp;
        int lineInd_temp;
        for (int i = 0; transferStations[trans_station_2].lines[i] != -1 ; ++i) {
            lineInd_temp = findLineInd_usingLineID(transferStations[trans_station_2].lines[i]);
            temp = BFS_return_path(GZLine, line_1, lineInd_temp, num_of_lines);
            // printQueue(temp);
            if (i == 0) { transferPath = temp; }
            else if (getQueueSize(transferPath) > getQueueSize(temp)) { transferPath = temp; }
            // printQueue(transferPath);
        }
        printPlanWithLQ_Line(transferPath, &station_1, &station_2);
    }
    // if both are transfer station
    else {
        assert(trans_station_1 != -1 && trans_station_2 != -1);
        LinkQueue *transferPath = NULL;
        LinkQueue *temp;
        int lineInd_temp_1 = -1, lineInd_temp_2 = -1;
        for (int i = 0; transferStations[trans_station_1].lines[i] != -1 ; ++i) {
            for (int j = 0; transferStations[trans_station_2].lines[j] != -1 ; ++j) {
                lineInd_temp_1 = findLineInd_usingLineID(transferStations[trans_station_1].lines[i]);
                lineInd_temp_2 = findLineInd_usingLineID(transferStations[trans_station_2].lines[j]);
                temp = BFS_return_path(GZLine, lineInd_temp_1, lineInd_temp_2, num_of_lines);
                // printQueue(temp);
                if (i == 0 && j == 0) { transferPath = temp; }
                else if (getQueueSize(transferPath) > getQueueSize(temp)) { transferPath = temp; }
                // printQueue(transferPath);
            }
        }
        printPlanWithLQ_Line(transferPath, &station_1, &station_2);
    }
    free_unweighted_adjList(GZLine, num_of_lines);
#endif

#ifdef GZ_Transfer
    /* ****************************** add start and destination to graph *********************************** */
    int start_trans = -1;               // src node index for Dijkstra
    int dest_trans = -1;                // dest node index for Dijkstra

    // add to graph
    addStation_Trans(GZTransfer, station_1, &start_trans, true);
    addStation_Trans(GZTransfer, station_2, &dest_trans, false);

    // printf("From %d to %d\n", start_trans, dest_trans);

    // print_weighted_adjList(GZTransfer, num_of_transfer_stations + 2);

    /* *************************** Find and Print the route using Dijkstra *************************** */

    LinkQueue *transferPath_2;
    transferPath_2 = Dijkstra_return_path_weight(GZTransfer, start_trans, dest_trans, num_of_transfer_stations + 2);
    // printQueue(transferPath_2);

    printPlan_Trans(transferPath_2, station_1, station_2);

    free(transferPath_2);
    free_weighted_adjList(GZTransfer, num_of_transfer_stations + 2);

#endif

    return 0;
}
