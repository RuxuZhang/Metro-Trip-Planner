# CSE6010

Author          : Ruxu Zhang, Yingqiao Zheng
Created         : November 11, 2018
Last Modified   : December 4, 2018

Affiliation          : Georgia Institute of Technology


Description
-------------

This program includes two parts:
	Part 1: management.c
	Part 2: other files

The first part aims to initialize the system by inputting information and generate a txt file. The second part is the main part, which aims to use the file to start the system and generate two trip plans according to the user's inputs. The two trip plans are base on two standards: (1) route with minimal transfer needs; (2) route with minimal passing stations.

Installation
------------

    Part 1: gcc management.c -o management -std=c99
    Part 2: gcc main.c output.c compute.c readFile.c -o main -std=c99


Execution
-----------

	Part 1: ./management test.txt
	Part 2: ./main gz.txt

Part 1 is complex to test because it has to input a lot of information in order to generate the txt file of Guangzhou metro map, so the validation of its correction is in the report attached to our project which is a screenshot of the printed txt file.

