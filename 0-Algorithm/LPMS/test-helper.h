/**
 * LPG Algorithm - test-helper
 * Author: Chris.Chen
 * Date: 05/03/2020
 * Desc: Label Propagation based Process Grouping Algorithm
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */
#ifndef LPMS_TEST_HELPER_H
#define LPMS_TEST_HELPER_H

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
}

/*
	communication graph initialization result test
	@param {void}
	@return {void}
*/
void graph_test() {
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
	vector_test((char*)"P_l", P_l);
	vector_test((char*)"P_u", P_u);
}



#endif // LPMS_TEST_HELPER_H