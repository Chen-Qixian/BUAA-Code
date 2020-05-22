/**
 * TopoMapping - global-var
 * Authors: Chris.Chen
 * Date: 09/03/2020
 * Desc: Automatic Topology Mapping of Diverse Large-scale Parallel Applications
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */

#ifndef TOPO_GLOBAL_VAR_H
#define TOPO_GLOBAL_VAR_H
#include <vector>
#include <string>
#include <cstring>

#define N 1024

#define OOLIST 0
#define BFSLIST 1
#define DFSLIST 2
#define TASKORDERS 3 // OO BFS DFS == 3

#define TASK_NUM 128
#define CORE_PER_NODE 3
#define NODE_NUM 48
#define PROCESS_NUM 6

#define ALPHA 10 // validation
#define SCALE 1 // int limit

using namespace std;

typedef pair<int, int> PII;

struct Edge {
	int nextNode;
	int cost;
};

struct HopBytes {
	int task; 
	int hopbytes;
};

struct Pick {
	int id;
	int val;

	bool operator < (const Pick &a) const {
		return val < a.val;
	}
};

int tg[N][N]; // task graph
vector<Edge> Tg[N]; // save adjacent
int ng[N][N]; // net graph
int task_size; // task number
int net_ct; // total core number
int net_core; // cores per node
int net_node; // node number
vector<int> taskList[TASKORDERS]; // OO BFS DFS task list
vector<int> Slist[N]; // greedy_map result list
vector<int> result;


#endif //TOPO_GLOBAL_VAR_H