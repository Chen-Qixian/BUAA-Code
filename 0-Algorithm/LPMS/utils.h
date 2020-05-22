/**
 * LPG Algorithm - utils
 * Author: Chris.Chen
 * Date: 05/03/2020
 * Desc: Label Propagation based Process Grouping Algorithm
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */
#ifndef LPMS_UTILS_H
#define LPMS_UTILS_H

#include <vector>
#include <ctime>

using namespace std;

void graph_init();
int random_choose(vector<int> v);
void eliminate_ele(vector<int>& v, int ele);
void update_global_deg();

/* 
	communication graph initialization
	@param {void}
	@return {void}
*/
void graph_init() {
	// reset graph info
	P_l.clear();
	P_u.clear();
	availableGroup.clear();
	for(int i = 0 ; i < N ; i ++) {
		edge[i].clear();
		groupResult[i].clear();
	}
}

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
	get global degree for each node 
	@param {void} 
	@return {vector<int>} a vector contains each node's global degree
*/
void update_global_deg() {
	nodeDeg.clear();
	for(int i = 0; i < procNum ; i ++) {
		int deg = edge[i].size();
		if(deg == 1 && edge[i][0].nextNode == UNDEFINED) {
			deg = 0;
		}
		nodeDeg.push_back(deg);

	}
}
#endif // LPMS_UTILS_H