/**
 * APHiD - utils
 * Authors: Chris.Chen
 * Date: 10/03/2020
 * Desc: Algorithm for Placing process Hierarchically on D-ary trees
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */

#ifndef APHID_UTILS_H
#define APHID_UTILS_H

#include <ctime>

using namespace std;

void global_var_init() {
	for(int i = 0 ; i < taskNum ; i ++) {
		Tg[i].clear();
	}
	groups.clear();
	tmpGroups.clear();
	result1.clear();
	result2.clear();
	picks.clear();
}

void merge_vector(vector<int> &a, vector<int> b) {
	int size = b.size();
	for(int i = 0 ; i < size; i ++) {
		a.push_back(b[i]);
	}
}

int random_choose(vector<int> v) {
	int size = v.size();
	if(size <= 0) {
		printf("[Error]: random_choose required a vector size greter than 0!");
		return 0;
	}
	if(size == 1) {
		return v[0];
	}
	srand(time(NULL));
	return v[rand() % size];
}

int randint(int r) {
	srand(time(NULL));
	return rand() % r;
}

#endif //APHID_UTILS_H





