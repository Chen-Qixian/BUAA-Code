/**
 * LPG Algorithm - label-update
 * Author: Chris.Chen
 * Date: 05/03/2020
 * Desc: Label Propagation based Process Grouping Algorithm
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */
#ifndef LPMS_LABEL_UPDATE_H
#define LPMS_LABEL_UPDATE_H
#include "utils.h"
#include "stdio.h"

using namespace std;

void label_update();

/*
	choose a group randomly for a 0-degree node from available group
	@param {void}
	@return {void} 
*/
int randomLabel() {
	return random_choose(availableGroup);
}

/*
	calculate distance from node to a group
	@param {int} current node to be consider
	@return {vector<int>} -1 when dist is INF, 
						  otherwise calculate sum of distance to nodes in each group
*/
vector<int> calculateDist(int curNode) {
	vector<int> ret;
	for(int i = 0 ; i < n ; i ++) {
		int dist = 0;
		int size = groupResult[i].size();
		for(int j = 0 ; j < size; j ++) {
			int curDist = graph[curNode][groupResult[i][j]];
			if(curDist == INFINITE) continue;
			dist += curDist;
		}
		if(dist == 0) {
			dist = -1;
		}
		ret.push_back(dist);
	}
	return ret;
}

/*
	judge if current group is available
	@param {int} current group id
	@return {bool} return true when available; otherwise unavailable
*/
bool group_available(int groupId) {
	return (find(availableGroup.begin(), availableGroup.end(), groupId) != availableGroup.end());
}

/*
	iteration entrance
	@param {int} current node to be allocated
	@return {int} UNALLOC when none group reachable;
				  otherwise choose group with max distance
*/
int allocLabel(int curNode) {
	vector<int> dist;
	dist = calculateDist(curNode);
	vector_test((char *)"dist", dist);
	int maxDist = 0;
	vector<int> maxDistGroup;
	maxDistGroup.clear();
	for(int i = 0 ; i < n ; i ++) {
		if(!group_available(i)) continue; // judge if current group is available
		if(dist[i] > maxDist) {
			maxDist = dist[i];
			maxDistGroup.clear();
			maxDistGroup.push_back(i);
		}
		else if(dist[i] == maxDist) {
			maxDistGroup.push_back(i);
		}
	}
	if(maxDistGroup.size() == 0) {
		return UNALLOC;
	}
	return random_choose(maxDistGroup);
}

/*
	update P_l P_u according to label, save updating result to tmp
	@param {void}
	@return {void} 
*/
void update(int curNode, vector<int>& tmp) {
	eliminate_ele(P_u, curNode);
	P_l.push_back(curNode);
	tmp.push_back(curNode);
	vector_test((char *)"new P_u", P_u);
	vector_test((char *)"new P_l", P_l);
}

/*
	update label
	@param {void}
	@return {void} 
*/
void label_update() {
	vector<int> tmpResult[n];
	for(int i = 0 ; i < P_u.size() ; ) {
		int curNode = P_u[i];
		printf("\n======labeling node: %d=======", curNode);
		int label;
		if(nodeDeg[curNode] == 0) {
			label = randomLabel();
		}
		else {
			label = allocLabel(curNode);
		}
		if(label == UNALLOC) {
			i ++;
		}
		else {
			update(curNode, tmpResult[label]);
		}	
	}
	printf("\n");
	for(int i = 0 ; i < n ; i ++) {
		printf("group %d: ", i);
		int size = tmpResult[i].size();
		for(int j = 0 ; j < size ; j ++){
			groupResult[i].push_back(tmpResult[i][j]);
		}
		size = groupResult[i].size();
		for(int j = 0 ; j < size ; j ++) {
			printf("%d ", groupResult[i][j]);
		}
		printf("\n");
	}
}

#endif // LPMS_LABEL_UPDATE_H