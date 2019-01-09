//
//  readFile.c
//
//  Created by Ruxu Zhang
//  Modified by Yingqiao Zheng on 12/1/2018
//

//enter deepthought:  ssh rzhang418@deepthought-login.cc.gatech.edu
//ask for node:  qsub -I -q class -l nodes=1 -l walltime=00:30:00

//complie:  gcc management.c -o main -std=c99
//run  ./main map_data.txt


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "readFile.h"
#define MAX 200

/*global variables*/
extern int num_of_lines;//the number of lines
extern int num_of_stations;//the number of stations, regardless of normal or transfer, without duplication
extern int num_of_normal_stations;//the number of normal stations
extern int num_of_transfer_stations;//the number of transfer stations

extern LINE lines[MAX/10];//max is 20, assume a line at least has 10 stations
extern STATION stations[MAX];//max is 200
extern STATION normalStations[MAX];// max is 200
extern TSTATION transferStations[MAX/2];//assume at most 1/2 stations are transfer station


int readFile(FILE* ifp, int num_t_lines, int num_t_stations){

	int s_count = 0;

	/*read the information of lines*/
	for(int k=0; k<num_of_lines; k++){

		int line_ID;
		int num_stations;
		int start_index;
		int end_index;

		/*read the information of lines*/
		if(fscanf(ifp, "%d", &line_ID) != 1) {printf("1 Error: read the file"); exit(1);}
		lines[k].line_ID = line_ID;
		//printf("line: %d\n", lines[k].line_ID);
		if(fscanf(ifp, "%d", &num_stations) != 1) {printf("2 Error: read the file"); exit(1);}
		lines[k].num_stations = num_stations;
		//printf("%d\n", lines[k].num_stations);
		lines[k].start_index = s_count;
		//printf("%d\n", lines[k].start_index);

		/*read the information of stations*/
		for(int i=0; i<num_stations; i++){
			/*variables*/
			char name[100];
			int time;
			int if_transfer;

			/*read*/
			if(fscanf(ifp, "%s", name) != 1) {printf("3 Error: read the file"); exit(1);}
			//printf("station: %s\n", name);
			if(fscanf(ifp, "%d", &time) != 1) {printf("4 Error: read the file"); exit(1);}
			//printf("%d\n", time);
			if(fscanf(ifp, "%d", &if_transfer) != 1) {printf("5 Error: read the file"); exit(1);}
			//printf("%d\n", if_transfer);

			//printf("aaa\n");

			memcpy(stations[s_count].name,name,strlen(name));
			stations[s_count].line_location = line_ID;
			stations[s_count].ID_in_line = i+1;
			stations[s_count].time = time;
			stations[s_count].if_transfer = if_transfer;

			//printf("aaa\n");

			if(if_transfer > 0 && if_transfer <= 4){
				for(int j=0; j<4; j++) stations[s_count].transfer_lines[j] = 0;
				for(int j=0; j<if_transfer; j++){
					int transfer_line;
					if(fscanf(ifp, "%d", &transfer_line) != 1) {printf("6 Error: read the file"); exit(1);}
					stations[s_count].transfer_lines[j] = transfer_line;
					//printf("%d\n", stations[s_count].transfer_lines[j]);
				}
			}

			s_count++;
		}//end station for

		lines[k].end_index = s_count-1;
		//printf("%d\n", lines[k].end_index);
	}//end line outside for
	findNextPrevTransfer(s_count-1);
	return s_count;
}//end readFile function


//////////////////////////////////////////////////////
/////////find
//////////////////////////////////////////////////////

void findNextPrevTransfer(int stations_length){
	for(int k=0; k<num_of_lines; k++){
		int p=-1, n=-1;
		for(int i=lines[k].start_index; i<=lines[k].end_index; i++){
			if(stations[i].if_transfer > 0)  n = i;
			stations[i].next_transfer_index = n;
		}
		for(int i=lines[k].end_index; i>=lines[k].start_index; i--){
			if(stations[i].if_transfer > 0)  p = i;
			stations[i].prev_transfer_index = p;
		}
	}
}

void findNormalStation(int stations_length){
	int c=0;
	for(int i=0; i<stations_length; i++){
		if(stations[i].if_transfer == 0){
			normalStations[c] = stations[i];
			c++;
		}
	}
	num_of_normal_stations = c;
}

void findTransferStation(int stations_length){
	int c=0;
	/*initialize to -1*/
	for(int i=0; i<MAX/2; i++){
		for(int j=0; j<5; j++){
			transferStations[i].lines[j] = -1;
			transferStations[i].ID[j] = -1;
		}
	}
	for(int i=0; i<stations_length; i++){
		if(stations[i].if_transfer != 0){
			int yes = 1;
			for(int j=0; j<c; j++){
				//printf("1 %s\n",transferStations[j].name);
				//printf("2 %s\n\n",stations[i].name);
				if(strcmp(transferStations[j].name, stations[i].name) == 0){
					yes = 0;//not create
					int k;
					for(k=0; k<5; k++){
						if(transferStations[j].lines[k] != -1) break;
					}
					transferStations[j].lines[k+1] = stations[i].line_location;
					transferStations[j].ID[k+1] = stations[i].ID_in_line;
				}
			}
			//printf("%d\n", yes);
			if(yes == 1){
				memcpy(transferStations[c].name,stations[i].name,strlen(stations[i].name));
				transferStations[c].lines[0] = stations[i].line_location;
				transferStations[c].ID[0] = stations[i].ID_in_line;
				c++;
			}
		}
	}
	num_of_transfer_stations = c;
}

///////////////////////////////////////////////////
////////// print
///////////////////////////////////////////////////

void printLines(int num_t_lines){
	printf("\n**********************************\nLines:\n");
	for(int i=0; i<num_t_lines; i++){
		printf("line:\n");
		printf("%d\t%d\t%d\t%d\n",lines[i].line_ID,lines[i].num_stations,lines[i].start_index,lines[i].end_index);
	}
}

void printStations(int num_t_stations){
	printf("\n**********************************\nStations:\n");
	printf("name   line   id   time   iftransfer  (transfer)  prev   next:\n");
	for(int i=0; i<num_t_stations; i++){
		printf("%s\t%d\t%d\t%d\t%d\t",stations[i].name,stations[i].line_location,stations[i].ID_in_line,stations[i].time,stations[i].if_transfer);
		for(int j=0; j<stations[i].if_transfer;j++){
			printf("%d\t", stations[i].transfer_lines[j]);
		}
		printf("%d\t%d\n", stations[i].next_transfer_index, stations[i].prev_transfer_index);
	}
}

void printTransferStations(){
	printf("\n**********************************\nTransfer Stations:\n");
	for(int i=0; i<num_of_transfer_stations; i++){
		printf("station name: %s\n", transferStations[i].name);
		for(int j=0; j<5;j++){
			if(transferStations[i].lines[j] == -1) break;
			printf("%d\t%d\n", transferStations[i].lines[j], transferStations[i].ID[j]);
		}
	}
}



int findTransferInd_usingName (char *name) {
	for (int i = 0; i < num_of_transfer_stations; ++i) {
		if (strcmp(name, transferStations[i].name) == 0 )
			return i;
	}
	printf("Transfer station not found. \n");
	return -1;
}

void findTwoLineInd_usingLineName (int *a, int *b) {
	bool flg_a = false, flg_b = false;
	for (int i = 0; i < num_of_lines; ++i) {
		if (lines[i].line_ID == *a) { *a = i; flg_a = true;}
		if (lines[i].line_ID == *b) { *b = i; flg_b = true;}
		if (flg_a == true && flg_b == true) return;
	}
	if (flg_a == false && flg_b == false) { perror("No matching line!"); exit(1); }
}

int findTransStationInd_betweenTwoLine (int a, int b) {
	assert(a>=0 && b>=0);
	if (a == b) {printf( "Same line, no transfer station!\n"); return -1;}
	int count = 0;
	for (int i = 0; i < num_of_transfer_stations; ++i) {
		count = 0;
		for (int j = 0; transferStations[i].lines[j] != -1; ++j) {
			if (transferStations[i].lines[j] == lines[a].line_ID || transferStations[i].lines[j] == lines[b].line_ID) count++;
			if (count == 2) return i;
		}
	}
	if (count < 2) { perror("No such transfer station.\n"); return -1; }
	return -1;
}

int findLineInd_usingLineID (int ID) {
	for (int i = 0; i < num_of_lines; ++i) {
		if (lines[i].line_ID == ID) { return i;}
	}
	printf("Cannot find Line Index.\n");
	return -1;
}


// transInd and line are both index
int findStationInd_withTransferInd (int transInd, int line) {
	// int lineInd = -1;
	int offset = 0;
    for (int i = 0; transferStations[transInd].lines[i] != -1 ; ++i) {
        if (transferStations[transInd].lines[i] == lines[line].line_ID) {
			// lineInd = findLineInd_usingLineID(line);
			offset = transferStations[transInd].ID[i] - 1;
			break;
        }
    }
    assert(strcmp(stations[lines[line].start_index + offset].name, transferStations[transInd].name) == 0);
    return lines[line].start_index + offset;
}

int findCommonLine_betweenTwoTransStation (int transInd_1, int transInd_2) {
    for (int i = 0; transferStations[transInd_1].lines[i] != -1 ; ++i) {
        for (int j = 0; transferStations[transInd_2].lines[j] != -1 ; ++j) {
            if (transferStations[transInd_1].lines[i] == transferStations[transInd_2].lines[j]) {
                return transferStations[transInd_1].lines[i];
            }
        }
    }
    printf("These two transfer station do not share a line! \n");
    return -1;
}



// void equalStations(STATION s1, STATION s2){
// 	s1.name = strdup(s2.name);
// 	printf("4 %s\n",s1.name);
//     s1.line_location = s2.line_location;
//     s1.ID_in_line = s2.ID_in_line;
//     s1.time = s2.time;
//     s1.if_transfer = s2.if_transfer;
//     for(int i=0; i<s1.if_transfer; i++){
//     	s1.transfer_lines[i] = s2.transfer_lines[i];
//     }
//     s1.next_transfer_index = s2.next_transfer_index;
//     s1.prev_transfer_index = s2.prev_transfer_index;
// }
