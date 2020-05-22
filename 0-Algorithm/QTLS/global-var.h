/**
 * QTLS Algorithm - global-var
 * Author: Chris.Chen
 * Date: 06/03/2020
 * Desc: Quadratic Time Complexity Locality Detection Algorithm for Shared HPC Systems
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */
#ifndef QTLS_GLOBAL_VAR_H
#define QTLS_GLOBAL_VAR_H

#include <vector>

#define N 1024
#define INFINITE -1

using namespace std;

typedef pair<int, int> PII;
// communication graph edge link weight
struct Edge {
	int nextNode;
	int cost;
};
// core list info
struct Cores {
	int id; // process group id
	int num; // number of cores

	bool operator < (const Cores& C) const {
		return num > C.num;
	}
};
// weight contribution
struct WeiCon {
	int node;
	int con;
	bool operator <(const WeiCon& wc) const{
		return con < wc.con;
	}
};

vector<Edge> edge[N]; // communication topology
int graph[N][N]; // communication topology
int n; // calculation node number = process group number
vector<Cores> m; // core number list
int procNum; // process number
vector<int> result[N]; // grouping result
vector<int> P_g[N]; // unsatisfied groups
vector<int> lastPga;
vector<int> lastPgb;

#endif // QTLS_GLOBAL_VAR_H












