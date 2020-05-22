/**
 * APHiD - test-helper
 * Authors: Chris.Chen
 * Date: 10/03/2020
 * Desc: Algorithm for Placing process Hierarchically on D-ary trees
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */

#ifndef APHID_TEST_HELPER_H
#define APHID_TEST_HELPER_H

using namespace std;

void graph_matrix_test(int size, int g[N][N]) {
	printf("===graph matrix test begin===\n");
	for(int i = 0 ; i < size ; i ++) {
		for(int j = 0 ; j < size; j ++) {
			printf("%d ", g[i][j]);
		}
		printf("\n");
	}
}

void graph_link_test() {
	printf("===graph link test begin===\n");
	for(int i = 0 ; i < taskNum ; i ++) {
		int len = Tg[i].size();
		printf("%d -> ", i);
		for(int j = 0 ; j < len ; j ++) {
			printf("%d, %d; ", Tg[i][j].nextNode , Tg[i][j].cost);
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

void picks_test() {
	printf("\n===picks test===\n");
	for(int i = 0 ; i < picks.size() ; i ++) {
		printf("a:%d b:%d w:%d\n", picks[i].a, picks[i].b, picks[i].w);	
	}
}

void group_test() {
	printf("\n===groups test===\n");
	for(int i = 0 ; i < groups.size() ; i ++) {
		for(int j = 0 ; j < groups[i].size() ; j ++) {
			printf("%d ", groups[i][j]);
		}
		printf("\n");
	}
}

void v_test(vector<int> v[N]) {
	printf("\n===dGroups test===\n");
	for(int i = 0 ; i < D ; i ++) {
		int size = v[i].size();
		for(int j = 0 ; j < size ; j ++) {
			printf("%d ", v[i][j]);
		}
		printf("\n");
	}
}

#endif //APHID_TEST_HELPER_H
