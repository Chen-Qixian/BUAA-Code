/**
 * QTLS Algorithm - test-helper
 * Author: Chris.Chen
 * Date: 06/03/2020
 * Desc: Quadratic Time Complexity Locality Detection Algorithm for Shared HPC Systems
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */
#ifndef QTLS_TEST_HELPER_H
#define QTLS_TEST_HELPER_H

using namespace std;

void vector_test(char *msg, vector<int> v);
void graph_test();
void run_test(bool enabled);


/*
	vector iter test
	@param {char *} test hint message
	@param {vector<int>} tested vector
	@return {void}
*/
void vector_test(char *msg, vector<int> v) {
	printf("\n%s: ", msg);
	for(int i = 0 ; i < v.size() ; i ++) {
		printf("%d ", v[i]);
	}
	printf("\n");
}

/*
	communication graph initialization result test
	@param {void}
	@return {void}
*/
void graph_test() {
	printf("*****graph test begin*****\n");
	for(int i = 0 ; i < procNum ; i ++) {
		int len = edge[i].size();
		printf("%d -> ", i);
		for(int j = 0 ; j < len ; j ++) {
			printf("%d, %d; ", edge[i][j].nextNode , edge[i][j].cost);
		}
		printf("\n");
	}
}

/*
	running test
	@param {bool} enable test running
	@return {void}
*/
void run_test(bool enabled) {
	if(!enabled) return;
	graph_test();
	printf("******core list test*****\n");
	for(auto it = m.begin() ; it != m.end() ; it ++) {
		printf("%d -> %d ; ", it->id , it->num);
	}
}

/*
	merge scheme test
	@param {vector<PII>} merge scheme list
	@return {void}
*/
void merge_scheme_test(vector<PII> merge) {
	printf("\n****merge scheme test*****\n");
	for(int i = 0 ; i < merge.size() ; i ++) {
		printf("<%d, %d>, ", merge[i].first, merge[i].second);
	}
	printf("\n");
}

/*
	P_g test
	@param {vector<PII>} merge scheme list
	@return {void}
*/
void pg_test() {
	for(int i = 0 ; i < procNum ; i ++) {
		printf("\n%d -> ", i);
		int s = P_g[i].size();
		for(int j = 0 ; j < s ; j ++) {
			printf("%d ", P_g[i][j]);
		}
	}
	printf("\n");
}

void result_test() {
	printf("\n======result test begin======\n");
	for(int i = 0 ; i < n ; i ++) {
		printf("group: %d -> ", i);
		int size = result[i].size();
		sort(result[i].begin(), result[i].end());
		for(int j = 0 ; j < size ; j ++) {
			printf("%d ", result[i][j]);
		}
		printf("\n");
	}
}

#endif // QTLS_TEST_HELPER_H









