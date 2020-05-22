/**
 * TopoMapping - task-graph
 * Authors: Chris.Chen
 * Date: 09/03/2020
 * Desc: Automatic Topology Mapping of Diverse Large-scale Parallel Applications
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */

#ifndef TOPO_TASK_GARAPH_H
#define TOPO_TASK_GARAPH_H
#include <queue>

using namespace std;

vector<int> OO() {
	vector<int> ret;
	for(int i = 0 ; i < task_size ; i ++) {
		ret.push_back(i);
	}
	return ret;
}

vector<int> BFS(int start) {
	bool mask[task_size];
	for(int i = 0 ; i < task_size ; i ++) {
		mask[i] = false;
	}
	vector<int> ret;
	queue<int> Q;
	Q.push(start);
	mask[start] = true;
	while(Q.empty() == false) {
		int now = Q.front();
		ret.push_back(now);
		Q.pop();
		int size = Tg[now].size();
		for(int i = 0 ; i < size ; i ++) {
			int nextNode = Tg[now][i].nextNode;
			if(mask[nextNode]) {
				continue;
			}
			else {
				Q.push(nextNode);
				mask[nextNode] = true;
			}
		}
	}
	return ret;
}

void DFS(int x, vector<int> &ret, bool mask[]) {
	int size = Tg[x].size();
	for(int i = 0 ; i < size ; i ++) {
		int now = Tg[x][i].nextNode;
		if(mask[now]) {
			continue;
		}
		else {
			mask[now] = true;
			ret.push_back(now);
			DFS(now, ret, mask);
		}
	}
} 

	
vector<int> BFS_DFS(int start) {
	vector<int> ret;
	bool mask[task_size];
	for(int i = 0 ; i < task_size ; i ++) {
		mask[i] = false;
	}
	mask[start] = true;
	ret.push_back(start);
	DFS(start, ret, mask);
	return ret;
}

#endif //TOPO_TASK_GARAPH_H











