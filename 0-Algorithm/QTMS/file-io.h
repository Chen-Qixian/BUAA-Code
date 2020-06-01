/**
 * QTLS Algorithm - file-io
 * Author: Chris.Chen
 * Date: 06/03/2020
 * Desc: Quadratic Time Complexity Locality Detection Algorithm for Shared HPC Systems
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */
#ifndef QTLS_FILE_IO_H
#define QTLS_FILE_IO_H

using namespace std;

/* 
	read input file & get network topology and communication topology
	@param {void}
	@return {void}
*/
void read_file() {
	FILE *fp;
	char inputPath[128] = "/Users/chenqixian/Desktop/MyBUAA/0-Codes/0-Algorithm/QTLS/input";

	if((fp = fopen(inputPath, "r")) == NULL) {
		printf("Error: file cannot read!\n");
		return;
	}
	fscanf(fp, "%d", &n);
	for(int i = 0 ; i < n ; i ++) {
		Cores c;
		c.id = i;
		fscanf(fp, "%d", &c.num);
		m.push_back(c);
	}
	sort(m.begin(), m.end());
	fscanf(fp, "%d", &procNum);
	for(int i = 0 ; i < procNum ; i ++) {
		for(int j = 0 ; j < procNum ; j ++) {
			fscanf(fp, "%d", &graph[i][j]);
			if(graph[i][j] > 0) {
				Edge e;
				e.nextNode = j;
				e.cost = graph[i][j];
				edge[i].push_back(e);
			}
		}
	}
	fclose(fp);
}

void write_result() {
	FILE *fp;
	char outputPath[128] = "/Users/chenqixian/Desktop/MyBUAA/0-Codes/0-Algorithm/QTLS/output";

	if((fp = fopen(outputPath, "w")) == NULL) {
		printf("Error: file cannot write!\n");
		return;
	}

	for(int i = 0 ; i < n ; i ++) {
		fprintf(fp, "group %d: ", i);
		int size = result[i].size();
		sort(result[i].begin(), result[i].end());
		for(int j = 0 ; j < size ; j ++) {
			fprintf(fp, "%d ", result[i][j]);
		}
		fprintf(fp, "\n");
	}

	fclose(fp);
}

#endif // QTLS_FILE_IO_H