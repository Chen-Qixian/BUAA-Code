/**
 * TopoMapping - file-io
 * Authors: Chris.Chen
 * Date: 09/03/2020
 * Desc: Automatic Topology Mapping of Diverse Large-scale Parallel Applications
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */

#ifndef TOPO_FILE_IO_H
#define TOPO_FILE_IO_H

using namespace std;

void read_task_graph() {
	FILE *fp;
	char path[128] = "/Users/chenqixian/Desktop/MyBUAA/0-Codes/0-Algorithm/TopoMapping/task_graph.lgl";

	if((fp = fopen(path, "r")) == NULL) {
		printf("Error: file cannot read!\n");
		return;
	}
	char s[10];
	int curTask, nextTask;
	int t;
	while(fscanf(fp, "%s%d", s, &t) > 0) {
		if(strcmp(s, "#") == 0) {
			curTask = t;
		}
		else {
			nextTask = stoi(s);
			tg[curTask][nextTask] = t;
			Edge e;
			e.nextNode = nextTask;
			e.cost = t;
			Tg[curTask].push_back(e);
		}
	}
	// graph_test(task_size, tg);
	// Tg_test();
	fclose(fp);
}

void read_net_graph() {
	FILE *fp;
	char path[128] = "/Users/chenqixian/Desktop/MyBUAA/0-Codes/0-Algorithm/TopoMapping/net_graph.txt";

	if((fp = fopen(path, "r")) == NULL) {
		printf("Error: file cannot read!\n");
		return;
	}

	for(int i = 0 ; i < net_node ; i ++) {
		for(int j = 0 ; j < net_node ; j ++) {
			fscanf(fp, "%d", &ng[i][j]);
		}
	}

	// graph_test(net_node, ng);

	fclose(fp);
}

void write_result() {
	FILE *fp;
	char path[128] = "/Users/chenqixian/Desktop/MyBUAA/0-Codes/0-Algorithm/TopoMapping/output";

	if((fp = fopen(path, "w")) == NULL) {
		printf("Error: file cannot write!\n");
		return;
	}

	fprintf(fp, "\nTopoMapping result is : \n");
	for(int i = 0 ; i < result.size() ; i ++) {
		fprintf(fp, "%d ", result[i]);
	}
	fprintf(fp, "\n");
	fclose(fp);
}

#endif //TOPO_FILE_IO_H