//
//  management.c
//
//  Created by Ruxu Zhang
//

//enter deepthought:  ssh rzhang418@deepthought-login.cc.gatech.edu
//ask for node:  qsub -I -q class -l nodes=1 -l walltime=00:30:00

//complie:  gcc management.c -o main -std=c99
//run  ./main map_data.txt


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void inputAndOutput(FILE* ofp){
		printf("**************************************************************\n");
		printf("**************************************************************\n");
		printf("************* Welcome to Metro Management System *************\n");
		printf("**************************************************************\n");
		printf("**************************************************************\n\n");

		printf("To start the application, we need some information of your metro system.\n");
		printf("Please enter the information following the prompts\n\n");

		int num_t_lines;
		int num_t_stations;

		printf("How many lines in the system : ");
		scanf("%d", &num_t_lines);
		printf("How many stations in the system : ");
		scanf("%d", &num_t_stations);
		fprintf(ofp, "%d %d\n", num_t_lines, num_t_stations);
		printf("\n");

		bool loop = true;
		int line_ID = 1;
		int line_name = -1;
		/* input the information of a line */
		while(loop){
			/* variables of metro line */
			int num_stations = 0;
			char if_continue = 'z';
			int last_time = 0;

			/* the first line in file */
			printf("How many stations on this line : ");
			scanf("%d", &num_stations);
			printf("The line's number is : ");
			scanf("%d", &line_name);
			fprintf(ofp, "%d %d\n", line_name, num_stations);
			printf("\n");

			/* the following line */
			/* input the information of stations */
			for(int i=1; i<=num_stations; i++){
				/* variables of station*/
				char name_station[100];
				char if_transfer_c;
				int if_transfer;
				int ID_transfer;
				int time;

				/* prompts */
				printf("The name of the No.%d station: ", i);
				scanf("%s", name_station);
				fprintf(ofp, "%s ", name_station);
				while(1){
					if(i==1) {
						time = 0; 
						fprintf(ofp, "%d ", time);
					}else{
						while(1){
							char time_c[10];
							printf("How long it takes from the start station to this station: ");
							scanf("%s", time_c);
							if((int)time_c[0] == 0 || (int)time_c[0] <48 || (int)time_c[0] >57){
								printf("\n---Attention! The time should be a number---\n");//the time is shorter than the last station
								printf("---Please enter again---\n\n");
							}else{
								int time = atoi(time_c);
								if(time > last_time && time != 0) {
									last_time = time;
									fprintf(ofp, "%d ", time);
									break;
								}else{
								printf("\n---Attention! The time should be longer than the time of the last station---\n");//the time is shorter than the last station
								printf("---Please enter again---\n\n");
								}
							}
						}
					}
					printf("Is it a transfer station? [y/n]: ");
					scanf(" %c", &if_transfer_c);
					if(if_transfer_c == 'y'){
						while(1){
							printf("How many lines this station can transfer to: ");
							scanf("%d", &if_transfer);
							if(if_transfer > 0){
								fprintf(ofp, "%d ", if_transfer);
								for(int j=1; j<=if_transfer; j++){
									while(1){
										printf("Enter the No.%d line it can transfer to: ", j);
										scanf("%d", &ID_transfer);
										if(ID_transfer != line_ID) {fprintf(ofp, "%d ", ID_transfer); break;}
										else if(ID_transfer == line_ID){
											printf("\n---Attention! Cannot transfer to the line of itself---\n");//transfer to itself
											printf("---Please enter again---\n\n");
										}else{
											printf("\n---Attention! Illegal input---\n");//not a legal number
											printf("---Please enter again---\n\n");
										}
									}
								}
								break;
							}else{
								printf("\n---Attention! Illegal input---\n");//the number of transfer station is <= 0, when it is a transfer station
								printf("---Please enter again---\n\n");
							}
						}
						break;
					}else if(if_transfer_c == 'n'){
						if_transfer = 0;
						fprintf(ofp, "%d ", if_transfer);
						break;
					}else{
						printf("\n---Attention! Please enter 'y' for yes or 'n' for no---\n");//not answer the yes-or-no question
						printf("---Please enter again---\n\n");
					}
				}//end while
				fprintf(ofp, "\n");
				printf("\n");
			}//end for

			while(1){
				printf("Continue to add the next line? [y/n]: ");
				scanf(" %c", &if_continue);
				if(if_continue == 'n') {loop = false; break;}
				else if(if_continue == 'y') {line_ID++; break;}
				else{
					printf("\n---Attention! Please enter 'y' for yes or 'n' for no\n");//not answer the yes-or-no question
					printf("---Please enter again---\n\n");
				}
			}//end while
		}//end while loop
}//end inputAndOutput function


int main(int argc, const char * argv[]) {

	FILE* ofp;
	if ((ofp = fopen(argv[1], "w")) == NULL)  printf("Error! opening output file\n");

	inputAndOutput(ofp);

	/* close file */
	fclose(ofp);



	return 0;
}//end main function
