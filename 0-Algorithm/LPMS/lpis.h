/**
 * LPG Algorithm - lpis
 * Author: Chris.Chen
 * Date: 05/03/2020
 * Desc: Label Propagation based Process Grouping Algorithm
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */
#ifndef LPMS_LPIS_H
#define LPMS_LPIS_H
#include "stdio.h"
#include "utils.h"

using namespace std;
void lpis();
vector<int> get_inter_deg();
int align_upper(int x);
int align_lower(int x);


/*
	Locality-Preserving Initialize Scheme
	@param {void}
	@return {void}
*/
void lpis() {
	vector<int> deg;
	deg = get_inter_deg(); 
	// vector_test((char *)"deg:", deg);
	int cnt = 0;
	int curLabel = 0;
	int maxDeg = 0;
	int firstLabel;
	vector<int> maxNodes;
	for(int i = 0 ; i < procNum ; i ++) {
		if(deg[i] > maxDeg) {
			maxDeg = deg[i];
			maxNodes.clear();
			maxNodes.push_back(i);
		}
		else if(deg[i] == maxDeg) {
			maxNodes.push_back(i);
		}
		cnt ++;
		if(cnt == m) {
			firstLabel = random_choose(maxNodes);
			groupResult[curLabel].push_back(firstLabel);
			cnt = 0;
			curLabel ++;
			maxDeg = 0;
			maxNodes.clear();
		}
	}
	if(cnt != 0) {
		firstLabel = random_choose(maxNodes);
		groupResult[curLabel].push_back(firstLabel);
	}
	printf("\nlpis picked: ");
	for(int i = 0 ; i < n ; i ++) {
		printf("%d ", groupResult[i][0]);
		eliminate_ele(P_u, groupResult[i][0]);
		P_l.push_back(groupResult[i][0]);
	}
}

/* 
	private: get natural group inter-degree for each node 
	@param {void} 
	@return {vector<int>} a vector contains each node's group inter degree
*/
vector<int> get_inter_deg() {
	vector<int> ret;
	for(int i = 0; i < procNum ; i ++) {
		int size = edge[i].size();
		int inter_deg = 0;
		for(int j = 0 ; j < size ; j ++) {
			// out of this group, break
			if(edge[i][j].nextNode >= align_upper(i)) {
				break;
			}
			// find node belongs to this group
			if(edge[i][j].nextNode >= align_lower(i)) {
				inter_deg ++;
			}
		}
		ret.push_back(inter_deg);
	}
	return ret;
}

/* 
	find next natural group start
	@param {int} current node id
	@return {int} next natural group start node id
*/
int align_upper(int x) {
	return x - x % m + m;
}

/* 
	find current natural group start
	@param {int} current node id
	@return {int} current natural group start node id
*/
int align_lower(int x) {
	return x - x % m;
}
#endif // LPMS_LPIS_H