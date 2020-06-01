/**
 * QTLS Algorithm - utils
 * Author: Chris.Chen
 * Date: 06/03/2020
 * Desc: Quadratic Time Complexity Locality Detection Algorithm for Shared HPC Systems
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */
#ifndef QTLS_UTILS_H
#define QTLS_UTILS_H

#include <vector>
#include <ctime>

using namespace std;

int random_choose(vector<int> v);
void eliminate_ele(vector<int>& v, int ele);

/* 
	eliminate a given element from given vector 
	@param {vector<int>} dest vector to eliminate 
	@param {int} given element
	@return {void}
*/
void eliminate_ele(vector<int>& v, int ele) {
	for(auto it = v.begin() ; it != v.end() ; it ++) {
		if(*it == ele) {
			v.erase(it);
			return;
		}
	}
}

/*
	choose an element from vector randomly
	@param {vector<int>} a int vector to choose element from
	@return {int} randomly chosen element
*/
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
	// return v[0];
}

/*
	initialize global var
	@param {void}
	@return {void}
*/
void global_var_init () {
	for(int i = 0 ; i < N ; i ++) {
		edge[i].clear();
		result[i].clear();
		P_g[i].clear();
	}
	m.clear();
}


/*
	initialize process groups P_g
	@param {void}
	@return {void}
*/
void group_initialize() {
	for(int i = 0 ; i < procNum ; i ++) {
		P_g[i].push_back(i);
	}
}

#endif // QTLS_UTILS_H


















