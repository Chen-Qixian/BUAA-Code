/**
 * TopoMapping - test-helper
 * Authors: Chris.Chen
 * Date: 09/03/2020
 * Desc: Automatic Topology Mapping of Diverse Large-scale Parallel Applications
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */

#ifndef TOPO_TEST_HELPER_H
#define TOPO_TEST_HELPER_H

using namespace std;

void graph_test(int size, int g[N][N]) {
	printf("===graph test begin===\n");
	for(int i = 0 ; i < size ; i ++) {
		for(int j = 0 ; j < size; j ++) {
			printf("%d ", g[i][j]);
		}
		printf("\n");
	}
}

void Tg_test() {
	printf("===Tg test begin===\n");
	for(int i = 0 ; i < task_size ; i ++) {
		printf("task: %d ->", i);
		for(int j = 0 ; j < Tg[i].size() ; j ++) {
			printf("%d ", Tg[i][j]);
		}
		printf("\n");
	}
}

void vector_test(char *msg, vector<int> v) {
	printf("\n%s: ", msg);
	for(int i = 0 ; i < v.size() ; i ++) {
		printf("%d ", v[i]);
	}
	printf("\n");
}

#endif //TOPO_TEST_HELPER_H