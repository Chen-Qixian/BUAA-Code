/**
 * LPG Algorithm - global-var
 * Author: Chris.Chen
 * Date: 05/03/2020
 * Desc: Label Propagation based Process Grouping Algorithm
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */
#ifndef LPMS_GLOBAL_VAR_H
#define LPMS_GLOBAL_VAR_H

#include <vector>

#define N 2048 // upper-bound process number
#define INFINITE -1
#define UNALLOC -2
#define UNDEFINED -3

using namespace std;

// Communication graph edge structure definition
struct Edge {
	int nextNode;
	int cost;
};

struct WeiCon {
	int node;
	int con;
	bool operator <(const WeiCon& wc) const{
		return con < wc.con;
	}
};
vector<Edge> edge[N]; // graph info
int graph[N][N]; // origin graph
int n; // number of process group
int m; // number of cores (upper-bound limit for each process group)
int procNum; // number of process
vector<int> P_l; // labeled process
vector<int> P_u; // unlabeled process
vector<int> P_f; // full process group (to be eliminated from P)
vector<int> groupResult[N]; // label group result
vector<int> availableGroup; // mark groups not full
vector<int> nodeDeg; // global degree for each node

#endif // LPMS_GLOBAL_VAR_H