/**
 * LPG Algorithm - file-io
 * Author: Chris.Chen
 * Date: 05/03/2020
 * Desc: Label Propagation based Process Grouping Algorithm
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */

#ifndef LPMS_FILE_IO_H
#define LPMS_FILE_IO_H

#include <cstring>
#include <string>
using namespace std;

void readFile();
void result_output();

/* 
	read input file & get network topology and communication topology
	@param {void}
	@return {void}
*/
void readFile() {
	FILE *fp;
	// absolute file path
	// char dir[128] = "/Users/chenqixian/Desktop/MyBUAA/0-Codes/0-Algorithm/LPMS/LPMS_test/";
	string dir = "/Users/chenqixian/Desktop/MyBUAA/0-Codes/0-Algorithm/LPMS/LPMS_test/";
	string test0 = "input";
	string test1 = "CloverLeaf1024.mat";
	string test2 = "miniAMR1024.mat";
	string test3 = "TeaLeaf1024.mat";
	string path = dir + test0;
	if((fp = fopen((char *)path.c_str(), "r")) == NULL) {
		printf("Error: read file failed!\n");
		return;
	}
	int cur_cost;
	fscanf(fp, "%d%d%d", &n, &m, &procNum);
	for(int i = 0 ; i < n ; i ++) {
		availableGroup.push_back(i);
	}
	for(int i = 0 ; i < procNum ; i ++) {
		for(int j = 0 ; j < procNum ; j ++) {
			fscanf(fp, "%d", &cur_cost);
			graph[i][j] = cur_cost;
			if(i == j) {
				continue;
			}
			if(cur_cost == INFINITE || cur_cost == 0) {
				graph[i][j] = -1;
				continue;
			}
			Edge e;
			e.cost = cur_cost;
			e.nextNode = j;
			edge[i].push_back(e);
		}
		P_u.push_back(i); // mark current process as unlabeled
	}
	fclose(fp);
}

/*
	format writing result to output file
	@param {void}
	@return {void} 
*/
void result_output() {
	FILE *fp;
	char path[128] = "/Users/chenqixian/Desktop/MyBUAA/0-Codes/0-Algorithm/LPMS/output";
	if((fp = fopen(path, "w")) == NULL) {
		printf("Error: write file failed!\n");
		return;
	}
	for(int i = 0 ; i < n ; i ++) {
		fprintf(fp, "group %d: ", i);
		int size = groupResult[i].size();
		sort(groupResult[i].begin(), groupResult[i].end());
		for(int j = 0 ; j < size ; j ++) {
			fprintf(fp, "%d ", groupResult[i][j]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

#endif // LPMS_FILE_IO_H