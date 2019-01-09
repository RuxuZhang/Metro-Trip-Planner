//
// Created by Yingqiao Zheng on 11/28/18.
//

#include "output.h"

/*global variables*/
extern int num_of_lines;//the number of lines
extern int num_of_stations;//the number of stations, regardless of normal or transfer, without duplication
extern int num_of_normal_stations;//the number of normal stations
extern int num_of_transfer_stations;//the number of transfer stations

extern LINE lines[MAX/10];//max is 20, assume a line at least has 10 stations
extern STATION stations[MAX];//max is 200
extern STATION normalStations[MAX];// max is 200
extern TSTATION transferStations[MAX/2];//assume at most 1/2 stations are transfer station

void printPlanWithLQ_Line (LinkQueue *path, int *start, int *dest) {
    assert(!IsQueueEmpty(path));
    int a = -1, b = -1;             // lineInd used to find transfer station
    int c = -1;                     // transfer station Ind
    int c_s = -1;                   // transfer station's Ind in stations
    int pre = -1;                   // previous transfer station Ind
    int transferTimes = 0;          // total number of transfer needed
    int num = 0;                    // total number of stations
    int time = 0;                   // total time spend

    assert(!IsQueueEmpty(path));
    a = DeQueue(path);
    if (stations[*start].line_location != lines[a].line_ID) {
        *start = findTransferInd_usingName(stations[*start].name);
        *start = findStationInd_withTransferInd(*start, a);
    }
    assert(stations[*start].line_location == lines[a].line_ID);
    transferTimes = getQueueSize(path);

    printf("\n\nPlan 1 (minimum number of line transfer): \n");
    printf("Starting from %s in Line %d,\n", stations[*start].name, lines[a].line_ID);
    for (int i = 0; i < transferTimes; ++i) {
        b = DeQueue(path);
        c = findTransStationInd_betweenTwoLine(a, b);
        c_s = findStationInd_withTransferInd(c, a);
        if (i == 0) {
            time += abs(stations[*start].time - stations[c_s].time);
            num += abs(stations[*start].ID_in_line - stations[c_s].ID_in_line);
            // printf("time %d num %d\n", time, num);
        } else {
            time += abs(stations[pre].time - stations[c_s].time);
            num += abs(stations[pre].ID_in_line - stations[c_s].ID_in_line);
            // printf("time %d num %d\n", time, num);
        }
        printf("then transfer to Line %d at %s,\n", lines[b].line_ID, stations[c_s].name);
        pre = findStationInd_withTransferInd(c, b);
        a = b;
    }
    if (stations[*dest].line_location != lines[a].line_ID) {
        *dest = findTransferInd_usingName(stations[*dest].name);
        *dest = findStationInd_withTransferInd(*dest, a);
    }
    time += abs(stations[pre].time - stations[*dest].time);
    num += abs(stations[pre].ID_in_line - stations[*dest].ID_in_line);
    assert(stations[*dest].line_location == lines[b].line_ID);
    printf("get to destination %s in Line %d.\n", stations[*dest].name, stations[*dest].line_location);
    printf("Total stations: %d, Total time spend: %d min.\n", num, time);

    clearQueue(path);
    free(path);
}

void addStation_Trans (WeightedVertexNode ** graph, int a, int *addIdx, bool is_start) {
    // make sure node is not isolated
    assert(!(stations[a].prev_transfer_index == -1 && stations[a].next_transfer_index == -1));

    // a is the index of the station in stations list
    int transIdx;           // index of connected transfer station in trans station list
    int weight;             // weight of the edge to be added

    if (is_start) { *addIdx = num_of_transfer_stations; }
    else { *addIdx = num_of_transfer_stations + 1; }
    // if is transfer station
    if (stations[a].prev_transfer_index == stations[a].next_transfer_index) {
        *addIdx = findTransferInd_usingName(stations[a].name);
    }
    // connect to next transfer station in graph
    else if (stations[a].prev_transfer_index == -1) {
        transIdx = findTransferInd_usingName(stations[stations[a].next_transfer_index].name);
        weight = abs(stations[a].ID_in_line - stations[stations[a].next_transfer_index].ID_in_line);
        add_weighted_undirected_edge(graph, *addIdx, transIdx, weight);
    }
    // connect to previous transfer station in graph
    else if (stations[a].next_transfer_index == -1) {
        transIdx = findTransferInd_usingName(stations[stations[a].prev_transfer_index].name);
        weight = abs(stations[a].ID_in_line - stations[stations[a].prev_transfer_index].ID_in_line);
        add_weighted_undirected_edge(graph, *addIdx, transIdx, weight);
    }
        // if two edge
    else {
        transIdx = findTransferInd_usingName(stations[stations[a].next_transfer_index].name);
        weight = abs(stations[a].ID_in_line - stations[stations[a].next_transfer_index].ID_in_line);
        add_weighted_undirected_edge(graph, *addIdx, transIdx, weight);

        transIdx = findTransferInd_usingName(stations[stations[a].prev_transfer_index].name);
        weight = abs(stations[a].ID_in_line - stations[stations[a].prev_transfer_index].ID_in_line);
        add_weighted_undirected_edge(graph, *addIdx, transIdx, weight);
    }
}

void printPlan_Trans (LinkQueue *path, int station_1, int station_2) {
    assert(!IsQueueEmpty(path));
    assert(station_1 >= 0 && station_2 >= 0);

    int length = getQueueSize(path);
    assert(length >= 4);            // should be at least start station, destination, one transfer and total number of station

    int num = 0;
    int time = 0;
    int transferLine = -1, prevTransLine = -1;

    int prevStation = -1, currStation = -1;     // transfer station list index(if is transfer station) of previous and current station
    int prevStaInd = -1, currStaInd = -1;       // stations list index of previous and current station
    int lineInd = -1;

    // extract start station
    prevStation = DeQueue(path);

    printf("\n\nPlan 2 (minimum number of passed-by stations): \n");
    // if station is normal station
    if (prevStation == num_of_transfer_stations) {
        prevStaInd = station_1;
        printf("Starting from %s in Line %d\n", stations[station_1].name, stations[station_1].line_location);
        currStation = DeQueue(path);

        prevTransLine = stations[prevStaInd].line_location;
        lineInd = findLineInd_usingLineID(prevTransLine);
        currStaInd = findStationInd_withTransferInd(currStation, lineInd);
        time += abs(stations[currStaInd].time - stations[prevStaInd].time);

        prevStation = currStation;
        currStation = DeQueue(path);

    }
    else {
        currStation = DeQueue(path);
        printf("Starting from %s in Line %d\n", transferStations[prevStation].name, findCommonLine_betweenTwoTransStation(prevStation, currStation));
        prevStation = currStation;
        currStation = DeQueue(path);
    }

    length = getQueueSize(path);

    for (int j = 0; j < length - 1; ++j) {
        if (currStation == num_of_transfer_stations + 1) { break; }
        transferLine = findCommonLine_betweenTwoTransStation(prevStation, currStation);
        lineInd = findLineInd_usingLineID(transferLine);
        prevStaInd = findStationInd_withTransferInd(prevStation, lineInd);
        currStaInd = findStationInd_withTransferInd(currStation, lineInd);
        time += abs(stations[currStaInd].time - stations[prevStaInd].time);
        // printf("%d, %d\n", prevTransLine, transferLine);
        if (prevTransLine != transferLine) {
            printf("then transfer to Line %d at %s,\n", transferLine, transferStations[prevStation].name);
        }

        prevStation = currStation;
        currStation = DeQueue(path);
        prevTransLine = transferLine;
    }

    assert(getQueueSize(path) == 1);
    num = DeQueue(path);


    // destination is normal station
    if (currStation == num_of_transfer_stations + 1) {
        currStaInd = station_2;
        transferLine = stations[station_2].line_location;
        lineInd = findLineInd_usingLineID(stations[currStaInd].line_location);
        prevStaInd = findStationInd_withTransferInd(prevStation, lineInd);
        time += abs(stations[currStaInd].time - stations[prevStaInd].time);
        // printf("%d, %d\n", prevTransLine, transferLine);
        if (prevTransLine != transferLine) {
            printf("then transfer to Line %d at %s,\n", stations[station_2].line_location, transferStations[prevStation].name);
        }
        printf("get to destination %s in Line %d.\n", stations[station_2].name, stations[station_2].line_location);
    }
    else {
        transferLine = findCommonLine_betweenTwoTransStation(prevStation, currStation);
        lineInd = findLineInd_usingLineID(transferLine);
        prevStaInd = findStationInd_withTransferInd(prevStation, lineInd);
        currStaInd = findStationInd_withTransferInd(currStation, lineInd);
        time += abs(stations[currStaInd].time - stations[prevStaInd].time);
        // printf("%d, %d\n", prevTransLine, transferLine);
        if (prevTransLine != transferLine) {
            printf("then transfer to Line %d at %s,\n", transferLine, transferStations[prevStation].name);
        }
        printf("get to destination %s in Line %d.\n", transferStations[currStation].name, transferLine);
    }

    printf("Total stations: %d, Total time spend: %d min.\n", num, time);


}
