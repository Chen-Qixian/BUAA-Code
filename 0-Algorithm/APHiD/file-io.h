/**
 * APHiD - file-io
 * Authors: Chris.Chen
 * Date: 10/03/2020
 * Desc: Algorithm for Placing process Hierarchically on D-ary trees
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */

#ifndef APHID_FILE_IO_H
#define APHID_FILE_IO_H

using namespace std;

void read_task_graph() {
	FILE *fp;
	char path[128] = "/Users/chenqixian/Desktop/MyBUAA/0-Codes/0-Algorithm/APHiD/input";

	if((fp = fopen(path, "r")) == NULL) {
		printf("Error: file cannot read!\n");
		return;
	}
	
	fscanf(fp, "%d%d", &D, &taskNum);
	for(int i = 0 ; i < taskNum ; i ++) {
		for(int j = 0 ; j < taskNum ; j ++) {
			fscanf(fp, "%d", &tg[i][j]);
			if(i == j || tg[i][j] == INFINITE) {
				continue;
			}
			Edge e;
			e.cost = tg[i][j];
			e.nextNode = j;
			Tg[i].push_back(e);
		}
	}

	graph_link_test();

	fclose(fp);
}

void write_result() {
	FILE *fp;
	char path[128] = "/Users/chenqixian/Desktop/MyBUAA/0-Codes/0-Algorithm/APHiD/output";

	if((fp = fopen(path, "w")) == NULL) {
		printf("Error: file cannot write!\n");
		return;
	}

	fprintf(fp, "\nbottom-up APHiD result is : \n");
	for(int i = 0 ; i < result1.size() ; i ++) {
		fprintf(fp, "%d ", result1[i]);
	}
	fprintf(fp, "\n");
	fprintf(fp, "\ntop-down APHiD result is : \n");
	for(int i = 0 ; i < result2.size() ; i ++) {
		fprintf(fp, "%d ", result2[i]);
	}
	fprintf(fp, "\n");
	fclose(fp);
}

#endif //APHID_FILE_IO_H