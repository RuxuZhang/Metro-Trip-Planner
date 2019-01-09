//
//  readFile.h
//
//  Created by Ruxu Zhang
//  Modified by Yingqiao Zheng on 12/1/2018
//

#ifndef readFile_h
#define readFile_h

#include <stdio.h>

typedef struct station{
  char name[100];
  int line_location; //the line on which this station is located
  int ID_in_line; //Each station in a line is assigned a number
  int time; //Time to reach this station from the first station on this line.
  int if_transfer; //number of other lines a transfer can be done to
  int transfer_lines[4]; //To store the transfer lines at a station.
  int next_transfer_index;//the next transfer station
  int prev_transfer_index;//the previous transfer station
} STATION;

typedef struct tStation{
  char name[100];
  int lines[5];
  int ID[5];
}TSTATION;

typedef struct line{
  int line_ID;
  int num_stations;
  int start_index;
  int end_index;
}LINE;

#endif /* readFile_h */

void readFirstLine(FILE* ifp);
int readFile(FILE* ifp, int num_t_lines, int num_t_stations);
void findNextPrevTransfer(int stations_length);
void printLines(int num_t_lines);
void printStations(int num_t_stations);
void printTransferStations();
void findNormalStation(int stations_length);
void findTransferStation(int stations_length);
// void equalStations(STATION s1, STATION s2);
int findTransferInd_usingName (char *name);
void findTwoLineInd_usingLineName (int *a, int *b);
int findTransStationInd_betweenTwoLine (int a, int b);
int findLineInd_usingLineID (int ID);
int findStationInd_withTransferInd (int transInd, int line);
int findCommonLine_betweenTwoTransStation (int transInd_1, int transInd_2);
