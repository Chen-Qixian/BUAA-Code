/**
 * TopoMapping - utils
 * Authors: Chris.Chen
 * Date: 09/03/2020
 * Desc: Automatic Topology Mapping of Diverse Large-scale Parallel Applications
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */

#ifndef TOPO_UTILS_H
#define TOPO_UTILS_H

using namespace std;

void init_global_var() {
	task_size = TASK_NUM; // task number
	net_core = CORE_PER_NODE; // cores per node
	net_node = NODE_NUM; // node number
	net_ct = net_core * net_node; // total core number
	for(int i = 0; i < task_size ; i ++) {
		Tg[i].clear();
	}
	for(int i = 0 ; i < PROCESS_NUM ; i ++) {
		Slist[i].clear();
	}
	for(int i = 0 ; i < TASKORDERS ; i ++) {
		taskList[i].clear();
	}
}

int get_node_order(int core) {
	return core / net_core;
}

PII get_core_range(int node) {
	int begin = node * net_core;
	int end = (node + 1) * net_core;
	return make_pair(begin, end);
}

int get_node_distance(int i, int j) {
	return ng[i][j];
}

int get_weight(int i, int j) {
	return tg[i][j];
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

#endif //TOPO_UTILS_H





